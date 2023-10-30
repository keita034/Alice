#include "CrossAdapterBuffer.h"

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN
#include<wrl.h>
#include<vector>
#include<directx/d3dx12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<array>
ALICE_SUPPRESS_WARNINGS_END

#include<DescriptorHeap.h>
#include<AliceAssert.h>
#include<BaseBuffer.h>


class CrossAdapterBuffer : public BaseBuffer,public ICrossAdapterBuffer
{
private:

	static constexpr size_t NUM_RESOURCE = 2;

	IAdapter* mainAdapter = nullptr;
	IAdapter* subAdapter = nullptr;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,NUM_RESOURCE> resources;
	std::array<Microsoft::WRL::ComPtr<ID3D12Heap>,NUM_RESOURCE> heaps;
	//GPUハンドル
	std::array <D3D12_GPU_DESCRIPTOR_HANDLE,NUM_RESOURCE> structuredBufferHandles;
	size_t bufferSize;

public:

	void Create(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)override;
	bool IsValid();
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(CrossAdapterResourceIndex index_)override;
	ID3D12Resource* GetResource()override;

	~CrossAdapterBuffer() = default;
	CrossAdapterBuffer() = default;

private:

	CrossAdapterBuffer(const CrossAdapterBuffer&) = delete;
	CrossAdapterBuffer& operator = (const CrossAdapterBuffer&) = delete;
};

void CrossAdapterBuffer::Create(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	mainAdapter = sMultiAdapters->GetAdapter(mainIndex_);
	subAdapter = sMultiAdapters->GetAdapter(subIndex_);

	ID3D12Device* lMainDevice = mainAdapter->GetDevice()->Get();

	D3D12_RESOURCE_DESC lSauceResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_,D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	D3D12_RESOURCE_ALLOCATION_INFO lBuferInf = lMainDevice->GetResourceAllocationInfo(0,1,&lSauceResDesc);

	bufferSize = lBuferInf.SizeInBytes;

	HRESULT lResult;

	//メインリソース生成
	{
		CD3DX12_HEAP_DESC lHeapDesc = CD3DX12_HEAP_DESC(bufferSize,D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,0,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);
		lHeapDesc.Properties.VisibleNodeMask = 0;
		lHeapDesc.Properties.CreationNodeMask = 0;

		lResult = lMainDevice->CreateHeap(&lHeapDesc,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::MAIN ]));
		assert(SUCCEEDED(lResult));

		//リソース生成
		lResult = lMainDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),0,&lSauceResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::MAIN ]));
		assert(SUCCEEDED(lResult));
	}


	//サブリソース生成
	{
		ID3D12Device* lSubDevice = subAdapter->GetDevice()->Get();

		HANDLE lHeapHandle = nullptr;

		//ヒープを共有
		lResult = lMainDevice->CreateSharedHandle(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),nullptr,GENERIC_ALL,nullptr,&lHeapHandle);
		assert(SUCCEEDED(lResult));

		lResult = lSubDevice->OpenSharedHandle(lHeapHandle,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::SUB ]));
		assert(SUCCEEDED(lResult));

		CloseHandle(lHeapHandle);

		//リソース生成
		lResult = lSubDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::SUB ].Get(),0,&lSauceResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::SUB ]));
		assert(SUCCEEDED(lResult));
	}

	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
		lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		lUavResDesc.Format = DXGI_FORMAT_UNKNOWN;
		lUavResDesc.Buffer.NumElements = static_cast< UINT >( length_ );
		lUavResDesc.Buffer.StructureByteStride = static_cast< UINT >( singleSize_ );

		ISRVDescriptorHeap* lMainHape = mainAdapter->GetSRVDescriptorHeap();
		ISRVDescriptorHeap* lSubHape = subAdapter->GetSRVDescriptorHeap();

		structuredBufferHandles[ CrossAdapterResourceIndex::MAIN ].ptr = lMainHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::MAIN ].Get());
		structuredBufferHandles[ CrossAdapterResourceIndex::SUB ].ptr = lSubHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::SUB ].Get());
	}
}

bool CrossAdapterBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& CrossAdapterBuffer::GetAddress(CrossAdapterResourceIndex index_)
{
	return structuredBufferHandles[ index_ ];
}

ID3D12Resource* CrossAdapterBuffer::GetResource()
{
	return resource.Get();
}

std::unique_ptr<ICrossAdapterBuffer> CreateUniqueCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::unique_ptr<ICrossAdapterBuffer> lCrossAdapterBuffer = std::make_unique<CrossAdapterBuffer>();
	lCrossAdapterBuffer->Create(length_,singleSize_,mainIndex_,subIndex_);
	return std::move(lCrossAdapterBuffer);
}

std::shared_ptr<ICrossAdapterBuffer> CreateSharedCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::shared_ptr<ICrossAdapterBuffer> lCrossAdapterBuffer = std::make_shared<CrossAdapterBuffer>();
	lCrossAdapterBuffer->Create(length_,singleSize_,mainIndex_,subIndex_);
	return lCrossAdapterBuffer;
}
