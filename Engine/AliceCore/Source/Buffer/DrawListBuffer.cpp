#include "DrawListBuffer.h"

#include<DescriptorHeap.h>
#include<cassert>
#include<MultiAdapters.h>
#include<BaseBuffer.h>

class DrawListBuffer : public BaseBuffer , public IDrawListBuffer
{
private:

	static constexpr size_t NUM_RESOURCE = 2;

	size_t bufferlength;
	size_t bufferSingleSize;

	std::array<Microsoft::WRL::ComPtr<ID3D12Heap>,NUM_RESOURCE>heaps;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,NUM_RESOURCE>resources;

	D3D12_GPU_DESCRIPTOR_HANDLE uavHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle;

	std::array <D3D12_GPU_DESCRIPTOR_HANDLE,NUM_RESOURCE> uavHandles;
	std::array <D3D12_GPU_DESCRIPTOR_HANDLE,NUM_RESOURCE> srvHandles;

	BufferType type;

	Byte4 PADING;

public:

	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	/// <param name="size">データサイズ</param>
	void Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)override;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	bool IsValid()override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetUAVAddress()override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetUAVAddress(CrossAdapterResourceIndex index_)override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetSRVAddress()override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetSRVAddress(CrossAdapterResourceIndex index_)override;
	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource()override;

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource(CrossAdapterResourceIndex index_)override;

	DrawListBuffer() = default;
	~DrawListBuffer() = default;

private:

	uint32_t PAlignForUavCounter(uint32_t bufferSize_)override;

	//コピーコンストラクタ・代入演算子削除
	DrawListBuffer& operator=(const DrawListBuffer&) = delete;
	DrawListBuffer(const DrawListBuffer&) = delete;
};

void DrawListBuffer::Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	bufferlength = length_;
	bufferSingleSize = singleSize_;

	size_t deadListByteSize = singleSize_ * length_;
	size_t countBufferOffset = ( size_t ) PAlignForUavCounter(( uint32_t ) deadListByteSize);

	HRESULT lResult;

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

			lResult = lMainDevice->CreateHeap(&lHeapDesc,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::MAIN ]));
			assert(SUCCEEDED(lResult));

			//リソース生成
			lResult = lMainDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),0,&lResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::MAIN ]));
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
			lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			lUavResDesc.Buffer.FirstElement = 0;
			lUavResDesc.Buffer.NumElements = static_cast< uint32_t > ( length_ );
			lUavResDesc.Buffer.StructureByteStride = static_cast< uint32_t > ( singleSize_ );
			lUavResDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
			lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
			lUavResDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;

			ISRVDescriptorHeap* lMainHape = lMainAdapter->GetSRVDescriptorHeap();
			ISRVDescriptorHeap* lSubHape = lSubAdapter->GetSRVDescriptorHeap();

			uavHandles[ CrossAdapterResourceIndex::MAIN ].ptr = lMainHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::MAIN ].Get(),true);
			uavHandles[ CrossAdapterResourceIndex::SUB ].ptr = lSubHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::SUB ].Get(),true);
		}

		//srv生成
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC lSrvResDesc = {};
			lSrvResDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			lSrvResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			lSrvResDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
			lSrvResDesc.Buffer.FirstElement = 0;
			lSrvResDesc.Buffer.NumElements = static_cast< uint32_t > ( length_ );
			lSrvResDesc.Buffer.StructureByteStride = static_cast< uint32_t > ( singleSize_ );

			ISRVDescriptorHeap* lMainHape = lMainAdapter->GetSRVDescriptorHeap();
			ISRVDescriptorHeap* lSubHape = lSubAdapter->GetSRVDescriptorHeap();

			srvHandles[ CrossAdapterResourceIndex::MAIN ].ptr = lMainHape->CreateSRV(lSrvResDesc,resources[ CrossAdapterResourceIndex::MAIN ].Get());
			srvHandles[ CrossAdapterResourceIndex::SUB ].ptr = lSubHape->CreateSRV(lSrvResDesc,resources[ CrossAdapterResourceIndex::SUB ].Get());
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

		//uav生成
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
			lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			lUavResDesc.Buffer.FirstElement = 0;
			lUavResDesc.Buffer.NumElements = static_cast< uint32_t > ( length_ );
			lUavResDesc.Buffer.StructureByteStride = static_cast< uint32_t > ( singleSize_ );
			lUavResDesc.Buffer.CounterOffsetInBytes = countBufferOffset;
			lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
			lUavResDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;

			uavHandle.ptr = lSRVHeap->CreateUAV(lUavResDesc,resource.Get(),true);
		}

		//srv生成
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC lSrvResDesc = {};
			lSrvResDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			lSrvResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			lSrvResDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
			lSrvResDesc.Buffer.FirstElement = 0;
			lSrvResDesc.Buffer.NumElements = static_cast< uint32_t > ( length_ );
			lSrvResDesc.Buffer.StructureByteStride = static_cast< uint32_t > ( singleSize_ );

			srvHandle.ptr = lSRVHeap->CreateSRV(lSrvResDesc,resource.Get());
		}
	}

	type = type_;
	isValid = true;
}

bool DrawListBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawListBuffer::GetUAVAddress()
{
	if ( type == BufferType::SHARED )
	{
		return uavHandles[ CrossAdapterResourceIndex::MAIN ];
	}
	else
	{
		return uavHandle;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawListBuffer::GetUAVAddress(CrossAdapterResourceIndex index_)
{
	if ( type == BufferType::SHARED )
	{
		return uavHandles[ index_ ];
	}
	else
	{
		return uavHandle;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawListBuffer::GetSRVAddress()
{
	if ( type == BufferType::SHARED )
	{
		return srvHandles[ CrossAdapterResourceIndex::MAIN ];
	}
	else
	{
		return srvHandle;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawListBuffer::GetSRVAddress(CrossAdapterResourceIndex index_)
{
	if ( type == BufferType::SHARED )
	{
		return srvHandles[ index_ ];
	}
	else
	{
		return srvHandle;
	}
}

ID3D12Resource* DrawListBuffer::GetResource(CrossAdapterResourceIndex index_)
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

uint32_t DrawListBuffer::PAlignForUavCounter(uint32_t bufferSize_)
{
	const uint32_t alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
	return ( bufferSize_ + ( alignment - 1 ) ) & ~( alignment - 1 );
}

ID3D12Resource* DrawListBuffer::GetResource()
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

std::unique_ptr<IDrawListBuffer> CreateUniqueDrawListBuffer(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::unique_ptr<IDrawListBuffer> lDrawListBuffer = std::make_unique<DrawListBuffer>();
	lDrawListBuffer->Create(length_,singleSize_,type_,mainIndex_,subIndex_);
	return std::move(lDrawListBuffer);
}

std::shared_ptr<IDrawListBuffer> CreateSharedDrawListBuffer(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::shared_ptr<IDrawListBuffer> lDrawListBuffer = std::make_shared<DrawListBuffer>();
	lDrawListBuffer->Create(length_,singleSize_,type_,mainIndex_,subIndex_);
	return lDrawListBuffer;
}
