#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>

class SandStormPostEffect : public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer>constantBuffer;
	float time = 0;
	const float TIME_MAX = D3D12_FLOAT32_MAX;
public:

	static SandStormPostEffect* SGetInstance();

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

	SandStormPostEffect();
	~SandStormPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	SandStormPostEffect& operator=(const SandStormPostEffect&) = delete;
	SandStormPostEffect(const SandStormPostEffect&) = delete;
};

