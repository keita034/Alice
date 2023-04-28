#include<PostEffectFactory.h>
#include<LutPostEffect.h>
#include<VignettePostEffect.h>
#include<GaussianBlurPostEffect.h>
#include<StripedPostEffect.h>
#include<OilPaintingPostEffect.h>
#include<ChromaticAberrationPostEffect.h>

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
	else if (postEffectName == "STRIPED")
	{
		postEffect = StripedPostEffect::GetInstance();
	}
	else if(postEffectName == "OILPAINTING")
	{
		postEffect = OilPaintingPostEffect::GetInstance();
	}
	else if(postEffectName == "CHROMATICABERRATION")
	{
		postEffect = ChromaticAberrationPostEffect::GetInstance();
	}

	return postEffect;
}

PostEffectFactory::PostEffectFactory()
{
}
