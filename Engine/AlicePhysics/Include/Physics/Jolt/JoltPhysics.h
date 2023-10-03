#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include<memory>
#include <thread>

ALICE_SUPPRESS_WARNINGS_END

#include <BasePhysics.h>
#include<JoltGroupFilter.h>
#include<JoltCollisionFiltering.h>
#include<JoltContactListener.h>
#include<IShape.h>
#include<RigidBodyManager.h>

#include<JoltDebugRenderer.h>

namespace AlicePhysics
{

	class JoltBodyActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void OnBodyActivated(const JPH::BodyID& inBodyID,uint64_t inBodyUserData) override;

		virtual void OnBodyDeactivated(const JPH::BodyID& inBodyID,uint64_t inBodyUserData) override;
	};

	class JoltPhysics final : public BasePhysics
	{
	private:

		IDevice* device = nullptr;
		ICommandList* commandList = nullptr;

		//物理システムのメインクラス
		std::unique_ptr<JPH::PhysicsSystem>physicsSystem;

		//物理演算の更新中に一時的にアロケートするためのテンポラリ・アロケーター(変更予定)
		std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;

		// 物理ジョブを複数のスレッドで実行するジョブ・システム(変更予定)
		std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;

		JoltObjectToBroadphaseLayer objectToBroadphaseLayer;

		JoltObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter;

		CollisionFilterConfigTable collisionFilterConfigTable;

		JoltObjectLayerPairFilter joltObjectLayerPairFilter;

		JoltBodyActivationListener bodyActivationListener;

		JoltContactListener contactListener;

		//グループフィルター
		JoltGroupFilter* groupFilter;

		//物理システムに追加できる剛体の最大数
		const uint32_t MAX_BODIES = 10000;

		//接触制約バッファの最大サイズ
		const uint32_t MAX_CONTACT_CONSTRAINTS = MAX_BODIES * 4;

		//剛体を同時アクセスから保護するために割り当てるミューテックスの数
		const uint32_t NUM_BODY_MUTEXES = 0;

		//ボディペアの最大量
		const uint32_t MAX_BODY_PAIRS = MAX_CONTACT_CONSTRAINTS * 10;

		//重力
		AliceMathF::Vector3 gravity = { 0.0f, -9.8f, 0.0f };

		std::unique_ptr<RigidBodyManager> rigidBodyManager;
#ifdef _DEBUG
		std::unique_ptr<JoltDebugRenderer>debugRenderer;

#endif // _DEBUG

	public:

		JoltPhysics() = default;
		~JoltPhysics() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize()override;

		void Update(float deltaTime_,float baseDeltaTime_)override;
		void SetGravity(const AliceMathF::Vector3& gravity_)override;

		void AddRigidBody(IRigidBody* rigidBody_)override;
		void RemoveRigidBody(IRigidBody* rigidBody_)override;

		virtual void CreateRigidBody(IRigidBody*& rigidBody_,const IRigidBodyCreationSettings* settings_)override;

		void EnableCollision(uint32_t groupA_,uint32_t groupB_,bool enable_)override;

		void Finalize()override;

		void Draw()override;

		void SetDevice(IDevice* device_) override;
		void SetCommandList(ICommandList* commandList_) override;
		void SetViewProjection(AliceMathF::Matrix4* viewMat_,AliceMathF::Matrix4* projectionMat_)override;
		void SetLight(AliceMathF::Vector3* lightV_,AliceMathF::Vector4* lightColor_)override;

	private:

		void PGetShape(JPH::Shape*& joltShape_,IShape* shape_);
	};
}

