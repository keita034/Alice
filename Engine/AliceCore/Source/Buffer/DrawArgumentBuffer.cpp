#include "DrawArgumentBuffer.h"


class DrawArgumentBuffer : public BaseBuffer , public IDrawArgumentBuffer
{
private:

	//バッファの長さ
	size_t bufferlength;
	//要素一つ分のサイズ
	size_t bufferSingleSize;
	//マップ用ポインタ
	void* bufferMappedPtr = nullptr;
	//リソースディスク
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE structuredBufferHandle;
	//ステータス
	D3D12_RESOURCE_STATES states;
	//ヒープタイプ
	D3D12_HEAP_TYPE heapType;

public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize_">単体のサイズ</param>
	/// <param name="data_">配列の先頭アドレス</param>
	void Create(size_t length_,size_t singleSize_,const void* data_) override;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	/// <returns>バッファ生成に成功したか</returns>
	bool IsValid() override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress() override;

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource() override;

	/// <summary>
	/// バッファにマッピングされたポインタを返す
	/// </summary>
	/// <returns>バッファにマッピングされたポインタ</returns>
	void* GetPtr() const override;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data_">データ</param>
	void Update(void* data_) override;

	/// <summary>
	/// ステータス変更
	/// </summary>
	/// <param name="statesAfter_">変更したいステータス</param>
	void Transition(D3D12_RESOURCE_STATES statesAfter_) override;

	/// <summary>
	/// ステータスを取得
	/// </summary>
	/// <returns></returns>
	D3D12_RESOURCE_STATES GetStates() override;

	/// <summary>
	/// ヒープタイプを設定
	/// </summary>
	/// <param name="heapType_">ヒープタイプ</param>
	void SetHeapType(D3D12_HEAP_TYPE heapType_) override;

	~DrawArgumentBuffer() = default;
	DrawArgumentBuffer() = default;

private:

	DrawArgumentBuffer(const DrawArgumentBuffer&) = delete;
	DrawArgumentBuffer& operator = (const DrawArgumentBuffer&) = delete;
};

void DrawArgumentBuffer::Create(size_t length_,size_t singleSize_,const void* data_)
{
	bufferlength = length_;
	bufferSingleSize = singleSize_;
	D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_);
	lResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(heapType);

	ID3D12Device* lDevice = sDevice->Get();
	HRESULT lResult = lDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if ( FAILED(lResult) )
	{
		printf("Failed to create buffer resource");
		return;
	}

	// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
	resource->Map(0,nullptr,( void** ) &bufferMappedPtr);
	if ( FAILED(lResult) )
	{
		printf("Buffer mapping failed");
		return;
	}
	if ( data_ )
	{
		memcpy(bufferMappedPtr,data_,static_cast< size_t >( singleSize_ * length_ ));
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
	lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	lUavResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	lUavResDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
	lUavResDesc.Buffer.NumElements = static_cast< UINT >( length_ );
	lUavResDesc.Buffer.StructureByteStride = static_cast< UINT >( singleSize_ );

	structuredBufferHandle.ptr = sSRVHeap->CreateUAV(lUavResDesc,resource.Get());

	isValid = true;
}

void DrawArgumentBuffer::Transition(D3D12_RESOURCE_STATES statesAfter_)
{
	ID3D12GraphicsCommandList* lCommandList = sCommandList->GetGraphicCommandList();

	CD3DX12_RESOURCE_BARRIER lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),states,statesAfter_);
	lCommandList->ResourceBarrier(1,&lBarrier);

	states = statesAfter_;
}

void DrawArgumentBuffer::Update(void* data_)
{
	if ( heapType == D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD )
	{
		HRESULT lResult = resource->Map(0,nullptr,&bufferMappedPtr);
		if ( FAILED(lResult) )
		{
			printf("Buffer mapping failed");
			return;
		}

		// 頂点データをマッピング先に設定
		memcpy(bufferMappedPtr,data_,bufferSingleSize * bufferlength);

		// マッピング解除
		resource->Unmap(0,nullptr);
	}
	else
	{
		printf("このバッファもヒープタイプはアップデートではありません");
	}
}

void DrawArgumentBuffer::SetHeapType(D3D12_HEAP_TYPE heapType_)
{
	heapType = heapType_;
}

bool DrawArgumentBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& DrawArgumentBuffer::GetAddress()
{
	return structuredBufferHandle;
}

void* DrawArgumentBuffer::GetPtr()const
{
	return bufferMappedPtr;
}

ID3D12Resource* DrawArgumentBuffer::GetResource()
{
	return resource.Get();
}

D3D12_RESOURCE_STATES DrawArgumentBuffer::GetStates()
{
	return states;
}

std::unique_ptr<IDrawArgumentBuffer> CreateUniqueDrawArgumentBuffer(size_t length_,size_t singleSize_,D3D12_HEAP_TYPE heapType_,const void* data_)
{
	std::unique_ptr<IDrawArgumentBuffer> lDrawArgumentBuffer = std::make_unique<DrawArgumentBuffer>();
	lDrawArgumentBuffer->SetHeapType(heapType_);
	lDrawArgumentBuffer->Create(length_,singleSize_,data_);
	return std::move(lDrawArgumentBuffer);
}

std::shared_ptr<IDrawArgumentBuffer> CreateSharedDrawArgumentBuffer(size_t length_,size_t singleSize_,D3D12_HEAP_TYPE heapType_,const void* data_)
{
	std::shared_ptr<IDrawArgumentBuffer> lDrawArgumentBuffer = std::make_shared<DrawArgumentBuffer>();
	lDrawArgumentBuffer->SetHeapType(heapType_);
	lDrawArgumentBuffer->Create(length_,singleSize_,data_);
	return lDrawArgumentBuffer;
}
