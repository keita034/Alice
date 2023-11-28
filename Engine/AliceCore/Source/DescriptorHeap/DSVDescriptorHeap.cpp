#include "DSVDescriptorHeap.h"
#include<cassert>
#include"BaseDescriptorHeap.h"

class DSVDescriptorHeap : public BaseDescriptorHeap, public IDSVDescriptorHeap
{
private:

	uint32_t maxDSV = 0;

	uint32_t countDSV = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;

	uint32_t incrementSize;
	int32_t PADING;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device_,uint32_t maxDsv_)override;

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

void DSVDescriptorHeap::Initialize(ID3D12Device* device_,uint32_t maxDsv_)
{
	maxDSV = maxDsv_;
	device = device_;

	HRESULT lResult = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC lDsvHeapDesc = {};
	lDsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	lDsvHeapDesc.NumDescriptors = static_cast<UINT>(maxDSV);

	// 設定を元にSRV用デスクリプタヒープを生成
	lResult = device->CreateDescriptorHeap(&lDsvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(lResult));

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

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

	device->CreateDepthStencilView(resource_, &desc_, lHandle);
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

std::unique_ptr<IDSVDescriptorHeap> CreateUniqueDSVDescriptorHeap(ID3D12Device* device_,uint32_t maxDsv_)
{
	std::unique_ptr<IDSVDescriptorHeap> lDSVDescriptorHeap = std::make_unique<DSVDescriptorHeap>();
	lDSVDescriptorHeap->Initialize(device_,maxDsv_);
	return std::move(lDSVDescriptorHeap);
}

std::shared_ptr<IDSVDescriptorHeap> CreateSharedDSVDescriptorHeap(ID3D12Device* device_,uint32_t maxDsv_)
{
	std::shared_ptr<IDSVDescriptorHeap> lDSVDescriptorHeap = std::make_shared<DSVDescriptorHeap>();
	lDSVDescriptorHeap->Initialize(device_,maxDsv_);
	return lDSVDescriptorHeap;
}