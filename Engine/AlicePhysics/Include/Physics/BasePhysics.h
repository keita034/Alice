#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

#include<AliceMathF.h>
#include<IRigidBody.h>
#include<IShape.h>
#include<IRigidBodyCreationSettings.h>
#include<Device.h>
#include<CommandList.h>

namespace AlicePhysics
{
	class BasePhysics
	{
	private:

	public:

		virtual void Initialize() = 0;

		virtual void Update(float deltaTime_,float baseDeltaTime_) = 0;

		virtual void SetGravity(const AliceMathF::Vector3& gravity_) = 0;

		virtual void AddRigidBody(IRigidBody* rigidBody_) = 0;

		virtual void RemoveRigidBody(IRigidBody* rigidBody_) = 0;

		virtual void CreateRigidBody(IRigidBody*& rigidBody_, const IRigidBodyCreationSettings* settings_) = 0;

		virtual void Finalize() = 0;

		virtual void EnableCollision(uint32_t groupA_,uint32_t groupB_,bool enable_ = true) = 0;

		virtual void Draw() = 0;

		virtual void SetDevice(IDevice* device_) = 0;
		virtual void SetCommandList(ICommandList* commandList_) = 0;
		virtual void SetViewProjection(AliceMathF::Matrix4* viewMat_,AliceMathF::Matrix4* projectionMat_) = 0;
		virtual void SetLight(AliceMathF::Vector3* lightV_,AliceMathF::Vector4* lightColor_) = 0;

		BasePhysics() = default;
		virtual ~BasePhysics() = default;


	};
}

