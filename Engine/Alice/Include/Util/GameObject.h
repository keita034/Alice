#pragma once
#include<Transform.h>
#include<AliceModel.h>
#include<Camera.h>
#include<AliceMathUtility.h>
#include<Sprite2D.h>
#include<IRigidBody.h>
#include <AlicePhysicsSystem.h>
#include<PadingType.h>

class UI
{
public:
	//ワールド変換データ
	Transform transform;
	//画像
	std::unique_ptr<Sprite2D>sprite;
	//画像ハンドル
	uint32_t spriteHandle;

	UI() = default;
	~UI() = default;

private:
	Byte4 PADING;

private:

	// コピーコンストラクタを禁止
	UI(const UI& obj) = delete;
	// コピー代入演算子を禁止
	UI& operator=(const UI& obj) = delete;
};

class GameObject :public AlicePhysics::RigidBodyCollision
{
protected:

	//リジットボディ
	AlicePhysics::IRigidBody* rigidBody;
	//名前
	std::string name;
	//ワールド変換データ
	Transform transform;
	//モデル
	std::unique_ptr<AliceModel>model;
	//形
	std::shared_ptr<AlicePhysics::IShape>shape;
	//モデルハンドル
	uint32_t modelHandle;

private:
	Byte4 PADING;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_) = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_,const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_ = {0.0f,0.0f,0.0f}, const AliceMathF::Vector3& scl_ = { 1.0f,1.0f,1.0 },const Transform* parent_ = nullptr) = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_) = 0;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ワールド行列更新
	/// </summary>
	/// <param name="camera_"></param>
	virtual void TransUpdate(Camera* camera_);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	const Transform* GetTransformPtr()const;

	void SetName(const std::string& objectName_);

	/// <summary>
	/// 当たった瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override = 0;

	/// <summary>
	/// 当たってる時に呼ばれる
	/// </summary>
	virtual void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override = 0;

	/// <summary>
	/// 離れた瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionExit() override = 0;

	virtual ~GameObject() = default;
	GameObject() = default;

private:

	// コピーコンストラクタを禁止
	GameObject(const GameObject& obj) = delete;
	// コピー代入演算子を禁止
	GameObject& operator=(const GameObject& obj) = delete;

};

