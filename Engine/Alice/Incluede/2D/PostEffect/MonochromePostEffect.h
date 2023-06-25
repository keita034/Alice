#pragma once
#include<BasePostEffect.h>

class MonochromePostEffect : public BasePostEffect
{
private:

public:

	static MonochromePostEffect* SGetInstance();

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

	MonochromePostEffect();
	~MonochromePostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	MonochromePostEffect& operator=(const MonochromePostEffect&) = delete;
	MonochromePostEffect(const MonochromePostEffect&) = delete;
};

