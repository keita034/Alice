#include<GaussianBlurPostEffect.h>

void GaussianBlurPostEffect::Initialize()
{
	if (needsInit)
	{
		needsInit = false;

		type = "GAUSSIANBLUR";

		gaussianXBlurPostEffect = std::make_unique<GaussianXBlurPostEffect>();
		gaussianXBlurPostEffect->Initialize();

		gaussianYBlurPostEffect = std::make_unique<GaussianYBlurPostEffect>();
		gaussianYBlurPostEffect->Initialize();

		UpdateWeightsTable(10.0f);
	}
}

void GaussianBlurPostEffect::PostUpdate(RenderTarget* mainRenderTarget_)
{
	gaussianXBlurPostEffect->PostUpdate(mainRenderTarget_);
	gaussianYBlurPostEffect->PostUpdate(mainRenderTarget_);

}

void GaussianBlurPostEffect::UpdateWeightsTable(float blurPower_)
{
	float lTotal = 0;
	for (size_t i = 0; i < NUM_WEIGHTS; i++)
	{
		weight[i] = expf(-0.5f * static_cast<float>(i * i) / blurPower_);
		lTotal += 2.0f * weight[i];

	}
	// 規格化
	for (size_t i = 0; i < NUM_WEIGHTS; i++)
	{
		weight[i] /= lTotal;
	}

	gaussianXBlurPostEffect->SetWeight(weight);
	gaussianYBlurPostEffect->SetWeight(weight);
}

const std::string& GaussianBlurPostEffect::GetType()
{
	return type;
}

GaussianBlurPostEffect* GaussianBlurPostEffect::SGetInstance()
{
	static GaussianBlurPostEffect lInstance;
	return &lInstance;
}

void GaussianBlurPostEffect::Draw(RenderTarget* mainRenderTarget_)
{
	static_cast<void*>(mainRenderTarget_);
}

void GaussianBlurPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget_)
{
	static_cast<void*>(mainRenderTarget_);
}

GaussianBlurPostEffect::GaussianBlurPostEffect()
{
}
