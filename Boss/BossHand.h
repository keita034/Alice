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
	uint32_t situation;
public:

	BossHand() = default;
	~BossHand() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const std::string& boneName_, AliceBlendTree* tree_, AliceModel* playerModel_);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	/// <summary>
	/// ワールド行列更新
	/// </summary>
	/// <param name="camera_"></param>
	virtual void TransUpdate(Camera* camera_)override;

	void SetSituation(uint32_t situation_);

	void Draw()override;

	void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_)override;
	void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_)override;
	void OnCollisionExit() override;

private:

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)override {};
	void Update()override {};
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override {};

	//コピーコンストラクタ・代入演算子削除
	BossHand& operator=(const BossHand&) = delete;
	BossHand(const BossHand&) = delete;
};

