﻿#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include "DirectX12Core.h"
#include "TextureManager.h"
#include "WindowsApp.h"
#include"DefaultMaterial.h"

class GaussianYBlurPostEffect final :public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer> weightBuff;

	std::array<float, 8> weight;

public:

	void Initialize() override;

	void PostUpdate(RenderTarget* mainRenderTarget_) override;

	const std::string& GetType()override;

	void SetWeight(const std::array<float, 8>& weightPtr_);

	GaussianYBlurPostEffect() = default;
	~GaussianYBlurPostEffect() = default;

private:

	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	GaussianYBlurPostEffect& operator=(const GaussianYBlurPostEffect&) = delete;
	GaussianYBlurPostEffect(const GaussianYBlurPostEffect&) = delete;
};
