#include "PlayerAnimation.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

void PlayerAnimation::Initialize(AliceInput::IInput* input_)
{
	assert(input_);

	input = input_;

	standAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Idle.almb");
	walkAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Walk.almb");
	runAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Run.almb");
	rowlingAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Rowling.almb");


	blendTree = std::make_unique<AliceBlendTree>();

	blendTree->AddAnimation(standAnimationHandle);
	blendTree->AddAnimation(walkAnimationHandle);
	blendTree->AddAnimation(runAnimationHandle);


	attackAnimationHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Attack.almb");
}

void PlayerAnimation::Update()
{

	ImGui::Begin("blend");

	ImGui::SliderFloat("thresh", &walkAnimationThresh, 0.0f, 1.0f);

	ImGui::End();

	PWalkAnimationUpdate();

	PAttackAnimationUpdate();

	blendTree->Update(0.02f);
}

void PlayerAnimation::Finalize()
{
}

AliceBlendTree* PlayerAnimation::GetAnimation() const
{
	return blendTree.get();
}

float PlayerAnimation::GetFrame()
{
	return frame;
}

void PlayerAnimation::PWalkAnimationUpdate()
{
	AliceMathF::Vector2 lLeftStickPower = input->GetLeftStickVec();

	float lStickPower = AliceMathUtility::Max<float>(AliceMathF::Abs(lLeftStickPower.x), AliceMathF::Abs(lLeftStickPower.y));

	//if (input->InputButton(ControllerButton::A))
	//{
	//	walkAnimationThresh = lStickPower;
	//}
	//else
	//{
	//	walkAnimationThresh = 0.5f * lStickPower;
	//}

	blendTree->SetThresh(walkAnimationThresh);
}

void PlayerAnimation::PAttackAnimationUpdate()
{
	if (input->TriggerButton(ControllerButton::RB))
	{
		blendTree->InsertAnimation(attackAnimationHandle);
	}

	if (input->TriggerButton(ControllerButton::A))
	{
		blendTree->InsertAnimation(rowlingAnimationHandle);
	}
}
