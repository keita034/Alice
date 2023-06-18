#pragma once
#include<BasePostEffect.h>
#include<ConstantBuffer.h>
#include<AliceUtility.h>

class LutPostEffect final :public BasePostEffect
{
private:

	std::unique_ptr<IConstantBuffer> lutSizeBuff;

	struct lutSize
	{
		float width;
		float height;
		float lutRed;
		float lutGreen;
		float lutBlue;
		float lutMul;
	};

	TextureData* lutTexture;

	lutSize size = {};

public:

	static LutPostEffect* SGetInstance();

	void Initialize() override;

	void PostUpdate(RenderTarget* mainRenderTarget_) override;

	void SetLutTexture(uint32_t handle_);

	const std::string& GetType()override;

private:

	void Draw(RenderTarget* mainRenderTarget_)override;

	void MainRenderTargetDraw(RenderTarget* mainRenderTarget_)override;


	LutPostEffect() = default;
	~LutPostEffect() = default;

	//コピーコンストラクタ・代入演算子削除
	LutPostEffect& operator=(const LutPostEffect&) = delete;
	LutPostEffect(const LutPostEffect&) = delete;
};

