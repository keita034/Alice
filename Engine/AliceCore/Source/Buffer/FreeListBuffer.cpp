#include "FreeListBuffer.h"

#include<cassert>
#include<DescriptorHeap.h>
#include<PadingType.h>
#include<MultiAdapters.h>
#include<BaseBuffer.h>

class FreeListBuffer : public BaseBuffer ,public IFreeListBuffer
{
private:

	static constexpr size_t NUM_RESOURCE = 2;

	size_t bufferlength;
	size_t bufferSingleSize;

	D3D12_GPU_DESCRIPTOR_HANDLE structuredBufferHandle;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvResDesc{};

	std::array<Microsoft::WRL::ComPtr<ID3D12Heap>,NUM_RESOURCE>heaps;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,NUM_RESOURCE>resources;

	std::array <D3D12_GPU_DESCRIPTOR_HANDLE,NUM_RESOURCE> structuredBufferHandles;

	BufferType type;

	Byte4 PADING;

public:

	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	/// <param name="size">データサイズ</param>
	void Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_) override;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	bool IsValid()override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress()override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(CrossAdapterResourceIndex index_)override;

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource()override;

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource(CrossAdapterResourceIndex index_)override;

	FreeListBuffer() = default;
	~FreeListBuffer() = default;

private:

	uint32_t PAlignForUavCounter(uint32_t bufferSize_)override;

	//コピーコンストラクタ・代入演算子削除
	FreeListBuffer& operator=(const FreeListBuffer&) = delete;
	FreeListBuffer(const FreeListBuffer&) = delete;
};

void FreeListBuffer::Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	bufferlength = length_;
	bufferSingleSize = singleSize_;

	size_t deadListByteSize = singleSize_ * length_;
	size_t countBufferOffset = ( size_t ) PAlignForUavCounter(( uint32_t ) deadListByteSize);

	HRESULT lResult = 0;

	if ( type_ == BufferType::SHARED )
	{
		IAdapter* lMainAdapter = sMultiAdapters->GetAdapter(mainIndex_);
		IAdapter* lSubAdapter = sMultiAdapters->GetAdapter(subIndex_);

		ID3D12Device* lMainDevice = lMainAdapter->GetDevice()->Get();

		D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + singleSize_,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER);
		D3D12_RESOURCE_ALLOCATION_INFO lBuferInf = lMainDevice->GetResourceAllocationInfo(0,1,&lResDesc);

			//メインリソース生成
		{
			CD3DX12_HEAP_DESC lHeapDesc = CD3DX12_HEAP_DESC(lBuferInf.SizeInBytes,D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,0,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);
			lHeapDesc.Properties.VisibleNodeMask = 0;
			lHeapDesc.Properties.CreationNodeMask = 0;

			lMainDevice->CreateHeap(&lHeapDesc,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::MAIN ]));
			assert(SUCCEEDED(lResult));

			//リソース生成
			lMainDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),0,&lResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::MAIN ]));
			assert(SUCCEEDED(lResult));
		}


		//サブリソース生成
		{
			ID3D12Device* lSubDevice = lSubAdapter->GetDevice()->Get();
			HANDLE lHeapHandle = nullptr;

			//ヒープを共有
			lResult = lMainDevice->CreateSharedHandle(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),nullptr,GENERIC_ALL,nullptr,&lHeapHandle);
			assert(SUCCEEDED(lResult));

			lResult = lSubDevice->OpenSharedHandle(lHeapHandle,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::SUB ]));
			assert(SUCCEEDED(lResult));

			CloseHandle(lHeapHandle);

			//リソース生成
			lResult = lSubDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::SUB ].Get(),0,&lResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::SUB ]));
			assert(SUCCEEDED(lResult));
		}

		//UAV生成
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
			lUavResDesc.Format = DXGI_FORMAT_UNKNOWN;
			lUavResDesc.Buffer.FirstElement = 0;
			lUavResDesc.Buffer.NumElements = static_cast< uint32_t >( length_ );
			lUavResDesc.Buffer.StructureByteStride = static_cast< uint32_t >( singleSize_ );
			lUavResDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
			lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

			ISRVDescriptorHeap* lMainHape = lMainAdapter->GetSRVDescriptorHeap();
			ISRVDescriptorHeap* lSubHape = lSubAdapter->GetSRVDescriptorHeap();

			structuredBufferHandles[ CrossAdapterResourceIndex::MAIN ].ptr = lMainHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::MAIN ].Get(),true);
			structuredBufferHandles[ CrossAdapterResourceIndex::SUB ].ptr = lSubHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::SUB ].Get(),true);
		}
	}
	else
	{
		IAdapter* lAdapter = sMultiAdapters->GetAdapter(static_cast< AdaptersIndex >( type_ ));
		ID3D12Device* lDevice = lAdapter->GetDevice()->Get();
		ISRVDescriptorHeap* lSRVHeap = lAdapter->GetSRVDescriptorHeap();

		D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + singleSize_,D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		D3D12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);

		lResult = lDevice->CreateCommittedResource(&lHeapProp,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,&lResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(resource.GetAddressOf()));
		assert(SUCCEEDED(lResult));

		D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
		lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		lUavResDesc.Buffer.FirstElement = 0;
		lUavResDesc.Buffer.NumElements = ( UINT ) length_;
		lUavResDesc.Buffer.StructureByteStride = ( UINT ) singleSize_;
		lUavResDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
		lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

		structuredBufferHandle.ptr = lSRVHeap->CreateUAV(lUavResDesc,resource.Get(),true);
	}

	type = type_;
	isValid = true;
}

bool FreeListBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& FreeListBuffer::GetAddress()
{
	if ( type == BufferType::SHARED )
	{
		return structuredBufferHandles[ CrossAdapterResourceIndex::MAIN ];
	}
	else
	{
		return structuredBufferHandle;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE& FreeListBuffer::GetAddress(CrossAdapterResourceIndex index_)
{
	if ( type == BufferType::SHARED )
	{
		return structuredBufferHandles[ index_ ];
	}
	else
	{
		return structuredBufferHandle;
	}
}

ID3D12Resource* FreeListBuffer::GetResource(CrossAdapterResourceIndex index_)
{
	if ( type == BufferType::SHARED )
	{
		return resources[ index_ ].Get();
	}
	else
	{
		return resource.Get();
	}
}

ID3D12Resource* FreeListBuffer::GetResource()
{
	if ( type == BufferType::SHARED )
	{
		return resources[ CrossAdapterResourceIndex::MAIN ].Get();
	}
	else
	{
		return resource.Get();
	}

}

uint32_t FreeListBuffer::PAlignForUavCounter(uint32_t bufferSize_)
{
	const uint32_t alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
	return ( bufferSize_ + ( alignment - 1 ) ) & ~( alignment - 1 );
}

std::unique_ptr<IFreeListBuffer> CreateUniqueFreeListBuffer(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::unique_ptr<IFreeListBuffer> lFreeListBuffer = std::make_unique<FreeListBuffer>();
	lFreeListBuffer->Create(length_,singleSize_,type_,mainIndex_,subIndex_);
	return std::move(lFreeListBuffer);
}

std::shared_ptr<IFreeListBuffer> CreateSharedFreeListBuffer(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::shared_ptr<IFreeListBuffer> lFreeListBuffer = std::make_shared<FreeListBuffer>();
	lFreeListBuffer->Create(length_,singleSize_,type_,mainIndex_,subIndex_);
	return lFreeListBuffer;
}
