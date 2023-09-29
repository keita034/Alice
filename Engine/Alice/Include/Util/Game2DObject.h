#pragma once
#include<Transform.h>
#include<AliceModel.h>
#include<Sprite2D.h>
#include<Camera.h>
#include<AliceMathUtility.h>

class Game2DObject
{
protected:

	//モデル
	std::unique_ptr<AliceModel>model;

	//スプライト
	std::unique_ptr<Sprite2D>sprite;

	//ワールド変換データ
	Transform transform;

	//モデルハンドル
	uint32_t modelHandle;

	uint32_t spriteHandle;

public:

	virtual ~Game2DObject() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const Point2D& pos_, const AliceMathF::Vector3& rot_ = { 0.0f,0.0f,0.0f }, const AliceMathF::Vector3& scl_ = { 1.0f,1.0f,1.0 }, Transform* parent_ = nullptr) = 0;

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
	virtual void OnCollision();

	virtual void CollisionUpdate();

private:

	// コピーコンストラクタを禁止
	Game2DObject(const Game2DObject& obj) = delete;
	// コピー代入演算子を禁止
	Game2DObject& operator=(const Game2DObject& obj) = delete;
};

