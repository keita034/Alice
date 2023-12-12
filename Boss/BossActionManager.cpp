#include "BossActionManager.h"

void BossActionManager::Initialize(BossAnimation* animation_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,Transform* bossTransform_)
{
	bossTransform = bossTransform_;
	chaseMove = std::make_unique<BossChaseMove>();
	chaseMove->Initialize();
	chaseMove->SetMaxDistance(maxDistance);
	chaseMove->SetSpeed(speed);

	jumpAttackMove = std::make_unique<BossJumpAttackMove>();
	jumpAttackMove->Initialize(physicsSystem_,animation_,particleEmitter->GetShockWaveParticle("BossShockWaveParticle"));
	jumpAttackMove->SetShockWaveRadius(1000);
	jumpAttackMove->SetShockWaveCollisionRadius(10);
	jumpAttackMove->SetShockWaveTime(100);

	bossBeamAttack = std::make_unique<BossBeamAttack>();
	bossBeamAttack->Initialize(physicsSystem_,animation_,particleEmitter->GetLaserParticle("BossLaserParticle"),bossTransform);

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
			do
			{
				//bossAction = ChoiceAction();
				bossAction = BossAction::JUMP_ATTACK;

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
				case BossAction::JUMP_ATTACK:
					bossInternalAction = BossInternalAction::JUMP_ATTACK;
					animation->InserJumpAttackAnimation();
					jumpAttackMove->SetBossPosition(bossPos_);
					direction = -AliceMathF::Vector3(plyerPos_ - bossPos_).Normal();
					break;
				case BossAction::BEAM_ATTACK:
					bossInternalAction = BossInternalAction::BEAM_ATTACK;
					animation->InserBeamAnimation();
					direction = -AliceMathF::Vector3(plyerPos_ - bossPos_).Normal();
					break;

				case BossAction::BOSS_ACTION_NUM:
				default:
					break;
				}
			}while (bossAction == BossAction::BOSS_ACTION_NUM);
		}
	}

	PMoveUpdate();
}

void BossActionManager::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	jumpAttackMove->Finalize(physicsSystem_);
	bossBeamAttack->Finalize(physicsSystem_);
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
	case BossAction::JUMP_ATTACK:
		PJumpAttack();
		break;
	case BossAction::BEAM_ATTACK:
		PBeamAttack();
		break;
	case BossAction::BOSS_ACTION_NUM:
		break;
	default:
		break;
	}
}

void BossActionManager::PChaseAttack()
{
	//追跡
	if (bossInternalAction == BossInternalAction::CHASE)
	{
		if (chaseMove->IsFinish())
		{
			bossInternalAction = BossInternalAction::ATTACK;
			animation->InsertDownAttackAnimation();
			direction = -chaseMove->GetDirection();
			chaseMove->Finalize();
			distanceTraveled = { 0,0,0 };
			animation->SetStandThresh();
			return;
		}

		chaseMove->Update();

		distanceTraveled = chaseMove->GetDistanceTraveled();
	}
	//攻撃
	else if ( bossInternalAction == BossInternalAction::ATTACK )
	{
		if (!animation->IsInsert())
		{
			actionCount = MAX_ACTION_COUNT;
			bossInternalAction = BossInternalAction::NONE;
			bossAction = BossAction::NONE;
		}
	}
}

void BossActionManager::SetParticleEmitter(GPUParticleEmitter* particleEmitter_)
{
	particleEmitter = particleEmitter_;
}

const AliceMathF::Vector3& BossActionManager::GetDirection() const
{
	return direction;
}

void BossActionManager::PJumpAttack()
{
	if ( bossInternalAction == BossInternalAction::JUMP_ATTACK )
	{
		jumpAttackMove->Update();

		if ( jumpAttackMove->IsFinish() )
		{
			jumpAttackMove->End();
			actionCount = MAX_ACTION_COUNT;
			bossInternalAction = BossInternalAction::NONE;
			bossAction = BossAction::NONE;
		}
	}
}

void BossActionManager::PBeamAttack()
{
	if ( bossInternalAction == BossInternalAction::BEAM_ATTACK )
	{
		bossBeamAttack->Update();

		if ( bossBeamAttack->IsFinish() )
		{
			bossBeamAttack->End();
			actionCount = MAX_ACTION_COUNT;
			bossInternalAction = BossInternalAction::NONE;
			bossAction = BossAction::NONE;
		}
	}
}

BossAction BossActionManager::ChoiceAction()
{
	int32_t lRand = AliceMathUtility::GetRand(0,7) ;

	if ( lRand == 0 )
	{
		return BossAction::NONE;
	}
	if ( lRand < 4 )
	{
		return BossAction::CHASE_ATTACK;
	}
	if ( lRand < 6 )
	{
		return BossAction::JUMP_ATTACK;
	}
	if ( lRand == 7 )
	{
		return BossAction::BEAM_ATTACK;
	}

	return BossAction::NONE;
}

#ifdef _DEBUG

BossJumpAttackMove* BossActionManager::GetBossJumpAttackMove()
{
	return jumpAttackMove.get();
}

#endif
BossBeamAttack* BossActionManager::GetBossBeamAttackMove()
{
	return bossBeamAttack.get();
}

const BossInternalAction BossActionManager::GetinternalAction()const
{
	return bossInternalAction;

}
