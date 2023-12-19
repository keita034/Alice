#pragma once
#include<AliceMathF.h>
#include<AlicePhysicsSystem.h>
#include<BossAnimation.h>
#include<BossSword.h>

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

	std::unique_ptr<BossSword>bossSword;

	AliceMathF::Vector3 playerPos;

	AliceMathF::Vector3 BossPos;
	AliceMathF::Vector3 oldBossPos;

	AliceMathF::Vector3 direction;
	AliceMathF::Vector3 distanceTraveled;

	float distanceFrame = 0.0f;

	float frameDistance = 0.0f;

	bool isFinish = false;

	float maxFrame = 0.0f;

	float frame = 0.0f;
	float length = 0.0f;
	Phase phase;

	AliceMathF::Vector3 retBossPos;

public:

	BossCloseRangeAttack() = default;
	~BossCloseRangeAttack() = default;

   /// <summary>
   /// 初期化
   /// </summary>
	void Initialize(Transform* bossTrans_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_);

   /// <summary>
   /// 更新処理
   /// </summary>
	void Update();

   /// <summary>
   /// 後始末
   /// </summary>
	void Finalize();

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

