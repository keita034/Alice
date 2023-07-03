#include "PlayerAnimation.h"

void PlayerAnimation::Initialize()
{
	standAnimationHandle = AliceMotionData::SCreateMotion("");
	standAnimation = std::make_unique<AliceMotionData>();
	standAnimation->SetMotion(standAnimationHandle);

	walkAnimationHandle = AliceMotionData::SCreateMotion("");
	walkAnimation = std::make_unique<AliceMotionData>();
	walkAnimation->SetMotion(walkAnimationHandle);

	runAnimationHandle = AliceMotionData::SCreateMotion("");
	runAnimation = std::make_unique<AliceMotionData>();
	runAnimation->SetMotion(runAnimationHandle);
}

void PlayerAnimation::Update()
{

}

void PlayerAnimation::Finalize()
{
}

AliceMotionData* PlayerAnimation::GetAnimation() const
{
	return returnAnimation;
}

float PlayerAnimation::GetFrame()
{
	return frame;
}
