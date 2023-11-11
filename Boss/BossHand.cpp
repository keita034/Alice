#include "BossHand.h"

#include<CollisionAttribute.h>
#include<SphereShape.h>

void BossHand::Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossHandIndex index_)
{
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
		lSetting.trigger = true;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
		bossUsData.index = index_;
	}

	{
		FireGPUParticleSetting lSetting;
		lSetting.radius = 10.0f;
		lSetting.isPlay = false;
		lSetting.emitLifeTime = -1.0f;
		lSetting.timeBetweenEmit = 0.005f;
		lSetting.maxParticles = 10000;
		lSetting.emitCount = 100;
		lSetting.startColor = { 1,0.01f,0.01f,1 };
		lSetting.endColor = {1.0f,0.0f,0.0f,1.0f};
		lSetting.lifeTime = 4.2f;
		lSetting.size = 10.0f;
		lSetting.convergePointPosition = { 0.0f,3.5f,0.0f };
		lSetting.speed = 15.0f;
		particleIndex = fireGPUParticle->Emit(lSetting);

	}
}

void BossHand::Update(const std::string& boneName_, AliceBlendTree* tree_, AliceModel* playerModel_)
{

	animationTransform = playerModel_->GetAnimationTransform(tree_, boneName_);
}

void BossHand::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	fireGPUParticle = nullptr;
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void BossHand::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_, &animationTransform.boneMatrix);

	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);

	fireGPUParticle->SetPos(AliceMathF::GetWorldPosition(transform),particleIndex);
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
	fireGPUParticle->EmitPlay(particleIndex);
}

void BossHand::ParticleStop()
{
	fireGPUParticle->EmitStop(particleIndex);
}

void BossHand::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(), { 1.0f,1.0f ,1.0f }, { 0.8f ,0.0f ,0.0f ,1.0f }, true);
}

void BossHand::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_){}
void BossHand::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_) {}
void BossHand::OnCollisionExit() {}