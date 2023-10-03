#include "BossHand.h"

#include<CollisionAttribute.h>
#include<SphereShape.h>

void BossHand::Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	transform.parent = parent_;
	transform.Initialize();

	shape.reset(AlicePhysics::CreateSphereShape(5.0f));

	AlicePhysics::IRigidBodyCreationSettings lSetting;
	lSetting.name = "BossHand";
	lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
	lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
	lSetting.allowSleeping = false;
	lSetting.collisionGroup = CollisionGroup::BOSS;
	lSetting.collisionAttribute = CollisionAttribute::HAND;
	lSetting.position = transform.translation;
	lSetting.shape = shape.get();
	lSetting.userData = static_cast< void* >( &situation );
	lSetting.trigger = true;
	physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
	physicsSystem_->AddRigidBody(rigidBody);
}

void BossHand::Update(const std::string& boneName_, AliceBlendTree* tree_, AliceModel* playerModel_)
{
	animationTransform = playerModel_->GetAnimationTransform(tree_, boneName_);
}

void BossHand::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void BossHand::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_, &animationTransform.boneMatrix);

	rigidBody->SetMatrix(transform.rigidBodyMatWorld);
}

void BossHand::SetSituation(uint32_t situation_)
{
	situation = situation_;
}

void BossHand::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(), { 1.0f,1.0f ,1.0f }, { 0.8f ,0.0f ,0.0f ,1.0f }, true);
}

void BossHand::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_){}
void BossHand::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_) {}
void BossHand::OnCollisionExit() {}