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

#include<JoltSphereShape.h>
#include<JoltBoxShape.h>
#include<JoltCapsuleShape.h>
#include<JoltMeshShape.h>

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

void AlicePhysics::JoltRigidBody::SetMatrix(const AliceMathF::Matrix4& matRigidBody_,const AliceMathF::Matrix4& matWorld_)
{
	JPH::RMat44 lMatRigidBody = matRigidBody_;

	SetPosition(AliceMathF::GetWorldPosition(matWorld_));
	lMatRigidBody.SetTranslation({ 0,0,0 });
	SetRotation(lMatRigidBody.GetQuaternion().Normalized());

}

AliceMathF::Matrix4 AlicePhysics::JoltRigidBody::GetCenterOfMassTransform()
{
	return body->GetTransformedShape().GetCenterOfMassTransform();
}

void AlicePhysics::JoltRigidBody::SetShape(IShape*& shape_)
{
	switch (shape_->GetShapeType())
	{
	case SPHERE:
	{
		JoltSphereShape* lShape = dynamic_cast<JoltSphereShape*>(shape_);
		lShape->shape = (JPH::SphereShape*)body->GetShape();
		lShape->radius = lShape->shape->GetRadius();
	}
	break;

	case BOX:
	{
		JoltBoxShape* lShape = dynamic_cast< JoltBoxShape* >( shape_ );
		lShape->shape = ( JPH::BoxShape* ) body->GetShape();
		lShape->halfExtent = lShape->shape->GetHalfExtent();
	}
	break;
	case CAPSULE:
	{
		JoltCapsuleShape* lShape = dynamic_cast< JoltCapsuleShape* >( shape_ );
		lShape->shape = ( JPH::CapsuleShape* ) body->GetShape();
		lShape->radius = lShape->shape->GetRadius();
		lShape->halfHeightOfCylinder = lShape->shape->GetHalfHeightOfCylinder();
	}
	break;
	case MESH:
	{
		JoltMeshShape* lShape = dynamic_cast< JoltMeshShape* >( shape_ );
		lShape->shape = ( JPH::MeshShape* ) body->GetShape();
	}
	break;
	case PHYSICSSHAPETYPE_COUNT:
	default:
		break;
	}
}

bool AlicePhysics::JoltRigidBody::IsActive()
{
	return body->IsActive();
}
