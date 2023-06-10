#include "ComputeVertexBuffer.h"
#include"BaseBuffer.h"

/// <summary>
/// コンピュートシェーダー用頂点バッファ
/// </summary>
class ComputeVertexBuffer : public BaseBuffer, public IComputeVertexBuffer
{
private:
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	//GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE handl = {};
	//マップ用ポインタ
	void* bufferMappedPtr = nullptr;

public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	void Create(size_t length_, size_t singleSize_, const void* data_) override;

	/// <summary>
	/// 頂点バッファビューを取得
	/// </summary>
	const D3D12_VERTEX_BUFFER_VIEW& GetView() override;

	ID3D12Resource* GetResource();

	/// <summary>
	/// バッファの生成に成功したかを取得
	/// </summary>
	bool IsValid();

	/// <summary>
	/// バッファのGPU上のアドレスを返す
	/// </summary>
	const D3D12_GPU_DESCRIPTOR_HANDLE& GetAddress() override;

	/// <summary>
	/// バッファの状態を変える
	/// </summary>
	void Transition(const D3D12_RESOURCE_STATES& beforeState_, const D3D12_RESOURCE_STATES& afterState_) override;

	/// <summary>
	/// データ更新
	/// </summary>
	void Update(void* data_)override;

	~ComputeVertexBuffer() = default;
	ComputeVertexBuffer() = default;

private:

	ComputeVertexBuffer(const ComputeVertexBuffer&) = delete;

	void operator = (const ComputeVertexBuffer&) = delete;
};

void ComputeVertexBuffer::Create(size_t length_, size_t singleSize_, const void* data_)
{
	if (!isValid)
	{
		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES lHeapProp{};
		lHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		lHeapProp.CreationNodeMask = 1;
		lHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		lHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
		lHeapProp.VisibleNodeMask = 1;

		// リソースの設定
		D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize_ * length_);
		lResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		// リソースを生成
		HRESULT lResult = sDevice->CreateCommittedResource(
			&lHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&lResDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(lResult))
		{
			printf("Failed to create vertex buffer resource");
			return;
		}

		// 頂点バッファビューの設定
		vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
		vertexBufferView.SizeInBytes = static_cast<UINT>(singleSize_ * length_);
		vertexBufferView.StrideInBytes = static_cast<UINT>(singleSize_);

		lResult = resource->Map(0, nullptr, &bufferMappedPtr);
		if (FAILED(lResult))
		{
			printf("Vertex buffer mapping failed");
			return;
		}

		// マッピングする
		if (data_ != nullptr)
		{
			// 頂点データをマッピング先に設定
			memcpy(bufferMappedPtr, data_, singleSize_ * length_);
		}

		D3D12_UNORDERED_ACCESS_VIEW_DESC lUavDesc{};
		lUavDesc.Format = DXGI_FORMAT_UNKNOWN;
		lUavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		lUavDesc.Buffer.NumElements = static_cast<UINT>(length_);
		lUavDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize_);

		handl.ptr = sSRVHeap->CreateUAV(lUavDesc, resource.Get());

		isValid = true;

		resource->Unmap(0, nullptr);
	}
}

void ComputeVertexBuffer::Update(void* data_)
{
	if (data_ == nullptr)
	{
		return;
	}

	HRESULT lResult = resource->Map(0, nullptr, &bufferMappedPtr);

	if (FAILED(lResult))
	{
		printf("Vertex buffer mapping failed");
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr, data_, vertexBufferView.SizeInBytes);

	resource->Unmap(0, nullptr);
}

const D3D12_VERTEX_BUFFER_VIEW& ComputeVertexBuffer::GetView()
{
	return vertexBufferView;
}

bool ComputeVertexBuffer::IsValid()
{
	return isValid;
}

ID3D12Resource* ComputeVertexBuffer::GetResource()
{
	return resource.Get();
}

void ComputeVertexBuffer::Transition(const D3D12_RESOURCE_STATES& beforeState_, const D3D12_RESOURCE_STATES& afterState_)
{
	auto lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), beforeState_, afterState_);
	sCommandList->ResourceBarrier(1, &lBarrier);
}

const D3D12_GPU_DESCRIPTOR_HANDLE& ComputeVertexBuffer::GetAddress()
{
	return handl;
}

std::unique_ptr<IComputeVertexBuffer> CreateUniqueComputeVertexBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::unique_ptr<IComputeVertexBuffer> lComputeVertexBuffer = std::make_unique<ComputeVertexBuffer>();
	lComputeVertexBuffer->Create(length_, singleSize_, data_);
	return std::move(lComputeVertexBuffer);
}

std::shared_ptr<IComputeVertexBuffer> CreateSharedComputeVertexBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::shared_ptr<IComputeVertexBuffer> lComputeVertexBuffer = std::make_shared<ComputeVertexBuffer>();
	lComputeVertexBuffer->Create(length_, singleSize_, data_);
	return lComputeVertexBuffer;
}