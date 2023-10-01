#include "JoltContactListener.h"

#include<JoltRigidBody.h>
#include<AliceAssert.h>

void AlicePhysics::JoltContactListener::SetRigidBodyManager(RigidBodyManager* rigidBodyManager_)
{
	AliceAssertNull(rigidBodyManager_,"マネージャーがnullです");
	rigidBodyManager = rigidBodyManager_;
}

JPH::ValidateResult AlicePhysics::JoltContactListener::OnContactValidate(const JPH::Body& inBody1,const JPH::Body& inBody2,JPH::RVec3Arg inBaseOffset,const JPH::CollideShapeResult& inCollisionResult)
{
	static_cast< void >( inBody1 );
	static_cast< void >( inBody2 );
	static_cast< void >( inBaseOffset );
	static_cast< void >( inCollisionResult );

	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void AlicePhysics::JoltContactListener::OnContactAdded(const JPH::Body& body1,const JPH::Body& body2,const JPH::ContactManifold& manifold,JPH::ContactSettings& ref_settings)
{

	RigidBodyUserData* lBody1Data = reinterpret_cast< RigidBodyUserData* >( body1.GetUserData() );
	RigidBodyUserData* lBody2Data = reinterpret_cast< RigidBodyUserData* >( body2.GetUserData() );

	IRigidBody * lBody1RigidBody = 	lBody1Data->GetRigidBody();
	lBody1RigidBody->OnCollisionEnter(lBody2Data);

	IRigidBody* lBody2RigidBody = lBody2Data->GetRigidBody();
	lBody2RigidBody->OnCollisionEnter(lBody1Data);

	static_cast< void >( manifold );
	static_cast< void >( ref_settings );
}

void AlicePhysics::JoltContactListener::OnContactPersisted(const JPH::Body& body1,const JPH::Body& body2,const JPH::ContactManifold& manifold,JPH::ContactSettings& ref_settings)
{
	RigidBodyUserData* lBody1Data = reinterpret_cast< RigidBodyUserData* >( body1.GetUserData() );
	RigidBodyUserData* lBody2Data = reinterpret_cast< RigidBodyUserData* >( body2.GetUserData() );

	IRigidBody* lBody1RigidBody = lBody1Data->GetRigidBody();
	lBody1RigidBody->OnCollisionStay(lBody2Data);

	IRigidBody* lBody2RigidBody = lBody2Data->GetRigidBody();
	lBody2RigidBody->OnCollisionStay(lBody1Data);

	static_cast< void >( manifold );
	static_cast< void >( ref_settings );
}

void AlicePhysics::JoltContactListener::OnContactRemoved(const JPH::SubShapeIDPair& subShapePair)
{
	IRigidBody* lBody1RigidBody = rigidBodyManager->GetRigidBody(subShapePair.GetBody1ID().GetIndex());
	lBody1RigidBody->OnCollisionExit();

	IRigidBody* lBody2RigidBody = rigidBodyManager->GetRigidBody(subShapePair.GetBody2ID().GetIndex());
	lBody2RigidBody->OnCollisionExit();

	static_cast< void >( subShapePair );
}
