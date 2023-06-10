#pragma once

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 4514)

#include<directx/d3d12.h>
#include<memory>
#pragma warning(pop)

/// <summary>
/// 深度デスクプリタヒープ(インターフェース)
/// </summary>
class IDSVDescriptorHeap
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// デプスステンシルビューを生成
	/// </summary>
	/// <param name="desc"> デプスステンシルビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	virtual uint64_t CreateDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc_, ID3D12Resource* resource_) = 0;

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	virtual ID3D12DescriptorHeap* GetHeap() = 0;

	/// <summary>
	/// インクリメントサイズを取得
	/// </summary>
	virtual uint32_t GetIncrementSize() = 0;

	virtual ~IDSVDescriptorHeap() = default;
	IDSVDescriptorHeap() = default;

};

/// <summary>
/// 深度デスクプリタヒープの生成(ユニーク)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IDSVDescriptorHeap> CreateUniqueDSVDescriptorHeap();

/// <summary>
/// 深度デスクプリタヒープの生成(シェアード)
/// </summary>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IDSVDescriptorHeap> CreateSharedDSVDescriptorHeap();
