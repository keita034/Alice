#include "DSVDescriptorHeap.h"

#include<cassert>

#include"BaseDescriptorHeap.h"

class DSVDescriptorHeap : public BaseDescriptorHeap, public IDSVDescriptorHeap
{
private:

	size_t maxDSV = 2048;

	size_t countDSV = 0;

	uint32_t incrementSize;

	char PADING2[4];

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// デプスステンシルビューを生成
	/// </summary>
	/// <param name="desc_"> デプスステンシルビューデスク</param>
	/// <param name="resource_">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	uint64_t CreateDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc_, ID3D12Resource* resource_)override;

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	ID3D12DescriptorHeap* GetHeap()override;

	uint32_t GetIncrementSize()override;

	DSVDescriptorHeap() = default;
	~DSVDescriptorHeap() = default;

};

void DSVDescriptorHeap::Initialize()
{
	HRESULT lResult = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC lDsvHeapDesc = {};
	lDsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	lDsvHeapDesc.NumDescriptors = static_cast<UINT>(maxDSV);

	// 設定を元にSRV用デスクリプタヒープを生成
	lResult = sDevice->CreateDescriptorHeap(&lDsvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(lResult));

	incrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT64 DSVDescriptorHeap::CreateDSV(const D3D12_DEPTH_STENCIL_VIEW_DESC& desc_, ID3D12Resource* resource_)
{
	if (countDSV > maxDSV)
	{
		assert(0);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE	lHandle{};

	lHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countDSV) * incrementSize);

	sDevice->CreateDepthStencilView(resource_, &desc_, lHandle);
	countDSV++;

	return lHandle.ptr;
}

ID3D12DescriptorHeap* DSVDescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}

UINT DSVDescriptorHeap::GetIncrementSize()
{
	return incrementSize;
}

std::unique_ptr<IDSVDescriptorHeap> CreateUniqueDSVDescriptorHeap()
{
	std::unique_ptr<IDSVDescriptorHeap> lDSVDescriptorHeap = std::make_unique<DSVDescriptorHeap>();
	lDSVDescriptorHeap->Initialize();
	return std::move(lDSVDescriptorHeap);
}

std::shared_ptr<IDSVDescriptorHeap> CreateSharedDSVDescriptorHeap()
{
	std::shared_ptr<IDSVDescriptorHeap> lDSVDescriptorHeap = std::make_shared<DSVDescriptorHeap>();
	lDSVDescriptorHeap->Initialize();
	return lDSVDescriptorHeap;
}