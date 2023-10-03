#include "StructuredBuffer.h"

#include"BaseBuffer.h"

/// <summary>
/// 読み取り専用構造化バッファ
/// </summary>
class StructuredBuffer : public BaseBuffer, public IStructuredBuffer
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

public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	/// <param name="length_">要素数</param>
	/// <param name="singleSize_">単体のサイズ</param>
	/// <param name="data_">配列の先頭アドレス</param>
	void Create(size_t length_, size_t singleSize_, const void* data_) override;

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
	/// バッファビューを返す
	/// </summary>
	/// <returns>バッファビュー</returns>
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetViewDesc() override;

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

	~StructuredBuffer() = default;
	StructuredBuffer() = default;

private:

	StructuredBuffer(const StructuredBuffer&) = delete;
	StructuredBuffer& operator = (const StructuredBuffer&) = delete;

};

void StructuredBuffer::Create(size_t length_, size_t singleSize_, const void* data_)
{
	if(!isValid)
	{
		bufferlength = length_;
		bufferSingleSize = singleSize_;

		D3D12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSingleSize * length_, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		D3D12_HEAP_PROPERTIES lHeapProp{};
		lHeapProp.Type = D3D12_HEAP_TYPE:: D3D12_HEAP_TYPE_UPLOAD;
		lHeapProp.VisibleNodeMask = 1;
		ID3D12Device* lDevice = sDevice->Get();
		lDevice->CreateCommittedResource(
			&lHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&lResDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(resource.GetAddressOf())
		);

		// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
		resource->Map(0, nullptr, reinterpret_cast<void**>(&bufferMappedPtr));
		if (data_ != nullptr)
		{
			memcpy(bufferMappedPtr, data_, static_cast<size_t>(singleSize_ * length_));
		}

		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//バッファとして
		srvDesc.Buffer.NumElements = static_cast<UINT>(length_);//要素の総数
		srvDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize_);//1個当たりの大きさ
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		structuredBufferHandle.ptr = sSRVHeap->CreateSRV(srvDesc, resource.Get());
	}

	isValid = true;
}

void StructuredBuffer::Update(void* data_)
{
	if (data_)
	{
		// 頂点データをマッピング先に設定
		memcpy(bufferMappedPtr, data_, static_cast<size_t>(bufferSingleSize * bufferlength));
	}
}

bool StructuredBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& StructuredBuffer::GetAddress()
{
	return structuredBufferHandle;
}

const D3D12_SHADER_RESOURCE_VIEW_DESC& StructuredBuffer::GetViewDesc()
{
	return srvDesc;
}

void* StructuredBuffer::GetPtr()const
{
	return bufferMappedPtr;
}

std::unique_ptr<IStructuredBuffer> CreateUniqueStructuredBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::unique_ptr<IStructuredBuffer> lStructuredBuffer = std::make_unique<StructuredBuffer>();
	lStructuredBuffer->Create(length_, singleSize_, data_);
	return std::move(lStructuredBuffer);
}

std::shared_ptr<IStructuredBuffer> CreateSharedStructuredBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::shared_ptr<IStructuredBuffer> lStructuredBuffer = std::make_shared<StructuredBuffer>();
	lStructuredBuffer->Create(length_, singleSize_, data_);
	return lStructuredBuffer;
}
