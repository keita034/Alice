#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>
ALICE_SUPPRESS_WARNINGS_END

#include<BasePhysics.h>
#include<Device.h>
#include<CommandList.h>

namespace AlicePhysics
{
	class AlicePhysicsSystem
	{
	private:

		IDevice* device = nullptr;
		ICommandList* commandList = nullptr;

		//システム
		std::unique_ptr<BasePhysics>physics;

	public:

		AlicePhysicsSystem() = default;
		~AlicePhysicsSystem() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新処理
		/// </summary>
		/// <param name="deltaTime">デルタタイム</param>
		void Update(float deltaTime_,float baseDeltaTime_);

		/// <summary>
		/// 重力を設定
		/// </summary>
		/// <param name="gravity_">重力の方向</param>
		void SetGravity(const AliceMathF::Vector3& gravity_);

		/// <summary>
		/// リジットボディをシステムに追加
		/// </summary>
		/// <param name="rigidBody_">リジットボディ</param>
		void AddRigidBody( IRigidBody* rigidBody_);

		/// <summary>
		/// リジットボディをシステムに削除
		/// </summary>
		/// <param name="rigidBody_">リジットボディ</param>
		void RemoveRigidBody( IRigidBody* rigidBody_);

		/// <summary>
		/// リジットボディを作成
		/// </summary>
		/// <param name="rigidBody_">作成するリジットボディ</param>
		/// <param name="settings_">リジットボディの設定</param>
		void CreateRigidBody(IRigidBody*& rigidBody_, const IRigidBodyCreationSettings* settings_);

		/// <summary>
		/// 終了処理
		/// </summary>
		void Finalize();

		/// <summary>
		///　デバッグ描画
		/// </summary>
		void Draw();

		void SetViewProjection(AliceMathF::Matrix4* viewMat_,AliceMathF::Matrix4* projectionMat_);
		void SetLight(AliceMathF::Vector3* lightV_ ,AliceMathF::Vector4* lightColor_);

		void SetDevice(IDevice* device_);
		void SetCommandList(ICommandList* commandList_);

		//コピーコンストラクタ・代入演算子削除
		AlicePhysicsSystem& operator=(const AlicePhysicsSystem&) = delete;
		AlicePhysicsSystem(const AlicePhysicsSystem&) = delete;
	};
}

