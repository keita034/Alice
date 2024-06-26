#include "BossActionManager.h"
#include<Player.h>
#include<AliceAssert.h>
#include<Boss.h>

void BossActionManager::Initialize(BossAnimation* animation_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,Transform* bossTransform_)
{
	AliceAssertNull(particleEmitter,"ParticleEmitterIsNull");
	AliceAssertNull(player,"PlayerIsNull");
	AliceAssertNull(bossTransform_,"BossTransformIsNull");
	AliceAssertNull(animation_,"AnimationIsNull");
	AliceAssertNull(physicsSystem_,"PhysicsSystemIsNull");


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

	closeRangeAttack = std::make_unique<BossCloseRangeAttack>();
	closeRangeAttack->Initialize(particleEmitter,bossTransform,physicsSystem_,animation_);
	closeRangeAttack->SetFrameDistance(460);
	closeRangeAttack->SetDistanceFrame(200);
	actionCount = 200;

	animation = animation_;
}

void BossActionManager::Update(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_,bool action_,const std::array< AliceMathF::Matrix4,2>& hands)
{
	float length = 0.0f;
	AliceMathF::Vector3 lPlyerPos = player->GetPosition();
	AliceMathF::Vector3 lBossPos = boss->GetPos();

	if ( bossAction == BossAction::NONE )
	{
		AliceMathF::Vector3 vec = lPlyerPos - lBossPos;
		length = AliceMathF::Abs(vec.Length());

		if ( length < 300 )
		{
			shortDistanceTIme++;

			longDistanceTIme = 0;
		}
		else
		{
			longDistanceTIme++;

			shortDistanceTIme = 0;
		}
	}

	if ( actionCount > 0 )
	{
		actionCount--;

		if ( actionCount == 0 )
		{
			do
			{
				if ( !isReconstruction && !isCloseRangeAttack )
				{
					bossAction = ChoiceAction(length,lBossPos,action_);
				}
				else if ( isReconstruction )
				{
					bossAction = BossAction::RECONSTRUCTION;
				}
				else if ( isCloseRangeAttack )
				{
					bossAction = BossAction::CLOSERANGE_ATTACK;
					isCloseRangeAttack = false;
				}

				switch ( bossAction )
				{
				case BossAction::NONE:
				{
					actionCount = MAX_ACTION_COUNT;
				}
				break;

				case BossAction::CHASE_ATTACK:
				{
					bossInternalAction = BossInternalAction::CHASE;
					chaseMove->SetPlayerPos(lPlyerPos);
					chaseMove->SetBossPos(lBossPos);
					animation->SetWalkThresh();
				}
				break;

				case BossAction::JUMP_ATTACK:
				{
					bossInternalAction = BossInternalAction::JUMP_ATTACK;
					animation->InserJumpAttackAnimation();
					jumpAttackMove->SetBossPosition(lBossPos);
					lPlyerPos = { lPlyerPos.x,0.0f,lPlyerPos.z };
					lBossPos = { lBossPos.x,0.0f,lBossPos.z };
					direction = -AliceMathF::Vector3(lPlyerPos - lBossPos).Normal();
				}
				break;

				case BossAction::BEAM_ATTACK:
				{
					bossInternalAction = BossInternalAction::BEAM_ATTACK;
					animation->InserBeamAnimation();
					lPlyerPos = { lPlyerPos.x,0.0f,lPlyerPos.z };
					lBossPos = { lBossPos.x,0.0f,lBossPos.z };
					direction = -AliceMathF::Vector3(lPlyerPos - lBossPos).Normal();
				}
				break;

				case BossAction::CLOSERANGE_ATTACK:
				{
					bossInternalAction = BossInternalAction::CLOSERANGE_ATTACK;
					closeRangeAttack->SetPlayerPos(lPlyerPos);
					closeRangeAttack->SetBossPos(lBossPos);
					animation->InsertCloseRangeAttackAnimation();
				}
				break;

				case BossAction::RECONSTRUCTION:
				{
					animation->InsertRecoveryAnimation();
				}
				break;

				case BossAction::BLOWAWAY_ATTACK:
				{
					animation->InsertBlowawayAnimation();
					lPlyerPos = { lPlyerPos.x,0.0f,lPlyerPos.z };
					lBossPos = { lBossPos.x,0.0f,lBossPos.z };
					playerForce = AliceMathF::Vector3(lPlyerPos - lBossPos).Normal();

				}
				break;

				case BossAction::BOSS_ACTION_NUM:
				default:
					break;
				}

			} while ( bossAction == BossAction::BOSS_ACTION_NUM );
		}
	}

	PMoveUpdate(boneName_,tree_,bossModel_,hands);
}

void BossActionManager::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	jumpAttackMove->Finalize(physicsSystem_);
	bossBeamAttack->Finalize(physicsSystem_);
	closeRangeAttack->Finalize(physicsSystem_);
}

const AliceMathF::Vector3& BossActionManager::GetDistanceTraveled() const
{
	return distanceTraveled;
}

void BossActionManager::SetBossPos(const AliceMathF::Vector3& pos_)
{
	switch ( bossInternalAction )
	{
	case BossInternalAction::NONE:
		break;
	case BossInternalAction::CHASE:
		chaseMove->SetBossPos(pos_);
		break;
	case BossInternalAction::ATTACK:
		break;
	case BossInternalAction::CLOSERANGE_ATTACK:
		closeRangeAttack->SetBossPos(pos_);
		break;
	case BossInternalAction::BOSS_INTERNAL_ACTION_NUM:
		break;
	default:
		break;
	}
}

void BossActionManager::PMoveUpdate(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_,const std::array< AliceMathF::Matrix4,2>& hands)
{
	switch ( bossAction )
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
	case BossAction::CLOSERANGE_ATTACK:
		PCloseRangeAttack(boneName_,tree_,bossModel_);
		break;
	case BossAction::RECONSTRUCTION:
		PReconstruction(hands);
		break;
	case BossAction::BLOWAWAY_ATTACK:
		PBlowaway();
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
	if ( bossInternalAction == BossInternalAction::CHASE )
	{
		if ( chaseMove->IsFinish() )
		{
			bossInternalAction = BossInternalAction::ATTACK;
			direction = -chaseMove->GetDirection();
			chaseMove->Finalize();
			distanceTraveled = { 0,0,0 };
			animation->SetStandThresh();

			if ( !( boss->GetSituation() & ActorSituation::RIGHT_CUTTING ) )
			{
				animation->InsertDownRightAttackAnimation();
			}
			else if ( !( boss->GetSituation() & ActorSituation::LEFT_CUTTING ) )
			{
				animation->InsertDownLeftAttackAnimation();
			}
			else
			{
				actionCount = 25;
				bossInternalAction = BossInternalAction::NONE;
				bossAction = BossAction::NONE;
			}

			return;
		}

		chaseMove->Update();

		distanceTraveled = chaseMove->GetDistanceTraveled();
	}
	//攻撃
	else if ( bossInternalAction == BossInternalAction::ATTACK )
	{
		if ( !animation->IsInsert() )
		{
			actionCount = 25;
			bossInternalAction = BossInternalAction::NONE;
			bossAction = BossAction::NONE;
		}
	}
}

void BossActionManager::SetParticleEmitter(GPUParticleEmitter* particleEmitter_)
{
	AliceAssertNull(particleEmitter_,"ParticleEmitterIsNull");
	particleEmitter = particleEmitter_;
}

void BossActionManager::SetPlayer(Player* player_)
{
	AliceAssertNull(player_,"PlayerIsNull");
	player = player_;
}

void BossActionManager::SetBoss(Boss* boss_)
{
	AliceAssertNull(boss_,"BossIsNull");
	boss = boss_;
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
			actionCount = 30;
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

void BossActionManager::PBlowaway()
{
	if ( animation->GetRatio() >= 0.2f && animation->GetRatio() <= 0.5f )
	{
		player->SetForce(playerForce * forcepower);

		if ( !blowaway )
		{
			blowaway = true;

			MeshGPUParticle* lParticle = particleEmitter->GetMeshGPUParticle("EffectSphereParticle");
			lParticle->EmitPlay();
			particleEmitter->ScatteringSetSpeed(500.0f);
			particleEmitter->ScatteringSetLifeTime(1.0f);
			particleEmitter->ScatteringSetCenterPos(AliceMathF::GetWorldPosition(boss->GetCenterOfMassTransform()));
			particleEmitter->MeshGPUParticleScattering("EffectSphereParticle");
			lParticle->EmitStop();
		}
	}

	if ( !animation->IsInsert() )
	{
		actionCount = 5;
		blowaway = false;
		isCloseRangeAttack = true;
		bossAction = BossAction::NONE;
	}
}

void BossActionManager::PCloseRangeAttack(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_)
{
	if ( bossInternalAction == BossInternalAction::CLOSERANGE_ATTACK )
	{
		closeRangeAttack->Update();
		closeRangeAttack->SwordUpdate(boneName_,tree_,bossModel_);

		distanceTraveled = closeRangeAttack->GetDistanceTraveled();
		direction = -closeRangeAttack->GetDirection();

		if ( closeRangeAttack->IsFinish() )
		{
			closeRangeAttack->End();
			actionCount = MAX_ACTION_COUNT;
			bossInternalAction = BossInternalAction::NONE;
			bossAction = BossAction::NONE;
		}
	}
}

void BossActionManager::PReconstruction(const std::array< AliceMathF::Matrix4,2>& hands)
{
	if ( animation->GetRatio()>= 0.4f && !reconstruction )
	{
		AnimationModelSuctionGPUParticle* lParticle = particleEmitter->GetAnimationModelSuctionGPUParticle("BossLeftHandScatterinParticle");
		lParticle->DrawStart();

		if ( ( boss->GetSituation() & ActorSituation::LEFT_CUTTING ) && !leftReconstruction )
		{
			leftReconstruction = true;
			AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(hands[ 0 ]);
			lParticle->SetBoneMesh("elemental_element1mesh.003","mixamorig:LeftHand");
			lParticle->SetCenterPos(centerPos);
			lParticle->EmitPlay();
			lParticle->EmitStop();
		}

		if ( ( boss->GetSituation() & ActorSituation::RIGHT_CUTTING ) && !rightReconstruction )
		{
			rightReconstruction = true;
			AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(hands[ 1 ]);

			lParticle->SetBoneMesh("elemental_element1mesh.003","mixamorig:RightHand");
			lParticle->SetCenterPos(centerPos);
			lParticle->EmitPlay();
			lParticle->EmitStop();

		}
	}

	if ( animation->GetRatio() >= 0.6f && !reconstruction )
	{
		reconstruction = true;
		particleEmitter->GetAnimationModelSuctionGPUParticle("BossLeftHandScatterinParticle")->DrawStop();

		if ( ( boss->GetSituation() & ActorSituation::RIGHT_CUTTING ) )
		{
			rightReconstruction = false;
			particleEmitter->GetAnimationMeshGPUParticle("BossRightHandParticle")->EmitPlay();
			particleEmitter->ScatteringSetSpeed(800.0f);
			particleEmitter->ScatteringSetAccel({ 0,4,0 });
			AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(hands[ 1 ]);
			particleEmitter->ScatteringSetCenterPos(centerPos);
			particleEmitter->AnimationMeshGPUParticleScattering("BossRightHandParticle");
			particleEmitter->GetAnimationMeshGPUParticle("BossRightHandParticle")->EmitStop();

			particleEmitter->GetAnimationModelGPUParticle("BossRightHandModelParticle")->EmitPlay();
		}

		if ( ( boss->GetSituation() & ActorSituation::LEFT_CUTTING ) )
		{
			leftReconstruction = false;
			particleEmitter->GetAnimationMeshGPUParticle("BossLeftHandParticle")->EmitPlay();
			particleEmitter->ScatteringSetSpeed(800.0f);
			particleEmitter->ScatteringSetAccel({ 0,4,0 });
			AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(hands[ 0 ]);
			particleEmitter->ScatteringSetCenterPos(centerPos);
			particleEmitter->AnimationMeshGPUParticleScattering("BossLeftHandParticle");
			particleEmitter->GetAnimationMeshGPUParticle("BossLeftHandParticle")->EmitStop();

			particleEmitter->GetAnimationModelGPUParticle("BossLeftHandModelParticle")->EmitPlay();
		}
	}

	if ( !animation->IsInsert() )
	{
		actionCount = MAX_ACTION_COUNT;
		bossAction = BossAction::NONE;
		reconstruction = false;
		isReconstruction = false;
		boss->CuttingReset();
	}
}

BossAction BossActionManager::ChoiceAction(float length_,const AliceMathF::Vector3& bossPos_,bool action_)
{
	if ( action_ )
	{
		if ( shortDistanceTIme >= 200 )
		{
			return BossAction::BLOWAWAY_ATTACK;

			shortDistanceTIme = 0;
		}
		else if ( shortDistanceTIme > 0 )
		{
			if ( !( boss->GetSituation() & ActorSituation::RIGHT_CUTTING ) || !( boss->GetSituation() & ActorSituation::LEFT_CUTTING ) )
			{
				return BossAction::CHASE_ATTACK;
			}
		}
		else if ( longDistanceTIme >= 250 )
		{
			longDistanceTIme = 0;

			return BossAction::BEAM_ATTACK;
		}
		else if ( length_ >= 200 )
		{
			return BossAction::CLOSERANGE_ATTACK;
		}
	}

	return BossAction::NONE;
}

void BossActionManager::StartReconstruction()
{
	isReconstruction = true;

	actionCount = 60;
}

bool BossActionManager::IsReconstruction() const
{
	return isReconstruction;
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

BossCloseRangeAttack* BossActionManager::GetBossCloseRangeAttack()
{
	return closeRangeAttack.get();
}

const BossInternalAction BossActionManager::GetinternalAction()const
{
	return bossInternalAction;

}
