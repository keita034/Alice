#include "PlayerAnimation.h"

void PlayerAnimation::Initialize(AliceInput::IInput* input_)
{
	assert(input_);

	input = input_;

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Idle.almb");
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Walk.almb");
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Run.almb");
	rowlingAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Rowling.almb");
	attackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Attack.almb");

	blendTree = std::make_unique<AliceBlendTree>();

	blendTree->AddAnimation(standAnimationHandle);
	blendTree->AddAnimation(walkAnimationHandle);
	blendTree->AddAnimation(runAnimationHandle);

}

void PlayerAnimation::Update()
{

	blendTree->Update(addFrame);
}

void PlayerAnimation::Finalize()
{
}

AliceBlendTree* PlayerAnimation::GetAnimation() const
{
	return blendTree.get();
}

float PlayerAnimation::GetRatio()
{
	return blendTree->GetRatio();
}

void PlayerAnimation::InsertAttackAnimation()
{
	blendTree->InsertAnimation(attackAnimationHandle);
}

void PlayerAnimation::InsertRowlingAnimation()
{
	blendTree->InsertAnimation(rowlingAnimationHandle);
}

bool PlayerAnimation::IsInsert()
{
	return blendTree->IsInsert();
}

InsertAnimationPhase PlayerAnimation::GetInsertAnimationPhase()
{
	return blendTree->GetInsertAnimationPhase();
}

void PlayerAnimation::SetAddFrame(float num_)
{
	addFrame = num_;
}

void PlayerAnimation::WalkAnimationUpdate(float thresh_)
{
	blendTree->SetThresh(thresh_);
}

