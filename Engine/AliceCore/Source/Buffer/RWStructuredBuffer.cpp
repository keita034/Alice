#include"RWStructuredBuffer.h"

#include"BaseBuffer.h"

/// <summary>
/// 書き込みと読み込み用構造化バッファ
/// </summary>
class RWStructuredBuffer : public BaseBuffer, public IRWStructuredBuffer
{
private:
	//マップ用ポインタ
	void* bufferMappedPtr = nullptr;
	//個数
	size_t bufferlength;
	//データ一つ分のサイズ
	size_t bufferSingleSize;
	//GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE structuredBufferHandle;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvResDesc{};
public:

	/// <summary>
	/// シェーダーリソースビューを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize_">要素1つ分のデータサイズ</param>
	/// <param name="data_">データ</param>
	void CreateSRV(size_t length_, size_t singleSize_, const void* data_ = nullptr)override;

	/// <summary>
	/// アンオーダーアクセスビューを生成
	/// </summary>
	/// <param name="length_">個数</param>
	/// <param name="singleSize__">要素1つ分のデータサイズ</param>
	/// <param name="data_">データ</param>
	void CreateUAV(size_t length_, size_t singleSize_, const void* data_ = nullptr)override;

	/// <summary>
	/// アンオーダーアクセスビューを生成
	/// </summary>
	void CreateUAV(CD3DX12_RESOURCE_DESC* texresDesc_)override;

	/// <summary>
	/// バッファ生成に成功したかを返す
	/// </summary>
	bool IsValid()override;

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	/// <returns>バッファのGPU上のアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress()override;

	/// <summary>
	/// 定数バッファビューを返す
	/// </summary>
	/// <returns>定数バッファビュー</returns>
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetViewDesc()override;

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource()override;

	/// <summary>
	/// 定数バッファにマッピングされたポインタを返す
	/// </summary>
	/// <returns>定数バッファにマッピングされたポインタ</returns>
	void* GetPtr() const override;

	/// <summary>
	/// データを更新
	/// </summary>
	/// <param name="data_"></param>
	void Update(void* data_)override;

	~RWStructuredBuffer() = default;
	RWStructuredBuffer() = default;

private:

	RWStructuredBuffer(const RWStructuredBuffer&) = delete;
	RWStructuredBuffer& operator = (const RWStructuredBuffer&) = delete;

};
void RWStructuredBuffer::CreateSRV(size_t length_, size_t singleSize_, const void* data_)
{
	bufferlength = length_;
	bufferSingleSize = singleSize_;

	D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_);
	lResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES lHeapProp{};
	lHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	lHeapProp.CreationNodeMask = 1;
	lHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	lHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	lHeapProp.VisibleNodeMask = 1;
	HRESULT lResult = sDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(lResult))
	{
		printf("Failed to create buffer resource");
		return;
	}

	// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
	lResult = resource->Map(0, nullptr, reinterpret_cast<void**>(&bufferMappedPtr));
	if (FAILED(lResult))
	{
		printf("Buffer mapping failed");
		return;
	}

	if (data_ != nullptr)
	{
		memcpy(bufferMappedPtr, data_, static_cast<size_t>(singleSize_ * length_));
	}

	
	srvResDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvResDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//バッファとして
	srvResDesc.Buffer.NumElements = static_cast<UINT>(length_);//要素の総数
	srvResDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize_);//1個当たりの大きさ
	srvResDesc.Buffer.FirstElement = 0;
	srvResDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvResDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	structuredBufferHandle.ptr = sSRVHeap->CreateSRV(srvResDesc, resource.Get());

	isValid = true;
}

void RWStructuredBuffer::CreateUAV(size_t length_, size_t singleSize_, const void* data_)
{
	bufferlength = length_;
	bufferSingleSize = singleSize_;
	D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_);
	lResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES lHeapProp{};
	lHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	lHeapProp.CreationNodeMask = 1;
	lHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	lHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	lHeapProp.VisibleNodeMask = 1;
	HRESULT lResult = sDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(lResult))
	{
		printf("Failed to create buffer resource");
		return;
	}

	// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
	resource->Map(0, nullptr, (void**)&bufferMappedPtr);
	if (FAILED(lResult))
	{
		printf("Buffer mapping failed");
		return;
	}
	if (data_)
	{
		memcpy(bufferMappedPtr, data_, static_cast<size_t>(singleSize_ * length_));
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
	lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	lUavResDesc.Format = DXGI_FORMAT_UNKNOWN;
	lUavResDesc.Buffer.NumElements = static_cast<UINT>(length_);
	lUavResDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize_);

	structuredBufferHandle.ptr = sSRVHeap->CreateUAV(lUavResDesc, resource.Get());

	isValid = true;
}

void RWStructuredBuffer::CreateUAV(CD3DX12_RESOURCE_DESC* texresDesc)
{
	texresDesc->Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES lHeapProp{};
	lHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	lHeapProp.CreationNodeMask = 1;
	lHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	lHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	lHeapProp.VisibleNodeMask = 1;
	HRESULT lResult = sDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		texresDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(lResult))
	{
		printf("Failed to create buffer resource");
		return;
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC lUavResDesc{};
	lUavResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	lUavResDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;//テクスチャとして　
	lUavResDesc.Texture2D.MipSlice = 0;
	lUavResDesc.Texture2D.PlaneSlice = 0;

	structuredBufferHandle.ptr = sSRVHeap->CreateUAV(lUavResDesc, resource.Get());

	isValid = true;
}

void RWStructuredBuffer::Update(void* data_)
{
	HRESULT lResult = resource->Map(0, nullptr, &bufferMappedPtr);
	if (FAILED(lResult))
	{
		printf("Buffer mapping failed");
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr, data_, bufferSingleSize * bufferlength);

	// マッピング解除
	resource->Unmap(0, nullptr);
}

bool RWStructuredBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& RWStructuredBuffer::GetAddress()
{
	return structuredBufferHandle;
}

void* RWStructuredBuffer::GetPtr()const
{
	return bufferMappedPtr;
}

const D3D12_SHADER_RESOURCE_VIEW_DESC& RWStructuredBuffer::GetViewDesc()
{
	return srvResDesc;
}

ID3D12Resource* RWStructuredBuffer::GetResource()
{
	return resource.Get();
}

std::unique_ptr<IRWStructuredBuffer> CreateUniqueSRVRWStructuredBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::unique_ptr<IRWStructuredBuffer> lRWStructuredBuffer = std::make_unique<RWStructuredBuffer>();
	lRWStructuredBuffer->CreateSRV(length_, singleSize_, data_);
	return std::move(lRWStructuredBuffer);
}

std::shared_ptr<IRWStructuredBuffer> CreateSharedSRVRWStructuredBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::shared_ptr<IRWStructuredBuffer> lRWStructuredBuffer = std::make_shared<RWStructuredBuffer>();
	lRWStructuredBuffer->CreateSRV(length_, singleSize_, data_);
	return lRWStructuredBuffer;
}

std::unique_ptr<IRWStructuredBuffer> CreateUniqueUAVRWStructuredBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::unique_ptr<IRWStructuredBuffer> lRWStructuredBuffer = std::make_unique<RWStructuredBuffer>();
	lRWStructuredBuffer->CreateUAV(length_, singleSize_, data_);
	return std::move(lRWStructuredBuffer);
}

std::shared_ptr<IRWStructuredBuffer> CreateSharedUAVRWStructuredBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::shared_ptr<IRWStructuredBuffer> lRWStructuredBuffer = std::make_shared<RWStructuredBuffer>();
	lRWStructuredBuffer->CreateUAV(length_, singleSize_, data_);
	return lRWStructuredBuffer;
}
