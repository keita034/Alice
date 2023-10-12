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

	IAdapter* sauceAdapter = nullptr;
	IAdapter* dustAdapter = nullptr;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,2> resources;
	//GPUハンドル
	std::array <D3D12_GPU_DESCRIPTOR_HANDLE,2> structuredBufferHandles;
	size_t bufferSize;
	std::array<Microsoft::WRL::ComPtr<ID3D12Heap>,2> heaps;

public:

	void Create(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_)override;
	bool IsValid();
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(ResourceIndex index_)override;
	ID3D12Resource* GetResource()override;
	void ResourceCopy() override;

	~CrossAdapterBuffer() = default;
	CrossAdapterBuffer() = default;

private:

	CrossAdapterBuffer(const CrossAdapterBuffer&) = delete;
	CrossAdapterBuffer& operator = (const CrossAdapterBuffer&) = delete;
};

void CrossAdapterBuffer::Create(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_)
{
	dustAdapter = sMultiAdapters->GetAdapter(dustIndex_);
	sauceAdapter = sMultiAdapters->GetAdapter(sauceIndex_);

	ID3D12Device* lSauceDevice = sauceAdapter->GetDevice()->Get();
	ID3D12Device* lDustDevice = dustAdapter->GetDevice()->Get();

	D3D12_RESOURCE_DESC lSauceResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_);
	lSauceResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	D3D12_RESOURCE_ALLOCATION_INFO lBuferInf = lSauceDevice->GetResourceAllocationInfo(0,1,&lSauceResDesc);

	bufferSize = lBuferInf.SizeInBytes;

	{
		//ヒープ生成
		CD3DX12_HEAP_DESC lHeapDesc = CD3DX12_HEAP_DESC(bufferSize,D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,0,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);

		lSauceDevice->CreateHeap(&lHeapDesc,IID_PPV_ARGS(&heaps[ ResourceIndex::SAUCE ]));

		//リソース生成
		//コピー元
		lSauceDevice->CreatePlacedResource(heaps[ ResourceIndex::SAUCE ].Get(),0,&lSauceResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE,nullptr,IID_PPV_ARGS(&resources[ ResourceIndex::SAUCE ]));
	}


	D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	lResDesc.Flags = lResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	lDustDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(resources[ ResourceIndex::DUST ].GetAddressOf())
	);
	D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
	lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	lUavResDesc.Format = DXGI_FORMAT_UNKNOWN;
	lUavResDesc.Buffer.NumElements = static_cast< UINT >( length_ );
	lUavResDesc.Buffer.StructureByteStride = static_cast< UINT >( singleSize_ );

	//ISRVDescriptorHeap* lDustHape = dustAdapter->GetSRVDescriptorHeap();
	//ISRVDescriptorHeap* lSauceHape = sauceAdapter->GetSRVDescriptorHeap();

	//structuredBufferHandles[ ResourceIndex::SAUCE ].ptr = lSauceHape->CreateUAV(lUavResDesc,resources[ ResourceIndex::SAUCE ].Get());
	//structuredBufferHandles[ ResourceIndex::DUST ].ptr = lDustHape->CreateUAV(lUavResDesc,resources[ ResourceIndex::DUST ].Get());
}

bool CrossAdapterBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& CrossAdapterBuffer::GetAddress(ResourceIndex index_)
{
	return structuredBufferHandles[ index_  ];
}

ID3D12Resource* CrossAdapterBuffer::GetResource()
{
	return resource.Get();
}

void CrossAdapterBuffer::ResourceCopy()
{
	ICommandList* lSauceCommandList = sauceAdapter->GetCommandList();
	ICommandList* lDustCommandList = dustAdapter->GetCommandList();
	ICrossAdapterFence* lCrossAdapterFence = sMultiAdapters->GetCrossAdapterFence();

	lSauceCommandList->CommandListExecute(ICommandList::CommandListIndex::COMPUTE);

	lCrossAdapterFence->Signal(lSauceCommandList->GetComputeCommandQueue());
	lCrossAdapterFence->Wait(lDustCommandList->GetCopyCommandQueue());

	lDustCommandList->CommandListExecute(ICommandList::CommandListIndex::COPY);

	ID3D12GraphicsCommandList* lDustCopyCommandList = lDustCommandList->GetCopyCommandList();
 	lDustCopyCommandList->CopyBufferRegion(resources[ ResourceIndex::DUST ].Get(),0,resources[ ResourceIndex::SAUCE ].Get(),0,bufferSize);
}

std::unique_ptr<ICrossAdapterBuffer> CreateUniqueCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_)
{
	std::unique_ptr<ICrossAdapterBuffer> lCrossAdapterBuffer = std::make_unique<CrossAdapterBuffer>();
	lCrossAdapterBuffer->Create(length_,singleSize_,dustIndex_,sauceIndex_);
	return std::move(lCrossAdapterBuffer);
}

std::shared_ptr<ICrossAdapterBuffer> CreateSharedCrossAdapterBuffer(size_t length_,size_t singleSize_,AdaptersIndex dustIndex_,AdaptersIndex sauceIndex_)
{
	std::shared_ptr<ICrossAdapterBuffer> lCrossAdapterBuffer = std::make_shared<CrossAdapterBuffer>();
	lCrossAdapterBuffer->Create(length_,singleSize_,dustIndex_,sauceIndex_);
	return lCrossAdapterBuffer;
}
