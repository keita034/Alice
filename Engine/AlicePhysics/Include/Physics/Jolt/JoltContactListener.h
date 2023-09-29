#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN


#include <Jolt/Jolt.h>
#include<Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/ContactListener.h>
ALICE_SUPPRESS_WARNINGS_END

#include<RigidBodyManager.h>

namespace AlicePhysics
{
	class JoltContactListener : public JPH::ContactListener
	{
	private:

		RigidBodyManager* rigidBodyManager = nullptr;

	public:

		void SetRigidBodyManager(RigidBodyManager* rigidBodyManager_);

		virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;

		virtual void OnContactAdded(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold& manifold, JPH::ContactSettings& ref_settings) override;

		virtual void OnContactPersisted(const JPH::Body& body1, const JPH::Body& body2, const JPH::ContactManifold& manifold, JPH::ContactSettings& ref_settings) override;

		virtual void OnContactRemoved(const JPH::SubShapeIDPair& subShapePair) override;

		JoltContactListener() = default;
		~JoltContactListener() = default;

	};

}