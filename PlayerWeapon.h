#pragma once
#include<GameObject.h>

class PlayerWeapon : public GameObject
{
private:

	AliceMathF::Matrix4 boneMat;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(Transform* parent_);

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize()override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update(const std::string& boneName_,AliceBlendTree* tree_, AliceModel* playerModel_);

	/// <summary>
	/// ワールド行列更新
	/// </summary>
	/// <param name="camera_"></param>
	virtual void TransUpdate(Camera* camera_)override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()override;

	PlayerWeapon() = default;
	~PlayerWeapon() = default;

private:

	virtual void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_ = { 0.0f,0.0f,0.0f }, const AliceMathF::Vector3& scl_ = { 1.0f,1.0f,1.0 }, const Transform* parent_ = nullptr)override;
	virtual void Initialize()override;
	virtual void Update()override;
};

