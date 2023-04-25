﻿#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>
#include<GaussianYBlurPostEffect.h>
#include<GaussianXBlurPostEffect.h>
#include<DirectX12Core.h>
#include<WindowsApp.h>

class StripedPostEffect : public BasePostEffect
{
private:

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void PostUpdate(RenderTarget* mainRenderTarget) override;

	/// <summary>
	/// タイプを取得
	/// </summary>
	const std::string& GetType()override;

	StripedPostEffect() = default;
	~StripedPostEffect() = default;

private:

	//描画
	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;

	//コピーコンストラクタ・代入演算子削除
	StripedPostEffect& operator=(const StripedPostEffect&) = delete;
	StripedPostEffect(const StripedPostEffect&) = delete;
};

