#include "DrawArgumentBuffer.h"

#include<DescriptorHeap.h>
#include<PadingType.h>
#include<AliceAssert.h>

class DrawArgumentBuffer : public BaseBuffer , public IDrawArgumentBuffer
{
private:

	static constexpr size_t NUM_RESOURCE = 2;

	IAdapter* mainAdapter = nullptr;
	IAdapter* subAdapter = nullptr;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,NUM_RESOURCE> resources;
	std::array<Microsoft::WRL::ComPtr<ID3D12Heap>,NUM_RESOURCE> heaps;
	std::array<D3D12_GPU_DESCRIPTOR_HANDLE,NUM_RESOURCE> uavHandles;
	std::array<D3D12_RESOURCE_STATES,NUM_RESOURCE> statess;

	//バッファの長さ
	size_t bufferlength;
	//要素一つ分のサイズ
	size_t bufferSingleSize;

	D3D12_GPU_DESCRIPTOR_HANDLE uavHandle;
	D3D12_RESOURCE_STATES  states;

	BufferType type;

public:

	void Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_) override;
	bool IsValid() override;


	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress(CrossAdapterResourceIndex index_) override;
	ID3D12Resource* GetResource(CrossAdapterResourceIndex index_) override;
	void Transition(D3D12_RESOURCE_STATES statesAfter_,CrossAdapterResourceIndex resourceIndex_,AdaptersIndex adaptersIndex_) override;
	D3D12_RESOURCE_STATES GetStates(CrossAdapterResourceIndex index_) override;


	 const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress() override;
	 ID3D12Resource* GetResource() override;
	 void Transition(D3D12_RESOURCE_STATES statesAfter_) override;
	 D3D12_RESOURCE_STATES GetStates()  override;

	~DrawArgumentBuffer() = default;
	DrawArgumentBuffer() = default;

private:

	DrawArgumentBuffer(const DrawArgumentBuffer&) = delete;
	DrawArgumentBuffer& operator = (const DrawArgumentBuffer&) = delete;
};

void DrawArgumentBuffer::Create(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	bufferlength = length_;
	bufferSingleSize = singleSize_;

	mainAdapter = sMultiAdapters->GetAdapter(mainIndex_);
	subAdapter = sMultiAdapters->GetAdapter(subIndex_);

	if ( type_ == BufferType::SHARED )
	{
		ID3D12Device* lMainDevice = mainAdapter->GetDevice()->Get();
		D3D12_RESOURCE_DESC lSauceResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_,D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
			//メインリソース生成
		{
			CD3DX12_HEAP_DESC lHeapDesc = CD3DX12_HEAP_DESC(singleSize_ * length_,D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,0,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);
			lHeapDesc.Properties.VisibleNodeMask = 0;
			lHeapDesc.Properties.CreationNodeMask = 0;

			lMainDevice->CreateHeap(&lHeapDesc,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::MAIN ]));

			//リソース生成
			lMainDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),0,&lSauceResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::MAIN ]));
		}

			//サブリソース生成
		{
			ID3D12Device* lSubDevice = subAdapter->GetDevice()->Get();

			HANDLE lHeapHandle = nullptr;

			//ヒープを共有
			HRESULT lResult = lMainDevice->CreateSharedHandle(heaps[ CrossAdapterResourceIndex::MAIN ].Get(),nullptr,GENERIC_ALL,nullptr,&lHeapHandle);
			AliceAssert(SUCCEEDED(lResult),"シェアハンドルの生成に失敗しました。");

			lSubDevice->OpenSharedHandle(lHeapHandle,IID_PPV_ARGS(&heaps[ CrossAdapterResourceIndex::SUB ]));

			CloseHandle(lHeapHandle);

			//リソース生成
			lSubDevice->CreatePlacedResource(heaps[ CrossAdapterResourceIndex::SUB ].Get(),0,&lSauceResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,nullptr,IID_PPV_ARGS(&resources[ CrossAdapterResourceIndex::SUB ]));
		}

		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
			lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
			lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			lUavResDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
			lUavResDesc.Buffer.NumElements = static_cast< UINT >( length_ );
			lUavResDesc.Buffer.StructureByteStride = static_cast< UINT >( singleSize_ );

			ISRVDescriptorHeap* lMainHape = mainAdapter->GetSRVDescriptorHeap();
			ISRVDescriptorHeap* lSubHape = subAdapter->GetSRVDescriptorHeap();

			uavHandles[ CrossAdapterResourceIndex::MAIN ].ptr = lMainHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::MAIN ].Get());
			uavHandles[ CrossAdapterResourceIndex::SUB ].ptr = lSubHape->CreateUAV(lUavResDesc,resources[ CrossAdapterResourceIndex::SUB ].Get());

			statess[ CrossAdapterResourceIndex::MAIN ] = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
			statess[ CrossAdapterResourceIndex::SUB ] = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
		}
	}
	else
	{
		IAdapter* lAdapter = sMultiAdapters->GetAdapter(static_cast< AdaptersIndex >( type_ ));
		ID3D12Device* lDevice = lAdapter->GetDevice()->Get();
		ISRVDescriptorHeap* lSRVHeap = lAdapter->GetSRVDescriptorHeap();

		D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_,D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		D3D12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);

		HRESULT lResult = lDevice->CreateCommittedResource(&lHeapProp,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,&lResDesc,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(resource.GetAddressOf()));

		if ( FAILED(lResult) )
		{
			printf("Failed to create buffer resource");
			return;
		}

		D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
		lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		lUavResDesc.Buffer.FirstElement = 0;
		lUavResDesc.Buffer.NumElements = ( UINT ) length_;
		lUavResDesc.Buffer.StructureByteStride = ( UINT ) singleSize_;
		lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

		uavHandle.ptr = lSRVHeap->CreateUAV(lUavResDesc,resource.Get(),true);

		states = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	}

	type = type_;
	isValid = true;
}

void DrawArgumentBuffer::Transition(D3D12_RESOURCE_STATES statesAfter_,CrossAdapterResourceIndex resourceIndex_,AdaptersIndex adaptersIndex_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(adaptersIndex_);
	ID3D12GraphicsCommandList* lCommandList = lAdapter->GetGraphicCommandList();

	CD3DX12_RESOURCE_BARRIER lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resources[ resourceIndex_ ].Get(),statess[ resourceIndex_ ],statesAfter_);
	lCommandList->ResourceBarrier(1,&lBarrier);

	statess[ resourceIndex_ ] = statesAfter_;
}


bool DrawArgumentBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawArgumentBuffer::GetAddress(CrossAdapterResourceIndex index_)
{
	return uavHandles[ index_ ];
}

ID3D12Resource* DrawArgumentBuffer::GetResource(CrossAdapterResourceIndex index_)
{
	return resources[index_].Get();
}

D3D12_RESOURCE_STATES DrawArgumentBuffer::GetStates(CrossAdapterResourceIndex index_)
{
	return statess[ index_ ];
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawArgumentBuffer::GetAddress()
{
	return uavHandle;
}

ID3D12Resource* DrawArgumentBuffer::GetResource()
{
	return resource.Get();
}

void DrawArgumentBuffer::Transition(D3D12_RESOURCE_STATES statesAfter_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(( static_cast< AdaptersIndex >( type ) ));
	ID3D12GraphicsCommandList* lCommandList = lAdapter->GetGraphicCommandList();

	CD3DX12_RESOURCE_BARRIER lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),states,statesAfter_);
	lCommandList->ResourceBarrier(1,&lBarrier);

	states = statesAfter_;
}

D3D12_RESOURCE_STATES DrawArgumentBuffer::GetStates()
{
	return states;
}

std::unique_ptr<IDrawArgumentBuffer> CreateUniqueDrawArgumentBuffer(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::unique_ptr<IDrawArgumentBuffer> lDrawArgumentBuffer = std::make_unique<DrawArgumentBuffer>();
	lDrawArgumentBuffer->Create(length_,singleSize_,type_,mainIndex_,subIndex_);
	return std::move(lDrawArgumentBuffer);
}

std::shared_ptr<IDrawArgumentBuffer> CreateSharedDrawArgumentBuffer(size_t length_,size_t singleSize_,BufferType type_,AdaptersIndex mainIndex_,AdaptersIndex subIndex_)
{
	std::shared_ptr<IDrawArgumentBuffer> lDrawArgumentBuffer = std::make_shared<DrawArgumentBuffer>();
	lDrawArgumentBuffer->Create(length_,singleSize_,type_,mainIndex_,subIndex_);
	return lDrawArgumentBuffer;
}
