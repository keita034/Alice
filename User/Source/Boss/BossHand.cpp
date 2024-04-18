#include "BossHand.h"

#include<CollisionAttribute.h>
#include<SphereShape.h>
#include<Player.h>
#include<AliceAssert.h>
#include<Boss.h>

void BossHand::Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossHandIndex index_,Boss* boss_,GPUParticleEmitter* emitter_)
{
	AliceAssertNull(parent_,"ParentがNullです。");
	AliceAssertNull(emitter_,"EmitterがNullです。");
	AliceAssertNull(physicsSystem_,"PhysicsSystemがNullです。");
	AliceAssertNull(boss_,"BossがNullです。");

	gpuParticleEmitter = emitter_;
	boss = boss_;
	parent = parent_;

	transform.translation = { 0.0f,1000.0f,0.0f };
	transform.parent = parent_;
	transform.Initialize();

	shape.reset(AlicePhysics::CreateSphereShape(25.0f));

	{
		AlicePhysics::IRigidBodyCreationSettings lSetting;
		lSetting.name = "BossHand";
		lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
		lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
		lSetting.allowSleeping = false;
		lSetting.collisionGroup = CollisionGroup::BOSS;
		lSetting.collisionAttribute = CollisionAttribute::HAND;
		lSetting.position = transform.translation;
		lSetting.shape = shape.get();
		lSetting.userData = static_cast< void* >( &bossUsData );
		lSetting.trigger = false;
		lSetting.mass = 1.0f;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
		rigidBody->SetRigidBodyCollision(this);
		bossUsData.index = index_;
	}


	if ( bossUsData.index == BossHandIndex::RIGHT )
	{
		modelGPUParticle = gpuParticleEmitter->GetAnimationModelGPUParticle("BossRightHandModelParticle");
		boneName = RIGHT_BONE_NAME;
	}
	else
	{
		modelGPUParticle = gpuParticleEmitter->GetAnimationModelGPUParticle("BossLeftHandModelParticle");
		boneName = LEFT_BONE_NAME;

	}

	suctionGPUParticle = gpuParticleEmitter->GetAnimationModelSuctionGPUParticle("BossLeftHandScatterinParticle");

	AliceAssertNull(modelGPUParticle,"modelGPUParticleIsNull");
	modelGPUParticle->InvisibleBoneMesh(MESH_NAME,"mixamorig:Hips");
	modelGPUParticle->VisibleBoneMesh(MESH_NAME,boneName);
	modelGPUParticle->EmitPlay();

	bossModelGPUParticle = boss->GetModelParticle();
	bossModelGPUParticle->InvisibleBoneMesh(MESH_NAME,boneName);


}

void BossHand::Update(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* playerModel_)
{
	animationTransform = playerModel_->GetAnimationTransform(tree_,boneName_);

	if ( hp <= 0 && !cutting )
	{
		AnimationMeshGPUParticle* lParticle = gpuParticleEmitter->GetAnimationMeshGPUParticle("BossHandScatteringParticle");
		lParticle->SetBoneMesh(MESH_NAME,boneName);
		modelGPUParticle->EmitStop();
		lParticle->EmitPlay();
		gpuParticleEmitter->ScatteringSetSpeed(400.0f);
		gpuParticleEmitter->ScatteringSetAccel({ 0,0,0 });
		AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(rigidBody->GetCenterOfMassTransform());
		gpuParticleEmitter->ScatteringSetCenterPos(centerPos);
		gpuParticleEmitter->AnimationMeshGPUParticleScattering("BossHandScatteringParticle");
		lParticle->EmitStop();

		hp = MAX_HP;
		cutting = true;
	}

	if ( isHit )
	{
		changeColorTimer--;
		modelGPUParticle->ChangeColor({ 0.6f,0.3f,0.1f,1.0f },MESH_NAME,boneName);

		if ( changeColorTimer <= 0 )
		{
			changeColorTimer = CHANGE_COLOR_MAX_TIMER;
			isHit = false;
		}
	}
	else
	{
		modelGPUParticle->ChangeColor(AliceMathF::Vector4Lerp(CHANGE_COLOR,DEFAULT_COLOR,static_cast< float >( hp ) / MAX_HP),MESH_NAME,boneName);
	}
}

void BossHand::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void BossHand::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_,&animationTransform.boneMatrix);

	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);

	if ( parent )
	{
		modelGPUParticle->SetMat(parent->matWorld);

		suctionGPUParticle->SetMat(parent->matWorld);

		AnimationMeshGPUParticle* lParticle = gpuParticleEmitter->GetAnimationMeshGPUParticle("BossHandScatteringParticle");
		lParticle->SetMat(parent->matWorld);
	}
}

void BossHand::SetSituation(uint32_t situation_)
{
	bossUsData.situation = situation_;
}

void BossHand::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 0.8f ,0.0f ,0.0f ,1.0f },true);
}

void BossHand::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
	boss->DamageEnterCalculation(BodyData_,hitPosdition_,0.5f);
	isHit = boss->DamageEnterCalculation(BodyData_,hitPosdition_,hp,1.0f,false,false);
}

void BossHand::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) {}
void BossHand::OnCollisionExit() {}

const AliceMathF::Matrix4 BossHand::GetCenterOfMassTransform() const
{
	return rigidBody->GetCenterOfMassTransform();
}

bool BossHand::GetDeath() const
{
	return death;
}

void BossHand::Reset()
{
	cutting = false;
	death = false;
	flag.ForceSetFlag(false);
}

bool BossHand::GetCutting() const
{
	return cutting;
}
