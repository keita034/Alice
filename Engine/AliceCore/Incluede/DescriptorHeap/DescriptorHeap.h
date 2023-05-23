﻿#pragma once
#include"BaseDescriptorHeap.h"

/// <summary>
/// デスクプリタヒープ
/// </summary>
class DescriptorHeap : public BaseDescriptorHeap
{
private:
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PADING1[4];

	size_t maxSRV = 340000;
	size_t maxUAV = 330000;
	size_t maxCBV = 330000;

	size_t countSRV = 0;
	size_t countUAV = 0;
	size_t countCBV = 0;

	UINT incrementSize;
	char PADING2[4];


	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	startGpuHandle;

public:

	struct DescriptorHeapViewHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// シェーダーリソースビュー生成
	/// </summary>
	/// <param name="desc">シェーダーリソースビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// シェーダーリソースビュー追加
	/// </summary>
	DescriptorHeapViewHandle AddSRV();

	/// <summary>
	/// アンオーダーアクセスビュー生成
	/// </summary>
	/// <param name="desc">アンオーダーアクセスビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// アンオーダーアクセスビュー追加
	/// </summary>
	DescriptorHeapViewHandle AddUAV();

	/// <summary>
	/// コンストバッファビュー生成
	/// </summary>
	/// <param name="desc">コンストバッファビューデスク</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

	/// <summary>
	/// コンストバッファビュー追加
	/// </summary>
	DescriptorHeapViewHandle AddCBV();

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	ID3D12DescriptorHeap* GetHeap();
};

