#include "RTVDescriptorHeap.h"

#include<cassert>

#include"BaseDescriptorHeap.h"

class RTVDescriptorHeap : public BaseDescriptorHeap, public IRTVDescriptorHeap
{
private:

	uint32_t maxRTV = 0;

	uint32_t countRTV = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;

	uint32_t incrementSize;
	int32_t PADING;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device_,uint32_t maxRTV_) override;

	/// <summary>
	/// レンダーターゲットビュー生成
	/// </summary>
	uint64_t CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC& desc_, ID3D12Resource* resource_) override;

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	ID3D12DescriptorHeap* GetHeap() override;

	uint32_t GetIncrementSize() override;
};

void RTVDescriptorHeap::Initialize(ID3D12Device* device_,uint32_t maxRTV_)
{
	device = device_;
	maxRTV = maxRTV_;

	HRESULT lResult = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC lRTVHeapDesc = {};
	lRTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	lRTVHeapDesc.NumDescriptors = static_cast<UINT>(maxRTV);

	// 設定を元にSRV用デスクリプタヒープを生成
	lResult = device->CreateDescriptorHeap(&lRTVHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(lResult));

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

uint64_t RTVDescriptorHeap::CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC& desc_, ID3D12Resource* resource_)
{
	if (countRTV > maxRTV)
	{
		assert(0);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE	lHandle{};

	lHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countRTV) * incrementSize);

	device->CreateRenderTargetView(resource_, &desc_, lHandle);
	countRTV++;

	return lHandle.ptr;
}

ID3D12DescriptorHeap* RTVDescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}

uint32_t RTVDescriptorHeap::GetIncrementSize()
{
	return incrementSize;
}

std::unique_ptr<IRTVDescriptorHeap> CreateUniqueRTVDescriptorHeap(ID3D12Device* device_,uint32_t maxRTV_)
{
	std::unique_ptr<IRTVDescriptorHeap> lRTVDescriptorHeap = std::make_unique<RTVDescriptorHeap>();
	lRTVDescriptorHeap->Initialize(device_,maxRTV_);
	return std::move(lRTVDescriptorHeap);
}

std::shared_ptr<IRTVDescriptorHeap> CreateSharedRTVDescriptorHeap(ID3D12Device* device_,uint32_t maxRTV_)
{
	std::shared_ptr<IRTVDescriptorHeap> lRTVDescriptorHeap = std::make_shared<RTVDescriptorHeap>();
	lRTVDescriptorHeap->Initialize(device_,maxRTV_);
	return lRTVDescriptorHeap;
}
