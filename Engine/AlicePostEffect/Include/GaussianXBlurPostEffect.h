#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<TextureManager.h>
#include<WindowsApp.h>
#include<MaterialManager.h>

class GaussianXBlurPostEffect final :public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer> weightBuff;

	std::array<float, 8> weight;

public:

	void Initialize() override;

	void PostUpdate(RenderTarget* mainRenderTarget_) override;

	const std::string& GetType()override;

	void SetWeight(const std::array<float, 8>& weight_);

	GaussianXBlurPostEffect() = default;
	~GaussianXBlurPostEffect() = default;

private:

	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	GaussianXBlurPostEffect& operator=(const GaussianXBlurPostEffect&) = delete;
	GaussianXBlurPostEffect(const GaussianXBlurPostEffect&) = delete;

};