#include "BossHandPhysics.h"
#include<AliceAssert.h>
#include<SphereShape.h>
void BossHandPhysics::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	AliceAssertNull(physicsSystem_,"PhysicsSystemがnullです");
	physicsSystem = physicsSystem_;

}

void BossHandPhysics::SetSetting(const AlicePhysics::IRigidBodyCreationSettings* setting_)
{
	shape.reset(AlicePhysics::CreateSphereShape(25.0f));

	physicsSystem->CreateRigidBody(rigidBody,setting_);
	physicsSystem->AddRigidBody(rigidBody);
	rigidBody->SetRigidBodyCollision(this);
}

void BossHandPhysics::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void BossHandPhysics::Update()
{
}

void BossHandPhysics::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,1.0f ,1.0f ,1.0f },true);
}

void BossHandPhysics::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
}

void BossHandPhysics::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
}

void BossHandPhysics::OnCollisionExit()
{
}

void BossHandPhysics::SetPosition(const AliceMathF::Vector3& pos_)
{
	rigidBody->SetPosition(pos_);
}

const AliceMathF::Vector3 BossHandPhysics::GetPosition()
{
	return rigidBody->GetPosition();
}
