#include "BossAnimation.h"

void BossAnimation::Initialize()
{

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Idle.almb");
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Walk.almb");
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Run.almb");

	downAttackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Boss/Motion/DownAttack.almb");

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
