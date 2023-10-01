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

#include<BossAnimation.h>
#include<BossChaseMove.h>

enum class BossAction
{
	NONE,
	CHASE_ATTACK,

	BOSS_ACTION_NUM
};

enum class BossInternalAction
{
	NONE,
	CHASE,
	ATTACK,
	BOSS_INTERNAL_ACTION_NUM
};

class BossActionManager
{
private:
	BossAnimation* animation = nullptr;

	std::unique_ptr<BossChaseMove> chaseMove;

	const int32_t MAX_ACTION_COUNT = 200;
	int32_t actionCount = 0;

	BossAction bossAction;
	BossInternalAction bossInternalAction;

	AliceMathF::Vector3 distanceTraveled;

	float maxDistance = 55.0f;
	float speed = 80.0f;



public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BossAnimation* animation_);

   /// <summary>
   /// 更新処理
   /// </summary>
	void Update(const AliceMathF::Vector3& plyerPos_, const AliceMathF::Vector3& bossPos_);

   /// <summary>
   /// 後始末
   /// </summary>
	void Finalize();

	BossActionManager() = default;
	~BossActionManager() = default;

	const AliceMathF::Vector3& GetDistanceTraveled() const;

	void SetBossPos(const AliceMathF::Vector3& pos_);

	const BossInternalAction GetinternalAction()const;

private:

	void PMoveUpdate();

	void PChaseAttack();
};

