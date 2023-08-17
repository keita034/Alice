#pragma once
#include<GameObject.h>

enum class BossHandIndex
{
	LEFT,
	RIGHT
};

class BossHand : public GameObject
{
private:

	AliceModel::AnimationTransform animationTransform;

public:

	BossHand() = default;
	~BossHand() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Transform* parent_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const std::string& boneName_, AliceBlendTree* tree_, AliceModel* playerModel_);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// ワールド行列更新
	/// </summary>
	/// <param name="camera_"></param>
	virtual void TransUpdate(Camera* camera_)override;

	virtual void OnTrigger(uint32_t attribute_)override;

	void SetSituation(uint32_t situation_);

private:

	void Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)override {};
	void Update()override {};
	void Draw()override {};
	void Initialize()override {};

	//コピーコンストラクタ・代入演算子削除
	BossHand& operator=(const BossHand&) = delete;
	BossHand(const BossHand&) = delete;
};

