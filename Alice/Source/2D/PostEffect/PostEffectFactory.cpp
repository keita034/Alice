#include "PostEffectFactory.h"
#include "LutPostEffect.h"
#include "VignettePostEffect.h"
#include "GaussianBlurPostEffect.h"

PostEffectFactory* PostEffectFactory::GetInstance()
{
	static PostEffectFactory instance;
	return &instance;
}

BasePostEffect* PostEffectFactory::CreatePostEffect(const std::string& postEffectName)
{
	BasePostEffect* postEffect = nullptr;

	if (postEffectName == "LUT")
	{
		postEffect = LutPostEffect::GetInstance();
	}
	else if (postEffectName == "VIGNETTE")
	{
		postEffect = VignettePostEffect::GetInstance();
	}
	else if (postEffectName == "GAUSSIANBLUR")
	{
		postEffect = GaussianBlurPostEffect::GetInstance();
	}
	
	return postEffect;
}
