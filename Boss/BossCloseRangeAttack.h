#pragma once
#include<AliceMathF.h>
#include<AlicePhysicsSystem.h>
#include<BossAnimation.h>
#include<BossSword.h>
#include<GPUParticleEmitter.h>

class BossCloseRangeAttack
{
public:

	enum class Phase
	{
		NONE,
		APPROACH,
		ATTACK
	};


private:

	AlicePhysics::AlicePhysicsSystem* physicsSystem;
	BossAnimation* bossAnimation;
	MeshGPUParticle* meshParticle;

	std::unique_ptr<BossSword>bossSword;

	AliceMathF::Vector3 playerPos;
	float distanceFrame = 0.0f;

	AliceMathF::Vector3 BossPos;
	float frameDistance = 0.0f;

	AliceMathF::Vector3 oldBossPos;
	float frame = 0.0f;

	AliceMathF::Vector3 direction;
	float length = 0.0f;

	AliceMathF::Vector3 distanceTraveled;
	float maxFrame = 0.0f;

	AliceMathF::Vector3 retBossPos;
	Phase phase;

	int32_t particleIndex;


	bool isFinish = false;

	int32_t time = 0;

public:

	BossCloseRangeAttack() = default;
	~BossCloseRangeAttack() = default;

   /// <summary>
   /// 初期化
   /// </summary>
	void Initialize(GPUParticleEmitter* particleEmitter_,Transform* bossTrans_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_);

   /// <summary>
   /// 更新処理
   /// </summary>
	void Update();

   /// <summary>
   /// 後始末
   /// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_);

	void End();

	void SetPlayerPos(const AliceMathF::Vector3& pos_);
	void SetBossPos(const AliceMathF::Vector3& pos_);

	void SetFrameDistance(float frameDistance_);
	void SetDistanceFrame(float distanceFrame_);

	void TransUpdate(Camera* camera_);

	bool IsFinish();

	void Draw();

	void SwordUpdate(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_);

	const AliceMathF::Vector3& GetDistanceTraveled()const;

	const AliceMathF::Vector3& GetDirection()const;
};

