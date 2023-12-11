#include "BossAnimation.h"

void BossAnimation::Initialize()
{

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Idle.almb");//アイドル時
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Walking.almb");//歩き
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Run.almb");//走り

	downAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Punch.almb");//パンチ
	deathAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Dying.almb");//死んだ時
	jumpAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/JumpAttack.almb");//ジャンプ攻撃
	beamAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/Beam.almb");//ビーム

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

void BossAnimation::InsertDownAttackAnimation()
{
	blendTree->InsertAnimation(downAttackAnimationHandle);
}

void BossAnimation::InsertDeathAnimation()
{
	blendTree->CoercionInsertAnimation(deathAnimationHandle,false);
}

void BossAnimation::InserJumpAttackAnimation()
{
	blendTree->InsertAnimation(jumpAttackAnimationHandle);
}

void BossAnimation::InserBeamAnimation()
{
	blendTree->InsertAnimation(beamAnimationHandle);
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
