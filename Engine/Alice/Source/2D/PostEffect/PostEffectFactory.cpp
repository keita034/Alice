#include<PostEffectFactory.h>
#include<LutPostEffect.h>
#include<VignettePostEffect.h>
#include<GaussianBlurPostEffect.h>
#include<StripedPostEffect.h>
#include<OilPaintingPostEffect.h>
#include<ChromaticAberrationPostEffect.h>

PostEffectFactory* PostEffectFactory::SGetInstance()
{
	static PostEffectFactory lInstance;
	return &lInstance;
}

BasePostEffect* PostEffectFactory::CreatePostEffect(const std::string& postEffectName_)
{
	BasePostEffect* lPostEffect = nullptr;

	if (postEffectName_ == "LUT")
	{
		lPostEffect = LutPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "VIGNETTE")
	{
		lPostEffect = VignettePostEffect::SGetInstance();
	}
	else if (postEffectName_ == "GAUSSIANBLUR")
	{
		lPostEffect = GaussianBlurPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "STRIPED")
	{
		lPostEffect = StripedPostEffect::SGetInstance();
	}
	else if(postEffectName_ == "OILPAINTING")
	{
		lPostEffect = OilPaintingPostEffect::SGetInstance();
	}
	else if(postEffectName_ == "CHROMATICABERRATION")
	{
		lPostEffect = ChromaticAberrationPostEffect::SGetInstance();
	}

	return lPostEffect;
}

PostEffectFactory::PostEffectFactory()
{
}
