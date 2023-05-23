#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<DirectX12Core.h>
#include<TextureManager.h>
#include<WindowsApp.h>
#include<DefaultMaterial.h>

class GaussianXBlurPostEffect final :public BasePostEffect
{
private:

	std::unique_ptr<ConstantBuffer> weightBuff;

	std::array<float, 8> weight;

public:

	void Initialize() override;

	void PostUpdate(RenderTarget* mainRenderTarget) override;

	const std::string& GetType()override;

	void SetWeight(std::array<float, 8>& weightPtr);

	GaussianXBlurPostEffect() = default;
	~GaussianXBlurPostEffect() = default;

private:

	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;

	//コピーコンストラクタ・代入演算子削除
	GaussianXBlurPostEffect& operator=(const GaussianXBlurPostEffect&) = delete;
	GaussianXBlurPostEffect(const GaussianXBlurPostEffect&) = delete;

};