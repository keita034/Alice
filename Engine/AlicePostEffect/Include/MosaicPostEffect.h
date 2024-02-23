#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>

class MosaicPostEffect : public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer>constantBuffer;
	AliceMathF::Vector2 roughness;
public:

	static MosaicPostEffect* SGetInstance();

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

	static void SetRoughness(const AliceMathF::Vector2& roughness_);

private:

	MosaicPostEffect();
	~MosaicPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;

	void PSetRoughness(const AliceMathF::Vector2& roughness_);

	//コピーコンストラクタ・代入演算子削除
	MosaicPostEffect& operator=(const MosaicPostEffect&) = delete;
	MosaicPostEffect(const MosaicPostEffect&) = delete;
};

