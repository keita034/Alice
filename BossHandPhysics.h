#pragma once
#include<GameObject.h>

class BossHandPhysics final:public GameObject
{
private:
	AlicePhysics::AlicePhysicsSystem* physicsSystem;

public:

	BossHandPhysics() = default;
	~BossHandPhysics() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	void SetSetting(const AlicePhysics::IRigidBodyCreationSettings* setting_);

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()override;

	/// <summary>
	/// 当たった瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;

	/// <summary>
	/// 当たってる時に呼ばれる
	/// </summary>
	virtual void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;

	/// <summary>
	/// 離れた瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionExit() override;

	void SetPosition(const AliceMathF::Vector3& pos_);

	const AliceMathF::Vector3 GetPosition();

private:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& rot_ = { 0.0f,0.0f,0.0f },const AliceMathF::Vector3& scl_ = { 1.0f,1.0f,1.0 },const Transform* parent_ = nullptr)override {};

	//コピーコンストラクタ・代入演算子削除
	BossHandPhysics& operator=(const BossHandPhysics&) = delete;
	BossHandPhysics(const BossHandPhysics&) = delete;

};

