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

	std::unique_ptr<Material>material2;

public:

	static StripedPostEffect* SGetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void PostUpdate(RenderTarget* mainRenderTarget_) override;

	/// <summary>
	/// タイプを取得
	/// </summary>
	const std::string& GetType()override;

private:

	StripedPostEffect();
	~StripedPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	StripedPostEffect& operator=(const StripedPostEffect&) = delete;
	StripedPostEffect(const StripedPostEffect&) = delete;
};

