#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<array>
#include<dxgi1_6.h>
#include<directx/d3dx12.h>
#include<memory>

#pragma warning(pop)

/// <summary>
/// レンダーターゲットバッファ(インタフェース)
/// </summary>
class IRenderTargetBuffer
{
public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="width_">横幅</param>
	/// <param name="height_">縦幅</param>
	/// <param name="resourceStates">ステータス</param>
	/// <param name="mipLevel">ミップレベル</param>
	/// <param name="arraySize">配列のサイズ</param>
	/// <param name="format">フォーマット</param>
	/// <param name="clearColor">カラー</param>
	virtual bool Create(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_, uint16_t arraySize_, DXGI_FORMAT format_, const std::array<float, 4>& clearColor_) =0;

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="swapChain4">スワップチェイン</param>
	virtual bool Create(IDXGISwapChain4* swapChain_, uint32_t index_) = 0;

	/// <summary>
	/// リソースを取得
	/// </summary>
	virtual ID3D12Resource* GetTexture() const = 0;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	virtual const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle() = 0;

	/// <summary>
	/// リソースステータス変更
	/// </summary>
	/// <param name="resourceStates">ステータス</param>
	virtual void Transition(D3D12_RESOURCE_STATES resourceStates_) = 0;

	/// <summary>
	/// リセット
	/// </summary>
	virtual void Reset() = 0;

	virtual void Resize(IDXGISwapChain4* swapChain_, uint32_t index_) = 0;

	virtual ~IRenderTargetBuffer() = default;
	IRenderTargetBuffer() = default;
};

/// <summary>
/// レンダーターゲットバッファの生成(ユニーク)
/// </summary>
/// <param name="width_">横幅</param>
/// <param name="height_">縦幅</param>
/// <param name="resourceStates">ステータス</param>
/// <param name="mipLevel">ミップレベル</param>
/// <param name="arraySize">配列のサイズ</param>
/// <param name="format">フォーマット</param>
/// <param name="clearColor">カラー</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IRenderTargetBuffer> CreateUniqueRenderTargetBuffer(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_ = 0, uint16_t arraySize_ = 1, DXGI_FORMAT format_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, const std::array<float, 4>& clearColor_ = { {1.0f,1.0f,1.0f,1.0f } });

/// <summary>
/// レンダーターゲットバッファの生成(ユニーク)
/// </summary>
/// <param name="swapChain4">スワップチェイン</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IRenderTargetBuffer> CreateUniqueRenderTargetBuffer(IDXGISwapChain4* swapChain_, uint32_t index_);

/// <summary>
/// レンダーターゲットバッファの生成(シェアード)
/// </summary>
/// <param name="width_">横幅</param>
/// <param name="height_">縦幅</param>
/// <param name="resourceStates">ステータス</param>
/// <param name="mipLevel">ミップレベル</param>
/// <param name="arraySize">配列のサイズ</param>
/// <param name="format">フォーマット</param>
/// <param name="clearColor">カラー</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IRenderTargetBuffer> CreateSharedRenderTargetBuffer(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_ = 0, uint16_t arraySize_ = 1, DXGI_FORMAT format_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, const std::array<float, 4>& clearColor_ = { {1.0f,1.0f,1.0f,1.0f } });

/// <summary>
/// レンダーターゲットバッファの生成(シェアード)
/// </summary>
/// <param name="swapChain4">スワップチェイン</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IRenderTargetBuffer> CreateSharedRenderTargetBuffer(IDXGISwapChain4* swapChain_, uint32_t index_);