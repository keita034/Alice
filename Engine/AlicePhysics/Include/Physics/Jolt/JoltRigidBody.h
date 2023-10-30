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

		private:
			int8_t PADING[ 2 ];
		};

	private:

		JPH::Body* body;
		RigidBodyCollision* rigidBodyCollision = nullptr;
		JPH::BodyInterface* bodyInterface = nullptr;

		std::unique_ptr<RigidBodyUserData> rigidBodyUserData;

		BodyData bodyData;

		JPH::BodyID id;
		bool isActive;

		int8_t PADING[ 3 ];
	public:

		JoltRigidBody() = default;
		~JoltRigidBody() = default;

		void OnCollisionEnter(RigidBodyUserData* BodyData_)override;
		void OnCollisionStay(RigidBodyUserData* BodyData_)override;
		void OnCollisionExit()override;
		void SetLinearVelocity(const AliceMathF::Vector3& inLinearVelocity_)override;
		void SetRigidBodyCollision(RigidBodyCollision* rigidBodyCollision_)override;
		void SetPositionAndRotationInternal(const AliceMathF::Vector3& position_,const AliceMathF::Quaternion& rotation_)override;
		void SetPosition(const AliceMathF::Vector3& position_)override;
		void SetRotation(const AliceMathF::Quaternion& rotation_)override;
		void SetMatrix(const AliceMathF::Matrix4& matRigidBody_,const AliceMathF::Matrix4& matWorld_)override;
		void* GetBody() override;
		bool IsActive()override;
		float GetFriction()override;
		float GetRestitution()override;
		AliceMathF::Vector3 GetLinearVelocity()override;
		AliceMathF::Vector3 GetAngularVelocity()override;
		AliceMathF::Vector3 GetPosition()override;
		AliceMathF::Quaternion GetRotation()override;
		AliceMathF::Matrix4 GetCenterOfMassTransform()override;

	private:
		//コピーコンストラクタ・代入演算子削除
		JoltRigidBody& operator=(const JoltRigidBody&) = delete;
		JoltRigidBody(const JoltRigidBody&) = delete;

	};
}

