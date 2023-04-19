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

void GaussianBlurPostEffect::PostUpdate(RenderTarget* mainRenderTarget)
{
	gaussianXBlurPostEffect->PostUpdate(mainRenderTarget);
	gaussianYBlurPostEffect->PostUpdate(mainRenderTarget);

}

void GaussianBlurPostEffect::UpdateWeightsTable(float blurPower)
{
	float total = 0;
	for (size_t i = 0; i < NUM_WEIGHTS; i++)
	{
		weight[i] = expf(-0.5f * (float)(i * i) / blurPower);
		total += 2.0f * weight[i];

	}
	// 規格化
	for (size_t i = 0; i < NUM_WEIGHTS; i++)
	{
		weight[i] /= total;
	}

	gaussianXBlurPostEffect->SetWeight(weight);
	gaussianYBlurPostEffect->SetWeight(weight);
}

const std::string& GaussianBlurPostEffect::GetType()
{
	return type;
}

GaussianBlurPostEffect* GaussianBlurPostEffect::GetInstance()
{
	static GaussianBlurPostEffect instance;
	return &instance;
}

void GaussianBlurPostEffect::Draw(RenderTarget* mainRenderTarget)
{
	static_cast<void*>(mainRenderTarget);
}

void GaussianBlurPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget)
{
}
