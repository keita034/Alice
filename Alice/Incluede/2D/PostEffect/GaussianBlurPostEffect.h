#pragma once
#include"BasePostEffect.h"
#include"ConstantBuffer.h"
#include"AliceMathUtility.h"
#include "GaussianYBlurPostEffect.h"
#include "GaussianXBlurPostEffect.h"

class GaussianBlurPostEffect final: public BasePostEffect
{
private:

	const size_t NUM_WEIGHTS = 8;

	std::array<float, 8> weight{};

	std::unique_ptr<GaussianXBlurPostEffect>gaussianXBlurPostEffect;
	std::unique_ptr<GaussianYBlurPostEffect>gaussianYBlurPostEffect;

public:

	static GaussianBlurPostEffect* GetInstance();

	//初期化
	void Initialize() override;

	//更新
	void PostUpdate(RenderTarget* mainRenderTarget) override;

	/// <summary>
	/// タイプを取得
	/// </summary>
	const std::string& GetType()override;

	void UpdateWeightsTable(float blurPower);
private:

	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;
	GaussianBlurPostEffect() = default;
	~GaussianBlurPostEffect() = default;

	//コピーコンストラクタ・代入演算子削除
	GaussianBlurPostEffect& operator=(const GaussianBlurPostEffect&) = delete;
	GaussianBlurPostEffect(const GaussianBlurPostEffect&) = delete;
};

