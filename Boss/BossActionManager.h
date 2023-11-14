#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)

#include<GPUParticleEmitter.h>

#include<BossAnimation.h>
#include<BossChaseMove.h>
#include<BossJumpAttackMove.h>
#include<BossBeamAttack.h>

enum class BossAction
{
	NONE,
	CHASE_ATTACK,
	JUMP_ATTACK,
	BEAM_ATTACK,
	BOSS_ACTION_NUM
};

enum class BossInternalAction
{
	NONE,
	CHASE,
	ATTACK,
	JUMP_ATTACK,
	BEAM_ATTACK,
	BOSS_INTERNAL_ACTION_NUM
};

class BossActionManager
{
private:
	BossAnimation* animation = nullptr;
	GPUParticleEmitter* particleEmitter = nullptr;
	Transform* bossTransform = nullptr;

	std::unique_ptr<BossChaseMove> chaseMove;
	std::unique_ptr<BossJumpAttackMove>jumpAttackMove;
	std::unique_ptr<BossBeamAttack>bossBeamAttack;

	const int32_t MAX_ACTION_COUNT = 200;
	int32_t actionCount = 0;

	BossAction bossAction;
	BossInternalAction bossInternalAction;

	AliceMathF::Vector3 distanceTraveled;

	float maxDistance = 80.0f;
	float speed = 80.0f;

	float fallingTime;
	float risingTime;

	AliceMathF::Vector3 direction = { 0,0,1 };

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BossAnimation* animation_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,Transform* bossTransform_);

   /// <summary>
   /// 更新処理
   /// </summary>
	void Update(const AliceMathF::Vector3& plyerPos_, const AliceMathF::Vector3& bossPos_);

   /// <summary>
   /// 後始末
   /// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_);

	BossActionManager() = default;
	~BossActionManager() = default;

	const AliceMathF::Vector3& GetDistanceTraveled() const;

	const AliceMathF::Vector3& GetDirection() const;

	void SetBossPos(const AliceMathF::Vector3& pos_);

	const BossInternalAction GetinternalAction()const;

	void SetParticleEmitter(GPUParticleEmitter* particleEmitter_);

	//デバッグ用
#ifdef _DEBUG

	BossJumpAttackMove* GetBossJumpAttackMove();
	BossBeamAttack* GetBossBeamAttackMove();

#endif // _DEBUG



private:

	void PMoveUpdate();

	void PChaseAttack();
	void PJumpAttack();
	void PBeamAttack();
};

