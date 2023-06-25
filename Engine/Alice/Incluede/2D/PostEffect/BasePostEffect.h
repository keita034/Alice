#pragma once

#include<Shader.h>
#include<Material.h>
#include<RenderTarget.h>
#include<PostEffectSprite.h>
#include<WindowsApp.h>

class BasePostEffect 
{
protected:

	//パイプラインステート
	std::unique_ptr<Material> material;

	std::unique_ptr<PostEffectSprite> sprite;

	static ID3D12GraphicsCommandList* sCmdList;
	static ID3D12Device* sDevice;
	static IDescriptorHeap* sSrvHeap;
	static IWindowsApp* sWindowsApp;

	//レンダーターゲットテクスチャ
	std::vector<std::unique_ptr<IRenderTargetBuffer>>renderTargetBuffers;
	//デプステクスチャ
	std::vector<std::unique_ptr<IDepthStencilBuffer>>depthStencilBuffers;
	//クリアーカラー
	std::array<float, 4> clearColor = { {0.1f, 0.25f, 0.5f, 1.0f} };

	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> handles;

	std::string type;

	float width = 0.0f;
	float height = 0.0f;

	//初期化が必要かどうか
	bool needsInit = true;

	int8_t PADING[7];

public:

	virtual ~BasePostEffect();
	BasePostEffect() = default;

	virtual void Initialize() = 0;

	virtual void PostUpdate(RenderTarget* mainRenderTarget_) = 0;

	virtual const std::string& GetType() = 0;

	static void SSetDirectX12Core(DirectX12Core* directX12Core_);

	static void SSetWindowsApp(IWindowsApp* windowsApp_);


protected:

	virtual void Draw(RenderTarget* mainRenderTarget_) = 0;

	virtual void MainRenderTargetDraw(RenderTarget* mainRenderTarget_) = 0;

	//コピーコンストラクタ・代入演算子削除
	BasePostEffect& operator=(const BasePostEffect&) = delete;
	BasePostEffect(const BasePostEffect&) = delete;
};

