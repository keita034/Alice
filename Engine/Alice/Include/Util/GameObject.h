#pragma once
#include<Transform.h>
#include<AliceModel.h>
#include<Camera.h>
#include<AliceMathUtility.h>
#include<Sprite2D.h>
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
	int32_t PADING;

	// コピーコンストラクタを禁止
	UI(const UI& obj) = delete;
	// コピー代入演算子を禁止
	UI& operator=(const UI& obj) = delete;
};

class GameObject
{
protected:

	std::string name;

	//ワールド変換データ
	Transform transform;
	//モデル
	std::unique_ptr<AliceModel>model;

	//モデルハンドル
	uint32_t modelHandle;

	int32_t debugIndex = -1;
public:

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

	//すり抜ける当たり判定
	virtual void OnTrigger(uint32_t attribute_);

	//すり抜けない当たり判定
	virtual void OnContact(uint32_t attribute_);

	virtual ~GameObject() = default;
	GameObject() = default;

private:

	// コピーコンストラクタを禁止
	GameObject(const GameObject& obj) = delete;
	// コピー代入演算子を禁止
	GameObject& operator=(const GameObject& obj) = delete;

};

