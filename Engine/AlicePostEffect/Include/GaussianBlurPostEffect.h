#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>
#include<GaussianYBlurPostEffect.h>
#include<GaussianXBlurPostEffect.h>
#include<WindowsApp.h>

class GaussianBlurPostEffect final: public BasePostEffect
{
private:

	const size_t NUM_WEIGHTS = 8;

	std::array<float, 8> weight{};

	std::unique_ptr<GaussianXBlurPostEffect>gaussianXBlurPostEffect;
	std::unique_ptr<GaussianYBlurPostEffect>gaussianYBlurPostEffect;

public:

	static GaussianBlurPostEffect* SGetInstance();

	//初期化
	void Initialize() override;

	//更新
	void PostUpdate(RenderTarget* mainRenderTarget_) override;

	/// <summary>
	/// タイプを取得
	/// </summary>
	const std::string& GetType()override;

	void UpdateWeightsTable(float blurPower_);
private:

	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;
	GaussianBlurPostEffect();
	~GaussianBlurPostEffect() = default;

	//コピーコンストラクタ・代入演算子削除
	GaussianBlurPostEffect& operator=(const GaussianBlurPostEffect&) = delete;
	GaussianBlurPostEffect(const GaussianBlurPostEffect&) = delete;
};

