#include<PostEffectFactory.h>
#include<LutPostEffect.h>
#include<VignettePostEffect.h>
#include<GaussianBlurPostEffect.h>
#include<StripedPostEffect.h>
#include<OilPaintingPostEffect.h>
#include<ChromaticAberrationPostEffect.h>
#include<BloomPostEffect.h>
#include<SandStormPostEffect.h>
#include<CRTPostEffect.h>
#include<MonochromePostEffect.h>
#include<FrostedGlassPostEffect.h>
#include<MosaicPostEffect.h>
#include<DistortionPostEffect.h>

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
	else if (postEffectName_ == "BLOOM")
	{
		lPostEffect = BloomPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "SANDSTORM")
	{
		lPostEffect = SandStormPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "CRT")
	{
		lPostEffect = CRTPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "MONOCHROME")
	{
		lPostEffect = MonochromePostEffect::SGetInstance();
	}
	else if (postEffectName_ == "FROSTEDGLASS")
	{
		lPostEffect = FrostedGlassPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "MOSAIC")
	{
		lPostEffect = MosaicPostEffect::SGetInstance();
	}
	else if (postEffectName_ == "DISTORTION")
	{
		lPostEffect = DistortionPostEffect::SGetInstance();
	}

	return lPostEffect;
}

PostEffectFactory::PostEffectFactory()
{
}
