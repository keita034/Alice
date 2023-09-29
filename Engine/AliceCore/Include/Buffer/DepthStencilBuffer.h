#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<directx/d3dx12.h>
#include<memory>

#pragma warning(pop)

/// <summary>
/// 深度バッファ(インタフェース)
/// </summary>
class IDepthStencilBuffer
{
public:

	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="width_">横幅</param>
	/// <param name="height_">縦幅</param>
	/// <param name="format_">フォーマット</param>
	virtual bool Create(uint32_t width_, uint32_t height_, DXGI_FORMAT format_) =0;

	/// <summary>
	/// サイズ変更
	/// </summary>
	/// <param name="width_">横幅</param>
	/// <param name="height_">縦幅</param>
	virtual bool Resize(uint32_t width_, uint32_t height_) = 0;

	/// <summary>
	/// リソースを取得
	/// </summary>
	virtual ID3D12Resource* GetTexture() const = 0;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	virtual const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle() = 0;

	virtual ~IDepthStencilBuffer() = default;
	IDepthStencilBuffer() = default;
};

/// <summary>
/// 深度バッファの生成(ユニーク)
/// </summary>
/// <param name="width_">横幅</param>
/// <param name="height_">縦幅</param>
/// <param name="format_">フォーマット</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IDepthStencilBuffer> CreateUniqueDepthStencilBuffer(uint32_t width_, uint32_t height_, DXGI_FORMAT format_);

/// <summary>
/// 深度バッファの生成(シェアード)
/// </summary>
/// <param name="width_">横幅</param>
/// <param name="height_">縦幅</param>
/// <param name="format_">フォーマット</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IDepthStencilBuffer> CreateSharedDepthStencilBuffer(uint32_t width_, uint32_t height_, DXGI_FORMAT format_);