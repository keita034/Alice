#include "DescriptorHeap.h"

#include<assert.h>

#include"BaseDescriptorHeap.h"

/// <summary>
/// デスクプリタヒープ
/// </summary>
class DescriptorHeap : public BaseDescriptorHeap, public IDescriptorHeap
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

	uint32_t incrementSize;

	HRESULT result = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	startGpuHandle;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	uint64_t CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc_, ID3D12Resource* resource_) override;

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

	DescriptorHeap() = default;
	~DescriptorHeap() = default;
};

void DescriptorHeap::Initialize()
{
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC lSrvHeapDesc = {};
	lSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	lSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	lSrvHeapDesc.NumDescriptors = static_cast<UINT>(maxSRV + maxUAV + maxCBV);

	// 設定を元にSRV用デスクリプタヒープを生成
	result = sDevice->CreateDescriptorHeap(&lSrvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	incrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	startGpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

uint64_t DescriptorHeap::CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc_, ID3D12Resource* resource_)
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
	sDevice->CreateShaderResourceView(resource_, &desc_, lCpuHandle);
	countSRV++;

	return lGpuHandle.ptr;
}

DescriptorHeap::DescriptorHeapViewHandle DescriptorHeap::AddSRV()
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

uint64_t DescriptorHeap::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc_, ID3D12Resource* resource_)
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

	// ハンドルの指す位置にシェーダーリソースビュー作成
	sDevice->CreateUnorderedAccessView(resource_, nullptr, &desc_, lCpuHandle);

	countUAV++;

	return lGpuHandle.ptr;
}

DescriptorHeap::DescriptorHeapViewHandle DescriptorHeap::AddUAV()
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

uint64_t DescriptorHeap::CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc_)
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
	sDevice->CreateConstantBufferView(&desc_, lCpuHandle);

	countCBV++;

	return lGpuHandle.ptr;
}

DescriptorHeap::DescriptorHeapViewHandle DescriptorHeap::AddCBV()
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

ID3D12DescriptorHeap* DescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}

std::unique_ptr<IDescriptorHeap> CreateUniqueDescriptorHeap()
{
	std::unique_ptr<IDescriptorHeap> lDescriptorHeap = std::make_unique<DescriptorHeap>();
	lDescriptorHeap->Initialize();
	return std::move(lDescriptorHeap);
}

std::shared_ptr<IDescriptorHeap> CreateSharedDescriptorHeap()
{
	std::shared_ptr<IDescriptorHeap> lDescriptorHeap = std::make_shared<DescriptorHeap>();
	lDescriptorHeap->Initialize();
	return lDescriptorHeap;
}
