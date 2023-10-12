#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>

class DistortionPostEffect : public BasePostEffect
{
private:

	struct ConstantBufferMap
	{
		AliceMathF::Vector2 center;
		float power;
	};
	int32_t PADING;

	ConstantBufferMap constantBufferMap;
	std::unique_ptr<IConstantBuffer>constantBuffer;
public:

	static DistortionPostEffect* SGetInstance();

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

	DistortionPostEffect();
	~DistortionPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	//コピーコンストラクタ・代入演算子削除
	DistortionPostEffect& operator=(const DistortionPostEffect&) = delete;
	DistortionPostEffect(const DistortionPostEffect&) = delete;
};