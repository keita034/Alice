#include "PlayerAnimation.h"

void PlayerAnimation::Initialize(AliceInput::IInput* input_)
{
	assert(input_);

	input = input_;

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Idle.almb");
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Walk.almb");
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Run.almb");

	attackCombo1AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/SwordSlash1.almb");
	attackCombo2AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/SwordSlash2.almb");
	attackCombo3AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/SwordSlash3.almb");

	greatAttackCombo1AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/GreatSwordSlash1.almb");
	greatAttackCombo2AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/GreatSwordSlash2.almb");
	greatAttackCombo3AnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/GreatSwordSlash3.almb");

	rowlingAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Rowling.almb");
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

void PlayerAnimation::InsertAttackCombo1Animation(float frame_)
{
	blendTree->CoercionInsertAnimation(attackCombo1AnimationHandle,true,frame_);

}

void PlayerAnimation::InsertAttackCombo2Animation(float frame_)
{
	blendTree->CoercionInsertAnimation(attackCombo2AnimationHandle,true,frame_);

}

void PlayerAnimation::InsertAttackCombo3Animation(float frame_)
{
	blendTree->CoercionInsertAnimation(attackCombo3AnimationHandle,true,frame_);

}

void PlayerAnimation::InsertGreatAttackCombo1Animation()
{
	blendTree->CoercionInsertAnimation(greatAttackCombo1AnimationHandle);
}

void PlayerAnimation::InsertGreatAttackCombo2Animation()
{
	blendTree->CoercionInsertAnimation(greatAttackCombo2AnimationHandle);
}

void PlayerAnimation::InsertGreatAttackCombo3Animation()
{
	blendTree->CoercionInsertAnimation(greatAttackCombo3AnimationHandle);
}

void PlayerAnimation::InsertRowlingAnimation()
{
	blendTree->CoercionInsertAnimation(rowlingAnimationHandle);
}

void PlayerAnimation::InsertDeathAnimation()
{
	blendTree->CoercionInsertAnimation(deathAnimationHandle, false);
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

