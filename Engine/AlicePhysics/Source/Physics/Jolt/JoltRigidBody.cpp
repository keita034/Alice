#include "JoltRigidBody.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include <Jolt/Core/Color.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Core/HashCombine.h>
#include <Jolt/Core/UnorderedMap.h>
#include <Jolt/Math/Float2.h>
#include <Jolt/Geometry/IndexedTriangle.h>
#include <Jolt/Geometry/AABox.h>
ALICE_SUPPRESS_WARNINGS_END

void* AlicePhysics::JoltRigidBody::GetBody()
{
    return static_cast<void*>(&bodyData);
}

void AlicePhysics::JoltRigidBody::OnCollisionEnter(RigidBodyUserData* BodyData_)
{
	if ( rigidBodyCollision )
	{
		rigidBodyCollision->OnCollisionEnter(BodyData_);
	}
}

void AlicePhysics::JoltRigidBody::OnCollisionStay(RigidBodyUserData* BodyData_)
{
	if ( rigidBodyCollision )
	{
		rigidBodyCollision->OnCollisionStay(BodyData_);
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

void AlicePhysics::JoltRigidBody::SetPositionAndRotationInternal(const AliceMathF::Vector3& position_,const AliceMathF::Quaternion& rotation_)
{
	bodyInterface->SetPositionAndRotation(id,position_,rotation_,JPH::EActivation::Activate);
}

void AlicePhysics::JoltRigidBody::SetPosition(const AliceMathF::Vector3& position_)
{
	bodyInterface->SetPosition(id,position_,JPH::EActivation::Activate);
}

void AlicePhysics::JoltRigidBody::SetRotation(const AliceMathF::Quaternion& rotation_)
{
	bodyInterface->SetRotation(id,rotation_,JPH::EActivation::Activate);
}

void AlicePhysics::JoltRigidBody::SetMatrix(const AliceMathF::Matrix4& mat_)
{
	JPH::RMat44 lTmpmat = mat_;

	SetRotation(lTmpmat.GetQuaternion().Normalized());
	SetPosition(lTmpmat.GetTranslation());
}

AliceMathF::Matrix4 AlicePhysics::JoltRigidBody::GetCenterOfMassTransform()
{
	return body->GetTransformedShape().GetCenterOfMassTransform();
}

bool AlicePhysics::JoltRigidBody::IsActive()
{
	return body->IsActive();
}
