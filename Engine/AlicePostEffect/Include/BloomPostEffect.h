#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>
#include<GaussianYBlurPostEffect.h>
#include<GaussianXBlurPostEffect.h>
#include<WindowsApp.h>

class BloomPostEffect : public BasePostEffect
{

private:

	std::unique_ptr<Material>material2;


public:

	static BloomPostEffect* SGetInstance();

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

	BloomPostEffect();
	~BloomPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	BloomPostEffect& operator=(const BloomPostEffect&) = delete;
	BloomPostEffect(const BloomPostEffect&) = delete;
};