#include "BossActionManager.h"

void BossActionManager::Initialize(BossAnimation* animation_)
{
	chaseMove = std::make_unique<BossChaseMove>();
	chaseMove->Initialize();
	chaseMove->SetMaxDistance(maxDistance);
	chaseMove->SetSpeed(speed);

	actionCount = MAX_ACTION_COUNT;

	animation = animation_;
}

void BossActionManager::Update(const AliceMathF::Vector3& plyerPos_, const AliceMathF::Vector3& bossPos_)
{
	if (actionCount > 0)
	{
		actionCount--;

		if (actionCount == 0)
		{
			//bossAction = static_cast<BossAction>(AliceMathF::GetRand(0.0f, static_cast<float>(BossAction::BOSS_ACTION_NUM) - 1.0f));
			bossAction = BossAction::CHASE_ATTACK;

			switch (bossAction)
			{
			case BossAction::NONE:
				actionCount = MAX_ACTION_COUNT;
				break;
			case BossAction::CHASE_ATTACK:
				bossInternalAction = BossInternalAction::CHASE;
				chaseMove->SetPlayerPos(plyerPos_);
				chaseMove->SetBossPos(bossPos_);
				animation->SetWalkThresh();
				break;
			case BossAction::BOSS_ACTION_NUM:
				break;
			default:
				break;
			}
		}
	}

	PMoveUpdate();
}

void BossActionManager::Finalize()
{
}

const AliceMathF::Vector3& BossActionManager::GetDistanceTraveled() const
{
	return distanceTraveled;
}

void BossActionManager::SetBossPos(const AliceMathF::Vector3& pos_)
{
	switch (bossInternalAction)
	{
	case BossInternalAction::NONE:
		break;
	case BossInternalAction::CHASE:
		chaseMove->SetBossPos(pos_);
		break;
	case BossInternalAction::ATTACK:
		break;
	case BossInternalAction::BOSS_INTERNAL_ACTION_NUM:
		break;
	default:
		break;
	}
}

void BossActionManager::PMoveUpdate()
{
	switch (bossAction)
	{
	case BossAction::NONE:
		break;
	case BossAction::CHASE_ATTACK:
		PChaseAttack();
		break;
	case BossAction::BOSS_ACTION_NUM:
		break;
	default:
		break;
	}
}

void BossActionManager::PChaseAttack()
{
	if (bossInternalAction == BossInternalAction::CHASE)
	{
		if (chaseMove->IsFinish())
		{
			bossInternalAction = BossInternalAction::ATTACK;
			chaseMove->Finalize();
			distanceTraveled = { 0,0,0 };
			animation->SetStandThresh();
			return;
		}
		chaseMove->Update();
		animation->InsertDownAttackAnimation();

		distanceTraveled = chaseMove->GetDistanceTraveled();
	}
	else// ATTACK
	{
		if (!animation->IsInsert())
		{
			actionCount = MAX_ACTION_COUNT;
			bossInternalAction = BossInternalAction::NONE;
			bossAction = BossAction::NONE;
		}
	}
}

const BossInternalAction BossActionManager::GetinternalAction()const
{
	return bossInternalAction;

}
