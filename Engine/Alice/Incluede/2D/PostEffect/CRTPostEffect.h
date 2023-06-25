#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>

class CRTPostEffect : public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer>constantBuffer;
	float time = 0;
	const float TIME_MAX = D3D12_FLOAT32_MAX;

	static float incrementNum;

public:

	static CRTPostEffect* SGetInstance();

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

	static void SetIncrementNum(float num);

private:

	CRTPostEffect();
	~CRTPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	CRTPostEffect& operator=(const CRTPostEffect&) = delete;
	CRTPostEffect(const CRTPostEffect&) = delete;
};
