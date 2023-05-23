#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>

class ChromaticAberrationPostEffect :public BasePostEffect
{
private:

	//定数バッファ用構造体
	struct Offset
	{
		float size;
	};

	std::unique_ptr<ConstantBuffer> constantBuffer;

	Offset* constBuffMap = nullptr;

public:

	static ChromaticAberrationPostEffect* GetInstance();

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

private:

	ChromaticAberrationPostEffect();
	~ChromaticAberrationPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;

	//コピーコンストラクタ・代入演算子削除
	ChromaticAberrationPostEffect& operator=(const ChromaticAberrationPostEffect&) = delete;
	ChromaticAberrationPostEffect(const ChromaticAberrationPostEffect&) = delete;
};

