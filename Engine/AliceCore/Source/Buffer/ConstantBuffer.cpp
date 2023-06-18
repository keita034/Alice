#include "ConstantBuffer.h"

#include"BaseBuffer.h"

/// <summary>
/// 定数バッファ
/// </summary>
class ConstantBuffer : public BaseBuffer, public IConstantBuffer
{

private:

	// 定数バッファビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC constantBufferView = {};

	//マップ用ポインタ
	void* bufferMappedPtr = nullptr;

	//バッファサイズ
	size_t bufferSize;

public:

	/// <summary>
	/// 定数バッファを生成
	/// </summary>
	void Create(size_t bufferSize_) override;

   /// <summary>
   /// バッファ生成に成功したかを返す
   /// </summary>
	bool IsValid() override;

   /// <summary>
   /// バッファのGPU上のアドレスを返す
   /// </summary>
	const D3D12_GPU_VIRTUAL_ADDRESS& GetAddress() override;

   /// <summary>
   /// 定数バッファビューを返す
   /// </summary>
	const D3D12_CONSTANT_BUFFER_VIEW_DESC& GetViewDesc() override;

   /// <summary>
   /// データの更新
   /// </summary>
	void Update(void* data_) override;

   /// <summary>
   /// バッファを取得
   /// </summary>
	ID3D12Resource* GetResource() override;

   /// <summary>
   /// マップ用ポインタを取得
   /// </summary>
	void* GetPtr() override;

	~ConstantBuffer() = default;
	ConstantBuffer() = default;


private:

	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator = (const ConstantBuffer&) = delete;

};

void ConstantBuffer::Create(size_t bufferSize_)
{
	if (!isValid)
	{
		bufferSize = bufferSize_;

		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		// リソースの設定
		CD3DX12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer((bufferSize + 0xff) & ~0xff);

		// リソースを生成
		HRESULT lResult = sDevice->CreateCommittedResource(
			&lHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&lResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

		if (FAILED(lResult))
		{
			printf("Failed to create constant buffer resource\n");
			return;
		}

		lResult = resource->Map(0, nullptr, &bufferMappedPtr);

		if (FAILED(lResult))
		{
			printf("Failed to map constant buffer\n");
			return;
		}

		constantBufferView = {};
		constantBufferView.BufferLocation = resource->GetGPUVirtualAddress();
		constantBufferView.SizeInBytes = static_cast<UINT>(lResDesc.Width);

		sSRVHeap->CreateCBV(constantBufferView);

		isValid = true;
	}
}

bool ConstantBuffer::IsValid()
{
	return isValid;
}

const D3D12_GPU_VIRTUAL_ADDRESS& ConstantBuffer::GetAddress()
{
	return constantBufferView.BufferLocation;
}

const D3D12_CONSTANT_BUFFER_VIEW_DESC& ConstantBuffer::GetViewDesc()
{
	return constantBufferView;
}

void ConstantBuffer::Update(void* data)
{
	if (data == nullptr)
	{
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr, data, bufferSize);
}

ID3D12Resource* ConstantBuffer::GetResource()
{
	return resource.Get();
}

void* ConstantBuffer::GetPtr()
{
	return bufferMappedPtr;
}

std::unique_ptr<IConstantBuffer> CreateUniqueConstantBuffer(size_t bufferSize_)
{
	std::unique_ptr<IConstantBuffer> lConstantBuffer = std::make_unique<ConstantBuffer>();
	lConstantBuffer->Create(bufferSize_);
	return std::move(lConstantBuffer);
}

std::shared_ptr<IConstantBuffer> CreateSharedConstantBuffer(size_t bufferSize_)
{
	std::shared_ptr<IConstantBuffer> lConstantBuffer = std::make_shared<ConstantBuffer>();
	lConstantBuffer->Create(bufferSize_);
	return lConstantBuffer;
}