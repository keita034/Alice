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
#include<BossCloseRangeAttack.h>

enum class BossAction
{
	NONE,
	CHASE_ATTACK,
	JUMP_ATTACK,
	BEAM_ATTACK,
	CLOSERANGE_ATTACK,
	RECONSTRUCTION,
	BLOWAWAY_ATTACK,
	BOSS_ACTION_NUM
};

enum class BossInternalAction
{
	NONE,
	CHASE,
	ATTACK,
	JUMP_ATTACK,
	BEAM_ATTACK,
	CLOSERANGE_ATTACK,
	BOSS_INTERNAL_ACTION_NUM
};

class Player;
class Boss;

class BossActionManager
{
private:
	BossAnimation* animation = nullptr;
	GPUParticleEmitter* particleEmitter = nullptr;
	Transform* bossTransform = nullptr;
	Player* player = nullptr;
	Boss* boss = nullptr;

	std::unique_ptr<BossChaseMove> chaseMove;
	std::unique_ptr<BossJumpAttackMove>jumpAttackMove;
	std::unique_ptr<BossBeamAttack>bossBeamAttack;
	std::unique_ptr<BossCloseRangeAttack>closeRangeAttack;

	BossAction bossAction;
	BossInternalAction bossInternalAction;

	AliceMathF::Vector3 distanceTraveled;
	AliceMathF::Vector3 direction = { 0,0,1 };
	AliceMathF::Vector3 playerForce;

	float maxDistance = 80.0f;
	float speed = 80.0f;
	float fallingTime;
	float risingTime;
	float forcepower = 65.0f;
	
	int32_t MAX_ACTION_COUNT = 90;
	int32_t actionCount = 0;
	int32_t shortDistanceTIme = 0;
	int32_t longDistanceTIme = 0;

	bool reconstruction;
	bool isReconstruction;
	bool isCloseRangeAttack;
	bool blowaway;
	bool right =  true;
	bool leftReconstruction = false;
	bool rightReconstruction = false;
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BossAnimation* animation_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,Transform* bossTransform_);

   /// <summary>
   /// 更新処理
   /// </summary>
	void Update(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_,bool action_,const std::array< AliceMathF::Matrix4,2>& hands);

   /// <summary>
   /// 後始末
   /// </summary>
	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_);

	BossActionManager() = default;
	~BossActionManager() = default;

	const AliceMathF::Vector3& GetDistanceTraveled() const;
	const AliceMathF::Vector3& GetDirection() const;
	const BossInternalAction GetinternalAction()const;

	void SetBossPos(const AliceMathF::Vector3& pos_);
	void SetParticleEmitter(GPUParticleEmitter* particleEmitter_);
	void SetPlayer(Player* player_);
	void SetBoss(Boss* boss_);

	BossAction ChoiceAction(float length_,const AliceMathF::Vector3& bossPos_, bool action_ = true);

	void StartReconstruction();

	bool IsReconstruction() const;

	//デバッグ用
#ifdef _DEBUG

	BossJumpAttackMove* GetBossJumpAttackMove();

#endif // _DEBUG

	BossBeamAttack* GetBossBeamAttackMove();
	BossCloseRangeAttack* GetBossCloseRangeAttack();


private:

	void PMoveUpdate(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_,const std::array< AliceMathF::Matrix4,2>& hands);

	void PChaseAttack();
	void PJumpAttack();
	void PBeamAttack();
	void PBlowaway();
	void PCloseRangeAttack(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_);
	void PReconstruction(const std::array< AliceMathF::Matrix4,2>& hands);
};

