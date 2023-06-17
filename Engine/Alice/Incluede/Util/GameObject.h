#pragma once
#include<Transform.h>
#include<AliceModel.h>
#include<ColliderObject.h>
#include<Camera.h>
#include<AliceMathUtility.h>

class GameObject :public ColliderObject
{
protected:

	std::string name;

	//ワールド変換データ
	Transform transform;
	//モデル
	std::unique_ptr<AliceModel>model;

	//モデルハンドル
	uint32_t modelHandle;
	char PADING[4];

	virtual ~GameObject() = default;
	GameObject() = default;

public:

	/// <summary>
	/// コライダーをセット
	/// </summary>
	void SetCollider(BaseCollider* coll_);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(uint32_t handle_,const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_ = {0.0f,0.0f,0.0f}, const AliceMathF::Vector3& scl_ = { 1.0f,1.0f,1.0 },const Transform* parent_ = nullptr) = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update(Camera* camera_) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision()override;

	const Transform* GetTransformPtr()const;

	void SetName(const std::string& objectName_);

private:

	// コピーコンストラクタを禁止
	GameObject(const GameObject& obj) = delete;
	// コピー代入演算子を禁止
	void operator=(const GameObject& obj) = delete;

};

