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

BasePostEffect* PostEffectFactory::CreatePostEffect(const std::string& postEffectName_)
{
	BasePostEffect* lPostEffect = nullptr;

	if (postEffectName_ == "LUT")
	{
		lPostEffect = LutPostEffect::GetInstance();
	}
	else if (postEffectName_ == "VIGNETTE")
	{
		lPostEffect = VignettePostEffect::GetInstance();
	}
	else if (postEffectName_ == "GAUSSIANBLUR")
	{
		lPostEffect = GaussianBlurPostEffect::GetInstance();
	}
	else if (postEffectName_ == "STRIPED")
	{
		lPostEffect = StripedPostEffect::GetInstance();
	}
	else if(postEffectName_ == "OILPAINTING")
	{
		lPostEffect = OilPaintingPostEffect::GetInstance();
	}
	else if(postEffectName_ == "CHROMATICABERRATION")
	{
		lPostEffect = ChromaticAberrationPostEffect::GetInstance();
	}

	return lPostEffect;
}

PostEffectFactory::PostEffectFactory()
{
}
