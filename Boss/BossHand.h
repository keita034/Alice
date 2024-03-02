#pragma once
#include<GameObject.h>

#include<GPUParticleEmitter.h>
#include<ReservationFlag.h>

enum class BossHandIndex
{
	LEFT,
	RIGHT
};

struct BossUsData
{
	int32_t situation;
	BossHandIndex index;
};

class Boss;

class BossHand : public GameObject
{
private:
	Boss* boss;
	AnimationModelGPUParticle* modelGPUParticle;
	FireGPUParticle* fireGPUParticle;
	GPUParticleEmitter* gpuParticleEmitter;
	Transform* parent;

	constexpr static int32_t MAX_HP = 1;

	AliceModel::AnimationTransform animationTransform;
	BossUsData bossUsData;
	Transform bossHandTrans;

	int32_t hp = 1;
	int32_t timer = 0;

	ReservationFlag flag;
	ReservationFlag flag2;

	bool death = false;
	bool isPa = false;
	bool cutting = false;
public:

	BossHand() = default;
	~BossHand() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossHandIndex index_,Boss* boss_,GPUParticleEmitter* emitter_);

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
	void SetFireGPUParticle(FireGPUParticle* fireGPUParticle_);
	void ParticleEmit();
	void ParticleStop();

	void Draw()override;

	const AliceMathF::Matrix4 GetCenterOfMassTransform() const;

	void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;
	void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;
	void OnCollisionExit() override;

	bool GetCutting() const;

	void Reset();

private:

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_)override {};
	void Update()override {};
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override {};

	//コピーコンストラクタ・代入演算子削除
	BossHand& operator=(const BossHand&) = delete;
	BossHand(const BossHand&) = delete;
};

