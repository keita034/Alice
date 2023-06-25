#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceMathUtility.h>

class FrostedGlassPostEffect : public BasePostEffect
{

private:

	struct ConstantBufferMap
	{
		AliceMathF::Vector2 intensity;
		float radius;
	};

	std::unique_ptr<IConstantBuffer>constantBuffer;


	ConstantBufferMap constantBufferMap;

	int32_t PADING;

public:

	static FrostedGlassPostEffect* SGetInstance();

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

	static void SetIntensity(const AliceMathF::Vector2& intensity_);

	static void SetRadius(float radius_);

private:

	FrostedGlassPostEffect();
	~FrostedGlassPostEffect() = default;

	//描画
	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;


	void PSetIntensity(const AliceMathF::Vector2& intensity_);

	void PSetRadius(float radius_);

	//コピーコンストラクタ・代入演算子削除
	FrostedGlassPostEffect& operator=(const FrostedGlassPostEffect&) = delete;
	FrostedGlassPostEffect(const FrostedGlassPostEffect&) = delete;
};


