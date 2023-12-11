#include "PlayerAnimation.h"

void PlayerAnimation::Initialize(AliceInput::IInput* input_)
{
	assert(input_);

	input = input_;

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Idle.almb");
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Walk.almb");
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Run.almb");

	attackCombo1AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/AttackCombo1.almb");;
	attackCombo2AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/AttackCombo2.almb");;
	attackCombo3AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/AttackCombo3.almb");;

	rowlingAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Rowling.almb");
	attackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Attack.almb");
	deathAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Death.almb");
	hitAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/HitReaction.almb");
	healingAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Healing.almb");

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

void PlayerAnimation::InsertAttackCombo1Animation()
{
	blendTree->InsertAnimation(attackCombo1AnimationHandle);

}

void PlayerAnimation::InsertAttackCombo2Animation()
{
	blendTree->InsertAnimation(attackCombo2AnimationHandle);

}

void PlayerAnimation::InsertAttackCombo3Animation()
{
	blendTree->InsertAnimation(attackCombo3AnimationHandle);

}

void PlayerAnimation::InsertRowlingAnimation()
{
	blendTree->InsertAnimation(rowlingAnimationHandle);
}

void PlayerAnimation::InsertDeathAnimation()
{
	blendTree->InsertAnimation(deathAnimationHandle, false);
}

void PlayerAnimation::InsertHitAnimation()
{
	blendTree->InsertAnimation(hitAnimationHandle);
}

void PlayerAnimation::InsertHealingAnimation()
{
	blendTree->InsertAnimation(healingAnimationHandle);
}

bool PlayerAnimation::IsInsert()
{
	return blendTree->IsInsert();
}

void PlayerAnimation::AnimationStop()
{
	blendTree->Stop();
}

void PlayerAnimation::AnimationEndStop()
{
	blendTree->AnimationEndStop();
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

