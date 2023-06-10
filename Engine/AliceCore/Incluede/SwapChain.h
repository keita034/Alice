#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)

#include<RenderTargetBuffer.h>

/// <summary>
/// スワップチェイン
/// </summary>
class ISwapChain
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="swapchain">スワップチェイン</param>
	/// <param name="device_">デバイス</param>
	/// <param name="heap">レンダーターゲット用ディスクリプタヒープ</param>
	virtual void Initialize(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_) = 0 ;

	ISwapChain() = default;
	virtual ~ISwapChain() = default;

	/// <summary>
	/// リサイズ
	/// </summary>
	virtual void ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters_) = 0;

	/// <summary>
	/// フルスクリーンか
	/// </summary>
	virtual bool IsFullScreen() const = 0;

	/// <summary>
	/// フルスクリーンをセット
	/// </summary>
	virtual void SetFullScreen(bool toFullScreen_) = 0;

	/// <summary>
	/// バッファサイズ変更
	/// </summary>
	virtual void ResizeBuffers(uint32_t width_, uint32_t height_) = 0;

	virtual void WaitPreviousFrame() = 0;

	virtual void WaitForGpu() = 0;

	virtual HRESULT Present(uint32_t SyncInterval_, uint32_t Flags_) = 0;

	virtual void Transition(size_t index,const D3D12_RESOURCE_STATES& resourceStates_) = 0;

	virtual uint32_t GetCurrentBackBufferIndex() const = 0;

	virtual IRenderTargetBuffer* GetRenderTarget(size_t index_)const = 0;

	virtual const D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderTargetHandl(size_t index_) = 0;

	virtual size_t GetBackBufferCount() const = 0;
};

/// <summary>
/// 深度デスクプリタヒープの生成(ユニーク)
/// </summary>
std::unique_ptr<ISwapChain> CreateUniqueSwapChain(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_);

/// <summary>
/// 深度デスクプリタヒープの生成(シェアード)
/// </summary>
std::shared_ptr<ISwapChain> CreateSharedSwapChain(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_);