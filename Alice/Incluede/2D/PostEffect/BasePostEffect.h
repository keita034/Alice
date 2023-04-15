#pragma once
#include"ComputePipelineState.h"
#include"Shader.h"
#include"Material.h"
#include"RenderTarget.h"
#include"PostEffectSprite.h"

class BasePostEffect 
{
protected:

	//パイプラインステート
	std::unique_ptr<Material> material;
	//初期化が必要かどうか
	bool needsInit = true;
	char PADING[7]{};

	ID3D12GraphicsCommandList* cmdList = nullptr;

	std::unique_ptr<PostEffectSprite> sprite;

	std::unique_ptr<RenderTarget> renderTarget;

	std::string type;

	float width = 0.0f;
	float height = 0.0f;

public:

	virtual ~BasePostEffect();
	BasePostEffect() = default;

	virtual void Initialize() = 0;

	virtual void PostUpdate(RenderTarget* mainRenderTarget) = 0;

	virtual const std::string& GetType() = 0;

protected:

	virtual void Draw(RenderTarget* mainRenderTarget) = 0;

	virtual void MainRenderTargetDraw(RenderTarget* mainRenderTarget) = 0;

	//コピーコンストラクタ・代入演算子削除
	BasePostEffect& operator=(const BasePostEffect&) = delete;
	BasePostEffect(const BasePostEffect&) = delete;
};

