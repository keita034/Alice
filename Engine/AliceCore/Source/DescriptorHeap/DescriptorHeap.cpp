#include "DescriptorHeap.h"

#include<assert.h>

#include"BaseDescriptorHeap.h"

/// <summary>
/// デスクプリタヒープ
/// </summary>
class SRVDescriptorHeap : public BaseDescriptorHeap, public ISRVDescriptorHeap
{
private:
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PADING1[4]{};

	uint32_t maxSRV = 0;
	uint32_t maxUAV = 0;
	uint32_t maxCBV = 0;

	uint32_t countSRV = 0;
	uint32_t countUAV = 0;
	uint32_t countCBV = 0;

	uint32_t incrementSize = 0;

	HRESULT result = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	startGpuHandle{};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device_,uint32_t maxSRV_,uint32_t maxUAV,uint32_t maxCBV);

	/// <summary>
	/// シェーダーリソースビュー生成
	/// </summary>
	uint64_t CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc_, ID3D12Resource* resource_) override;

	/// <summary>
	/// シェーダーリソースビュー追加
	/// </summary>
	DescriptorHeapViewHandle AddSRV() override;

	/// <summary>
	/// アンオーダーアクセスビュー生成
	/// </summary>
	uint64_t CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc_, ID3D12Resource* resource_,bool countBuff_ = false) override;

	/// <summary>
	/// アンオーダーアクセスビュー追加
	/// </summary>
	DescriptorHeapViewHandle AddUAV() override;

	/// <summary>
	/// コンストバッファビュー生成
	/// </summary>
	uint64_t CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc_) override;

	/// <summary>
	/// コンストバッファビュー追加
	/// </summary>
	DescriptorHeapViewHandle AddCBV() override;

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	ID3D12DescriptorHeap* GetHeap() override;

	SRVDescriptorHeap() = default;
	~SRVDescriptorHeap() = default;
};

void SRVDescriptorHeap::Initialize(ID3D12Device* device_,uint32_t maxSRV_,uint32_t maxUAV_,uint32_t maxCBV_)
{
	device = device_;
	maxSRV = maxSRV_;
	maxUAV = maxUAV_;
	maxCBV = maxCBV_;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC lSrvHeapDesc = {};
	lSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	lSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	lSrvHeapDesc.NumDescriptors = static_cast<UINT>(maxSRV + maxUAV + maxCBV);

	// 設定を元にSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&lSrvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	startGpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

uint64_t SRVDescriptorHeap::CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc_, ID3D12Resource* resource_)
{
	if (countSRV > maxSRV)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	lCpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	lGpuHandle{};

	//ハンドルのポインタずらし
	lCpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);
	lGpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(resource_, &desc_, lCpuHandle);
	countSRV++;

	return lGpuHandle.ptr;
}

SRVDescriptorHeap::DescriptorHeapViewHandle SRVDescriptorHeap::AddSRV()
{
	if (countSRV > maxSRV)
	{
		assert(0);
	}

	DescriptorHeapViewHandle lHandle{};

	//ハンドルのポインタずらし
	lHandle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);
	lHandle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);

	countSRV++;

	return lHandle;
}

uint64_t SRVDescriptorHeap::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc_, ID3D12Resource* resource_,bool countBuff_)
{
	if (countUAV > maxUAV)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	lCpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	lGpuHandle{};

	//ハンドルのポインタずらし
	lCpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);
	lGpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);

	if ( countBuff_ )
	{
		// ハンドルの指す位置にシェーダーリソースビュー作成
		device->CreateUnorderedAccessView(resource_,resource_,&desc_,lCpuHandle);
	}
	else
	{
		// ハンドルの指す位置にシェーダーリソースビュー作成
		device->CreateUnorderedAccessView(resource_,nullptr,&desc_,lCpuHandle);
	}

	countUAV++;

	return lGpuHandle.ptr;
}

SRVDescriptorHeap::DescriptorHeapViewHandle SRVDescriptorHeap::AddUAV()
{
	if (countUAV > maxUAV)
	{
		assert(0);
	}

	DescriptorHeapViewHandle lHandle{};

	//ハンドルのポインタずらし
	lHandle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);
	lHandle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);

	countUAV++;

	return lHandle;
}

uint64_t SRVDescriptorHeap::CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc_)
{
	if (countCBV > maxCBV)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	lCpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	lGpuHandle{};

	//ハンドルのポインタずらし
	lCpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);
	lGpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateConstantBufferView(&desc_, lCpuHandle);

	countCBV++;

	return lGpuHandle.ptr;
}

SRVDescriptorHeap::DescriptorHeapViewHandle SRVDescriptorHeap::AddCBV()
{
	if (countCBV > maxCBV)
	{
		assert(0);
	}

	DescriptorHeapViewHandle lHandle{};

	//ハンドルのポインタずらし
	lHandle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);
	lHandle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);

	countCBV++;

	return lHandle;
}

ID3D12DescriptorHeap* SRVDescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}

std::unique_ptr<ISRVDescriptorHeap> CreateUniqueSRVDescriptorHeap(ID3D12Device* device_,uint32_t maxSRV_,uint32_t maxUAV,uint32_t maxCBV)
{
	std::unique_ptr<ISRVDescriptorHeap> lDescriptorHeap = std::make_unique<SRVDescriptorHeap>();
	lDescriptorHeap->Initialize(device_,maxSRV_,maxUAV,maxCBV);
	return std::move(lDescriptorHeap);
}

std::shared_ptr<ISRVDescriptorHeap> CreateSharedSRVDescriptorHeap(ID3D12Device* device_,uint32_t maxSRV_,uint32_t maxUAV,uint32_t maxCBV)
{
	std::shared_ptr<ISRVDescriptorHeap> lDescriptorHeap = std::make_shared<SRVDescriptorHeap>();
	lDescriptorHeap->Initialize(device_,maxSRV_,maxUAV,maxCBV);
	return lDescriptorHeap;
}