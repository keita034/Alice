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
/// レンダーターゲットデスクプリタヒープ
/// </summary>
class IRTVDescriptorHeap
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// レンダーターゲットビュー生成
	/// </summary>
	/// <param name="desc_">レンダーターゲットビューデスク</param>
	/// <param name="resource_">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	virtual uint64_t CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC& desc_, ID3D12Resource* resource_) = 0;

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	virtual ID3D12DescriptorHeap* GetHeap() = 0;

	virtual uint32_t GetIncrementSize() = 0;

	IRTVDescriptorHeap() = default;
	virtual ~IRTVDescriptorHeap() = default;
};

/// <summary>
/// 深度デスクプリタヒープの生成(ユニーク)
/// </summary>
std::unique_ptr<IRTVDescriptorHeap> CreateUniqueRTVDescriptorHeap();

/// <summary>
/// 深度デスクプリタヒープの生成(シェアード)
/// </summary>
std::shared_ptr<IRTVDescriptorHeap> CreateSharedRTVDescriptorHeap();