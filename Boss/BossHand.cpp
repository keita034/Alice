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

	{
		FireGPUParticleSetting lSetting;
		lSetting.radius = 10.0f;
		lSetting.isPlay = false;
		lSetting.emitLifeTime = -1.0f;
		lSetting.timeBetweenEmit = 0.005f;
		lSetting.maxParticles = 100000;
		lSetting.emitCount = 100000;
		lSetting.endColor = { 1.0f,0.1f,0.0f,1.0f };
		lSetting.startColor = { 1.0f,0.1f,0.0f,1.0f };
		lSetting.lifeTime = 4.2f;
		lSetting.size = 10.0f;
		lSetting.convergePointPosition = { 0.0f,3.5f,0.0f };
		lSetting.speed = 15.0f;
		fireGPUParticle->Emit(lSetting);

	}

	if ( bossUsData.index == BossHandIndex::RIGHT )
	{
		modelGPUParticle = gpuParticleEmitter->GetAnimationModelGPUParticle("BossRightHandModelParticle");
		AliceAssertNull(modelGPUParticle,"modelGPUParticleIs");
		modelGPUParticle->InvisibleBoneMesh("elemental_element1mesh.003","mixamorig:Hips");
		modelGPUParticle->VisibleBoneMesh("elemental_element1mesh.003","mixamorig:RightHand");

	}
	else
	{
		modelGPUParticle = gpuParticleEmitter->GetAnimationModelGPUParticle("BossLeftHandModelParticle");
		AliceAssertNull(modelGPUParticle,"modelGPUParticleIs");
		modelGPUParticle->InvisibleBoneMesh("elemental_element1mesh.003","mixamorig:Hips");
		modelGPUParticle->VisibleBoneMesh("elemental_element1mesh.003","mixamorig:LeftHand");
	}
}

void BossHand::Update(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* playerModel_)
{
	animationTransform = playerModel_->GetAnimationTransform(tree_,boneName_);
}

void BossHand::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	fireGPUParticle->EmitStop();
	fireGPUParticle = nullptr;
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void BossHand::TransUpdate(Camera* camera_)
{
	if ( hp > 0 )
	{
		transform.TransUpdate(camera_,&animationTransform.boneMatrix);

		rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
	}
	else
	{

		bossHandTrans.matWorld = transform.parent->matWorld;
		bossHandTrans.matWorld.m[ 3 ][ 0 ] = rigidBody->GetCenterOfMassTransform().m[ 3 ][ 0 ];
		bossHandTrans.matWorld.m[ 3 ][ 1 ] = rigidBody->GetCenterOfMassTransform().m[ 3 ][ 1 ];
		bossHandTrans.matWorld.m[ 3 ][ 2 ] = rigidBody->GetCenterOfMassTransform().m[ 3 ][ 2 ];
		transform.parent = &bossHandTrans;

		if ( flag )
		{
			timer++;
		}

		transform.TransUpdate(camera_);
		if ( !flag )
		{
			flag.Determine();
			rigidBody->AddForce({ 0,600,0 });
		}

		{
			AnimationMeshGPUParticle* lParticle = gpuParticleEmitter->GetAnimationMeshGPUParticle("BossHandScatteringParticle");
			lParticle->SetMat(transform.matWorld);
		}
	}

	modelGPUParticle->SetMat(transform.matWorld);
}

void BossHand::SetSituation(uint32_t situation_)
{
	bossUsData.situation = situation_;
}

void BossHand::SetFireGPUParticle(FireGPUParticle* fireGPUParticle_)
{
	fireGPUParticle = fireGPUParticle_;
}

void BossHand::ParticleEmit()
{
	//fireGPUParticle->EmitPlay();
}

void BossHand::ParticleStop()
{
	fireGPUParticle->EmitStop();
}

void BossHand::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 0.8f ,0.0f ,0.0f ,1.0f },true);
}

void BossHand::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
	if ( timer >= 4 && BodyData_->GetGroup() == CollisionGroup::FIELD && !isPa )
	{
		AnimationMeshGPUParticle* lParticle = gpuParticleEmitter->GetAnimationMeshGPUParticle("BossHandScatteringParticle");
		modelGPUParticle->EmitStop();
		modelGPUParticle->PlayAnimation();

		if ( bossUsData.index == BossHandIndex::RIGHT )
		{
			lParticle->SetBoneMesh("elemental_element1mesh.003","mixamorig:RightHand");
		}
		else
		{
			lParticle->SetBoneMesh("elemental_element1mesh.003","mixamorig:LeftHand");
		}

		lParticle->EmitPlay();
		gpuParticleEmitter->ScatteringSetSpeed(200.0f);
		gpuParticleEmitter->ScatteringSetAccel({ 0,40,0 });
		AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(rigidBody->GetCenterOfMassTransform());
		gpuParticleEmitter->ScatteringSetCenterPos(centerPos);
		gpuParticleEmitter->AnimationMeshGPUParticleScattering("BossHandScatteringParticle");
		lParticle->EmitStop();

		timer = 0;
		isPa = true;
		hp = MAX_HP;
		cutting = true;
		transform.translation = { 0.0f,1000.0f,0.0f };
		transform.parent = parent;
		death = false;

	}

	boss->DamageEnterCalculation(BodyData_,hitPosdition_,0.5f);
	boss->DamageEnterCalculation(BodyData_,hitPosdition_,hp,1.0f,false,false);

	if ( !death && !cutting && hp <= 0 )
	{
		death = true;
		modelGPUParticle->EmitPlay();
		modelGPUParticle->StopAnimation();
		rigidBody->AddForce({ 0,200000,0 });
		flag.SetFlag(true);
		flag2.SetFlag(true);

		if ( bossUsData.index == BossHandIndex::RIGHT )
		{
			transform.translation = { 100 * 20.0f,100 * -35.0f ,100 * 40.0f };
			boss->GetModelParticle()->InvisibleBoneMesh("elemental_element1mesh.003","mixamorig:RightHand");
		}
		else
		{
			transform.translation = { 100 * -20.0f,100 * -35.0f ,100 * 40.0f };
			boss->GetModelParticle()->InvisibleBoneMesh("elemental_element1mesh.003","mixamorig:LeftHand");
		}
	}
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
	isPa = false;
	flag.ForceSetFlag(false);
}

bool BossHand::GetCutting() const
{
	return cutting;
}
