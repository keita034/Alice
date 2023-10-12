#include "VertexBuffer.h"

#include<BaseBuffer.h>
#include<StringUtility.h>

/// <summary>
/// 頂点バッファ
/// </summary>
class VertexBuffer : public BaseBuffer , public IVertexBuffer
{
private:

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	//マッピングのポインター
	void* bufferMappedPtr = nullptr;

public:

	/// <summary>
	/// バッファを生成
	/// </summary>
	void Create(size_t length_, size_t singleSize_, const void* data_)override;

	/// <summary>
	/// 頂点バッファビューを取得
	/// </summary>
	const D3D12_VERTEX_BUFFER_VIEW& GetView() override;

	/// <summary>
	/// バッファを取得
	/// </summary>
	ID3D12Resource* GetResource()override;

	/// <summary>
	/// バッファの生成に成功したかを取得
	/// </summary>
	bool IsValid()override;

	/// <summary>
	/// データの更新
	/// </summary>
	void Update(void* data_)override;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	/// <param name="length_">データの数</param>
	void Update(void* data_,size_t length_)override;

	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name_">名前</param>
	void SetName(const std::string& name_)override;

	~VertexBuffer() = default;
	VertexBuffer() = default;

private:

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator = (const VertexBuffer&) = delete;
};


void VertexBuffer::Create(size_t length_, size_t singleSize_, const void* data_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(AdaptersIndex::MAIN);
	ID3D12Device* lDevice = lAdapter->GetDevice()->Get();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// リソースの設定
	CD3DX12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(length_ * singleSize_);

	// リソースを生成
	HRESULT lResult = lDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

	if (FAILED(lResult))
	{
		printf("Failed to create vertex buffer resource");
		return;
	}

	// 頂点バッファビューの設定
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = static_cast<UINT>(length_ * singleSize_);
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
		memcpy(bufferMappedPtr, data_, length_ * singleSize_);
	}

	resource->SetName(L"VertexBuffer");
	isValid = true;
}

void VertexBuffer::Update(void* data_)
{
	if (data_ == nullptr)
	{
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr, data_, vertexBufferView.SizeInBytes);
}

void VertexBuffer::Update(void* data_,size_t length_)
{
	if ( data_ == nullptr )
	{
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr,data_,vertexBufferView.StrideInBytes * length_);
}

const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::GetView()
{
	return vertexBufferView;
}

bool VertexBuffer::IsValid()
{
	return isValid;
}

ID3D12Resource* VertexBuffer::GetResource()
{
	return resource.Get();
}

void VertexBuffer::SetName(const std::string& name_)
{
	resource->SetName(AliceUtility::String::StringToWstring(name_).c_str());
}

std::unique_ptr<IVertexBuffer> CreateUniqueVertexBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::unique_ptr<IVertexBuffer> lVertexBuffer = std::make_unique<VertexBuffer>();
	lVertexBuffer->Create(length_, singleSize_, data_);
	return std::move(lVertexBuffer);
}

std::shared_ptr<IVertexBuffer> CreateSharedVertexBuffer(size_t length_, size_t singleSize_, const void* data_)
{
	std::shared_ptr<IVertexBuffer> lVertexBuffer = std::make_shared<VertexBuffer>();
	lVertexBuffer->Create(length_, singleSize_, data_);
	return lVertexBuffer;
}
