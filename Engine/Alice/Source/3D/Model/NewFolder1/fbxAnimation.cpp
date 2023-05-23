#include"fbxAnimation.h"

float fbxAnimation::GetAnimeMaxflame()
{
	return static_cast<float>(mAnimation->mAnimations[0]->mDuration);
}

float fbxAnimation::GetTickTimes(float frame)
{

	aiAnimation* pAnimation = mAnimation->mAnimations[0];

	float TicksPerSecond = (float)(pAnimation->mTicksPerSecond != 0 ? pAnimation->mTicksPerSecond : 25.0f);

	return  frame * TicksPerSecond;
}
