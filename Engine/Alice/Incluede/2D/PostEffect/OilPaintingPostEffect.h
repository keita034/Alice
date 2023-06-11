#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>

class OilPaintingPostEffect:public BasePostEffect
{
private:

	//定数バッファ用構造体
	struct RadTxeSize
	{
		INT radius;
		float radiusF;
		AliceMathF::Vector2 TexSize;
	};

	std::unique_ptr<IConstantBuffer> constantBuffer;

	RadTxeSize constBuffMap;

public:

	static OilPaintingPostEffect* GetInstance();

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

	OilPaintingPostEffect();
	~OilPaintingPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget)override;

	//コピーコンストラクタ・代入演算子削除
	OilPaintingPostEffect& operator=(const OilPaintingPostEffect&) = delete;
	OilPaintingPostEffect(const OilPaintingPostEffect&) = delete;
};

