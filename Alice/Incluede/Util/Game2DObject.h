#pragma once
#include<Transform.h>
#include<AliceModel.h>
#include<Sprite2D.h>
#include<Collider2DObject.h>
#include<Camera.h>
#include<AliceMathUtility.h>

class Game2DObject : public Collider2DObject
{
protected:

	//ワールド変換データ
	Transform transform;
	//モデル
	std::unique_ptr<AliceModel>model;
	//モデルハンドル
	uint32_t modelHandle;

	//スプライト
	std::unique_ptr<Sprite2D>sprite;
	uint32_t spriteHandle;

public:

	virtual ~Game2DObject() = default;

	/// <summary>
	/// コライダーをセット
	/// </summary>
	void SetCollider(Base2DCollider* coll);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const Point2D& pos, const AliceMathF::Vector3& rot = { 0.0f,0.0f,0.0f }, const AliceMathF::Vector3& scl = { 1.0f,1.0f,1.0 }, Transform* parent = nullptr) = 0;

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

	virtual void CollisionUpdate()override;
};

