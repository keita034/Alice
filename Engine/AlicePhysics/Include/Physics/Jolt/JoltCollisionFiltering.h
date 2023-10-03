#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

ALICE_SUPPRESS_WARNINGS_END

#include<PhysicsRigidBodyType.h>
#include<CollisionFilterConfigTable.h>

namespace AlicePhysics
{
	/// <summary>
	/// 他のブロードフェーズと衝突するかを決定する衝突マスクを返す
	/// </summary>
	uint32_t GetBroadphaseCollisionMask(PhysicsRigidBodyType broadphase_);

	/// <summary>
	/// ObjectLayerを生成(上位8ビットをブロードフェーズ、下位8ビットをコリジョングループで表現する)
	/// </summary>
	/// <param name="broadphase">ブロードフェーズ</param>
	/// <param name="collisionGroup">コリジョングループ</param>
	/// <returns></returns>
	uint16_t ConstructObjectLayer(PhysicsRigidBodyType broadphase_,uint8_t collisionGroup_);

	//オブジェクトレイヤーをブロードフェーズレイヤーにマッピングできるようにするためのインターフェース。
	class JoltObjectToBroadphaseLayer final : public JPH::BroadPhaseLayerInterface
	{
	public:

		JoltObjectToBroadphaseLayer() = default;
		~JoltObjectToBroadphaseLayer() = default;

		//ブロードフェーズ層の数を返す
		virtual uint32_t GetNumBroadPhaseLayers() const override;

		//オブジェクトレイヤーを対応するブロードフェーズレイヤーに変換する
		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		//ブロードフェーズ層のユーザが読める名前を取得（デバッグ目的）
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif

	private:
		JoltObjectToBroadphaseLayer(JoltObjectToBroadphaseLayer&&) = delete;
	};

	//オブジェクトがブロードフェイズのレイヤーと衝突できるかどうかをテストするクラス
	class JoltObjectVsBroadPhaseLayerFilter final : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		JoltObjectVsBroadPhaseLayerFilter() = default;
		~JoltObjectVsBroadPhaseLayerFilter() = default;

		//オブジェクトレイヤーがブロードフェーズレイヤーと衝突する場合に真を返す
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1,JPH::BroadPhaseLayer inLayer2) const override;

	private:
		JoltObjectVsBroadPhaseLayerFilter(JoltObjectVsBroadPhaseLayerFilter&&) = delete;
	};

	class JoltObjectLayerPairFilter final : public JPH::ObjectLayerPairFilter
	{
	private:

		CollisionFilterConfigTable* collisionFilterConfigTable = nullptr;

	public:
		JoltObjectLayerPairFilter() = default;
		~JoltObjectLayerPairFilter() = default;

		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1,JPH::ObjectLayer inLayer2) const override;

		void SetCollisionFilterConfigTable(CollisionFilterConfigTable* collisionFilterConfigTable_);

	private:
		JoltObjectLayerPairFilter(JoltObjectLayerPairFilter&&) = delete;
	};
}

