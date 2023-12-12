#pragma once
#include<AliceMathF.h>
#include<BossAnimation.h>
#include<ShockWaveGPUParticle.h>
#include<GameObject.h>

struct BossShockWaveUsData
{
	float radius = 0.0f;
	AliceMathF::Vector3 pos;
	bool isFinish;
};

class BossJumpAttackMove : public GameObject
{
private:

	BossAnimation* bossAnimation = nullptr;
	ShockWaveGPUParticle* shockWaveGPUParticle = nullptr;
	AlicePhysics::AlicePhysicsSystem* physicsSystem = nullptr;

	AliceMathF::Vector3 bossPosition;

	AliceMathF::Vector3 distance;

	float shockWaveRadius;
	float shockWaveMaxRadius;

	float time;

	float shockWaveTime;
	float shockMaxWaveTime;

	BossShockWaveUsData usData;

	int32_t particleIndex;

	bool isFinish;
	bool animationFinish;
	bool shockWaveFinish;

public:

	BossJumpAttackMove() = default;
	~BossJumpAttackMove() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_,ShockWaveGPUParticle* shockWaveGPUParticle_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	void TransUpdate(Camera* camera_)override;

	const AliceMathF::Vector3& GetBossPosition()const;

	void SetBossPosition(const AliceMathF::Vector3& position_);
	void SetShockWaveTime(float time_);
	void SetShockWaveRadius(float radius_);
	void SetShockWaveCollisionRadius(float radius_);
	void ShockWave();
	bool ShockWaveUpdate();

	bool IsFinish();

	void End();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;
	void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;
	void OnCollisionExit() override;
	void Draw()override;

private:

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& rot_,const AliceMathF::Vector3& scl_,const Transform* parent_)override {};
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override {};
};

