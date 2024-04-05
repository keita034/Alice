#include "BossAnimation.h"

void BossAnimation::Initialize()
{

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Idle.almb");//アイドル時
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Walking.almb");//歩き
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Run.almb");//走り

	downRightAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/PunchR.almb");//パンチ
	downLeftAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/PunchL.almb");//パンチ
	deathAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Dying.almb");//死んだ時
	jumpAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/JumpAttack.almb");//ジャンプ攻撃
	beamAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Beam.almb");//ビーム
	closeRangeAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/CloseRangeAttack.almb");//近づく攻撃
	recoveryAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Recovery.almb");//回復
	blowawayAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Blowaway.almb");//回復

	blendTree = std::make_unique<AliceBlendTree>();
	blendTree->AddAnimation(standAnimationHandle);
	blendTree->AddAnimation(walkAnimationHandle);
	blendTree->AddAnimation(runAnimationHandle);
}

void BossAnimation::Update()
{
	blendTree->Update(addFrame);
}

void BossAnimation::Finalize()
{
}

AliceBlendTree* BossAnimation::GetAnimation() const
{
	return blendTree.get();
}

float BossAnimation::GetRatio()
{
	return blendTree->GetRatio();
}

void BossAnimation::InsertDownRightAttackAnimation()
{
	blendTree->CoercionInsertAnimation(downRightAttackAnimationHandle);
}

void BossAnimation::InsertDownLeftAttackAnimation()
{
	blendTree->CoercionInsertAnimation(downLeftAttackAnimationHandle);
}

void BossAnimation::InsertDeathAnimation()
{
	blendTree->CoercionInsertAnimation(deathAnimationHandle,false);
}

void BossAnimation::InserJumpAttackAnimation()
{
	blendTree->CoercionInsertAnimation(jumpAttackAnimationHandle);
}

void BossAnimation::InserBeamAnimation()
{
	blendTree->CoercionInsertAnimation(beamAnimationHandle);
}

void BossAnimation::InsertCloseRangeAttackAnimation()
{
	blendTree->CoercionInsertAnimation(closeRangeAttackAnimationHandle);
}

void BossAnimation::InsertRecoveryAnimation()
{
	blendTree->CoercionInsertAnimation(recoveryAnimationHandle);
}

void BossAnimation::InsertBlowawayAnimation()
{
	blendTree->CoercionInsertAnimation(blowawayAnimationHandle);
}

bool BossAnimation::IsInsert()
{
	return blendTree->IsInsert();
}

InsertAnimationPhase BossAnimation::GetInsertAnimationPhase()
{
	return blendTree->GetInsertAnimationPhase();
}

void BossAnimation::SetAddFrame(float num_)
{
	addFrame = num_;
}

void BossAnimation::WalkAnimationUpdate(float thresh_)
{
	blendTree->SetThresh(thresh_);
}

void BossAnimation::SetStandThresh()
{
	blendTree->SetThresh(standThresh);
}

void BossAnimation::SetWalkThresh()
{
	blendTree->SetThresh(walkThresh);
}

void BossAnimation::SetRunThresh()
{
	blendTree->SetThresh(runThresh);
}

void BossAnimation::AnimationStop()
{
	blendTree->Stop();
}

void BossAnimation::AnimationEndStop()
{
	blendTree->AnimationEndStop();
}
