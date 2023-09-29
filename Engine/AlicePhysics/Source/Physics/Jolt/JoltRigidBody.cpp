#include "JoltRigidBody.h"

void* AlicePhysics::JoltRigidBody::GetBody()
{
    return static_cast<void*>(&bodyData);
}

void AlicePhysics::JoltRigidBody::OnCollisionEnter()
{
	if ( rigidBodyCollision )
	{
		rigidBodyCollision->OnCollisionEnter();
	}
}

void AlicePhysics::JoltRigidBody::OnCollisionStay()
{
	if ( rigidBodyCollision )
	{
		rigidBodyCollision->OnCollisionStay();
	}
}

void AlicePhysics::JoltRigidBody::OnCollisionExit()
{
	if ( rigidBodyCollision )
	{
		rigidBodyCollision->OnCollisionExit();
	}
}

void AlicePhysics::JoltRigidBody::SetLinearVelocity(const AliceMathF::Vector3& inLinearVelocity_)
{
	body->SetLinearVelocity(inLinearVelocity_);
}

AliceMathF::Vector3 AlicePhysics::JoltRigidBody::GetPosition()
{
    return body->GetCenterOfMassPosition();
}

AliceMathF::Quaternion AlicePhysics::JoltRigidBody::GetRotation()
{
    return body->GetRotation();
}

float AlicePhysics::JoltRigidBody::GetFriction()
{
    return body->GetFriction();
}

float AlicePhysics::JoltRigidBody::GetRestitution()
{
    return body->GetRestitution();
}

AliceMathF::Vector3 AlicePhysics::JoltRigidBody::GetLinearVelocity()
{
    return body->GetLinearVelocity();
}

AliceMathF::Vector3 AlicePhysics::JoltRigidBody::GetAngularVelocity()
{
    return body->GetAngularVelocity();
}

void AlicePhysics::JoltRigidBody::SetRigidBodyCollision(RigidBodyCollision* rigidBodyCollision_)
{
	rigidBodyCollision = rigidBodyCollision_;
}

AliceMathF::Matrix4 AlicePhysics::JoltRigidBody::GetCenterOfMassTransform()
{
	return body->GetTransformedShape().GetCenterOfMassTransform();
}

bool AlicePhysics::JoltRigidBody::IsActive()
{
	return body->IsActive();
}
