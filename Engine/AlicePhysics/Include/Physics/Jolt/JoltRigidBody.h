#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

ALICE_SUPPRESS_WARNINGS_END

#include<RigidBodyUserData.h>
#include<JoltPhysics.h>

namespace AlicePhysics
{

	class JoltRigidBody final : public IRigidBody
	{
	private:
		friend JoltPhysics;
	public:

		struct BodyData
		{
			JPH::BodyID id;
			bool isActive;
			bool isAdd;
		};

	private:

		JPH::Body* body;
		JPH::BodyID id;
		bool isActive;

		BodyData bodyData;
		std::unique_ptr<RigidBodyUserData> rigidBodyUserData;

		RigidBodyCollision* rigidBodyCollision = nullptr;
	public:

		void* GetBody() override;

		JoltRigidBody() = default;
		~JoltRigidBody() = default;

		virtual void OnCollisionEnter()override;

		virtual void OnCollisionStay()override;

		virtual void OnCollisionExit()override;

		void SetLinearVelocity(const AliceMathF::Vector3& inLinearVelocity_)override;

		virtual AliceMathF::Vector3 GetPosition()override;

		virtual AliceMathF::Quaternion GetRotation()override;

		virtual float GetFriction()override;

		virtual float GetRestitution()override;

		virtual AliceMathF::Vector3 GetLinearVelocity()override;

		virtual AliceMathF::Vector3 GetAngularVelocity()override;

		virtual void SetRigidBodyCollision(RigidBodyCollision* rigidBodyCollision_)override;

		AliceMathF::Matrix4 GetCenterOfMassTransform()override;

		virtual bool IsActive()override;
	};
}

