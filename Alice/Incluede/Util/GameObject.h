#pragma once
#include<Transform.h>
#include<AliceModel.h>
#include<ColliderObject.h>
#include<Camera.h>
#include<AliceMathUtility.h>

class GameObject :public ColliderObject
{
protected:

	//ワールド変換データ
	Transform transform;
	//モデル
	std::unique_ptr<AliceModel>model;
	//モデルハンドル
	uint32_t modelHandle;

	virtual ~GameObject() = default;

public:

	/// <summary>
	/// コライダーをセット
	/// </summary>
	void SetCollider(BaseCollider* coll);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const AliceMathF::Vector3& pos , const AliceMathF::Vector3& rot = {0.0f,0.0f,0.0f}, const AliceMathF::Vector3& scl = { 1.0f,1.0f,1.0 }, Transform* parent = nullptr) = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update(Camera* camera) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision()override;
};

