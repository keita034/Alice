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
/// デスクプリタヒープ
/// </summary>
class IDescriptorHeap
{

public:

	struct DescriptorHeapViewHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() =0 ;

	/// <summary>
	/// シェーダーリソースビュー生成
	/// </summary>
	/// <param name="desc">シェーダーリソースビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	virtual uint64_t CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc_, ID3D12Resource* resource_) = 0;

	/// <summary>
	/// シェーダーリソースビュー追加
	/// </summary>
	virtual DescriptorHeapViewHandle AddSRV() = 0;

	/// <summary>
	/// アンオーダーアクセスビュー生成
	/// </summary>
	/// <param name="desc">アンオーダーアクセスビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	virtual uint64_t CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc_, ID3D12Resource* resource_) = 0;

	/// <summary>
	/// アンオーダーアクセスビュー追加
	/// </summary>
	virtual DescriptorHeapViewHandle AddUAV() = 0;

	/// <summary>
	/// コンストバッファビュー生成
	/// </summary>
	/// <param name="desc">コンストバッファビューデスク</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	virtual uint64_t CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc_) = 0;

	/// <summary>
	/// コンストバッファビュー追加
	/// </summary>
	virtual DescriptorHeapViewHandle AddCBV() = 0;

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	virtual ID3D12DescriptorHeap* GetHeap() = 0;


	/// <summary>
	/// コンストラクタ・デストラクタ
	/// </summary>
	IDescriptorHeap() = default;
	virtual~IDescriptorHeap() = default;
};


/// <summary>
/// デスクプリタヒープの生成(ユニーク)
/// </summary>
std::unique_ptr<IDescriptorHeap> CreateUniqueDescriptorHeap();

/// <summary>
/// デスクプリタヒープの生成(シェアード)
/// </summary>
std::shared_ptr<IDescriptorHeap> CreateSharedDescriptorHeap();

