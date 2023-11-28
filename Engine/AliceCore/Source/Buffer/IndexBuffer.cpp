#include "IndexBuffer.h"

#include<BaseBuffer.h>
#include<StringUtility.h>
#include<DescriptorHeap.h>

/// <summary>
/// インデックスバッファ
/// </summary>
class IndexBuffer : public BaseBuffer,public IIndexBuffer
{
private:

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW bufferView{};

	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle;

	//マップ用ポインタ
	void* bufferMappedPtr = nullptr;

	//バッファの長さ
	size_t bufferlength = 0;

	AdaptersIndex index;

private:

	Byte4 PADING;
public:

	/// <summary>
	/// インデックスバッファを生成
	/// </summary>
	/// <param name="length_">インデックスバッファの要素数</param>
	/// <param name="data">インデックス配列の先頭アドレス(uint32_t)</param>
	void Create(size_t length_,AdaptersIndex index_,const uint32_t* data_) override;

	/// <summary>
	/// 成功したか
	/// </summary>
	bool IsValid()override;

	/// <summary>
	/// インデックスバッファビューを取得
	/// </summary>
	const D3D12_INDEX_BUFFER_VIEW& GetView()override;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	void Update(void* data_)override;

	/// <summary>
	/// データの更新
	/// </summary>
	/// <param name="data">データ</param>
	/// <param name="length">データの長さ</param>
	void Update(void* data_,size_t length_)override;

	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="name_">名前</param>
	void SetName(const std::string& name_)override;

	void CreateSRV();

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetSRVAddress();

	IndexBuffer() = default;
	~IndexBuffer() = default;

private:

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator = (const IndexBuffer&) = delete;
};

void IndexBuffer::Create(size_t length_,AdaptersIndex index_,const uint32_t* data)
{
	if ( !isValid )
	{
		bufferlength = length_;
		index = index_;

		IAdapter* lAdapter = sMultiAdapters->GetAdapter(index_);
		ID3D12Device* lDevice = lAdapter->GetDevice()->Get();

		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		// リソースの設定
		CD3DX12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Buffer(length_ * sizeof(uint32_t));	// リソースの設定

		// リソースを生成
		HRESULT lResult = lDevice->CreateCommittedResource(
			&lHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&lResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
		if ( FAILED(lResult) )
		{
			printf("Failed to create index buffer resource");
			return;
		}

		resource->SetName(L"IndexBuffer");

		// インデックスバッファビューの設定
		bufferView = {};
		bufferView.BufferLocation = resource->GetGPUVirtualAddress();
		bufferView.Format = DXGI_FORMAT_R32_UINT;
		bufferView.SizeInBytes = static_cast< UINT >( length_ * sizeof(uint32_t) );

		lResult = resource->Map(0,nullptr,&bufferMappedPtr);
		if ( FAILED(lResult) )
		{
			printf("Index buffer mapping failed");
			return;
		}

		// マッピングする
		if ( data != nullptr )
		{
			// インデックスデータをマッピング先に設定
			memcpy(bufferMappedPtr,data,length_ * sizeof(uint32_t));

			// マッピング解除
			resource->Unmap(0,nullptr);
		}

		isValid = true;
	}
}

bool IndexBuffer::IsValid()
{
	return isValid;
}

const D3D12_INDEX_BUFFER_VIEW& IndexBuffer::GetView()
{
	return bufferView;
}

void IndexBuffer::Update(void* data)
{
	memcpy(bufferMappedPtr,data,bufferlength * sizeof(uint32_t));
}


void IndexBuffer::Update(void* data_,size_t length_)
{
	memcpy(bufferMappedPtr,data_,length_ * sizeof(uint32_t));
}

void IndexBuffer::SetName(const std::string& name_)
{
	resource->SetName(AliceUtility::String::StringToWstring(name_).c_str());
}

void IndexBuffer::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC lSrvResDesc = {};
	lSrvResDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	lSrvResDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	lSrvResDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
	lSrvResDesc.Buffer.FirstElement = 0;
	lSrvResDesc.Buffer.NumElements = static_cast< uint32_t > ( bufferlength );
	lSrvResDesc.Buffer.StructureByteStride = static_cast< uint32_t > ( sizeof(uint32_t) );

	IAdapter* lAdapter = sMultiAdapters->GetAdapter(index);
	ISRVDescriptorHeap* lSRVHeap = lAdapter->GetSRVDescriptorHeap();

	srvHandle.ptr = lSRVHeap->CreateSRV(lSrvResDesc,resource.Get());
}

const D3D12_GPU_DESCRIPTOR_HANDLE& IndexBuffer::GetSRVAddress()
{
	return srvHandle;
}

std::unique_ptr<IIndexBuffer> CreateUniqueIndexBuffer(size_t length_,AdaptersIndex index_,const uint32_t* data_)
{
	std::unique_ptr<IIndexBuffer> lIndexBuffer = std::make_unique<IndexBuffer>();
	lIndexBuffer->Create(length_,index_,data_);
	return std::move(lIndexBuffer);
}

std::shared_ptr<IIndexBuffer> CreateSharedIndexBuffer(size_t length_,AdaptersIndex index_,const uint32_t* data_)
{
	std::shared_ptr<IIndexBuffer> lIndexBuffer = std::make_shared<IndexBuffer>();
	lIndexBuffer->Create(length_,index_,data_);
	return lIndexBuffer;
}
