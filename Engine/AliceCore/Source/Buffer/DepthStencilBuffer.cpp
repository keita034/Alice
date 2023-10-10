#include "DepthStencilBuffer.h"

#include"BaseBuffer.h"
#include<DSVDescriptorHeap.h>

class DepthStencilBuffer : public BaseBuffer,public IDepthStencilBuffer
{
private:

	//幅
	uint32_t width = 0;
	//高さ
	uint32_t height = 0;
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};

public:

	/// <summary>
	/// 生成
	/// </summary>
	bool Create(uint32_t width_, uint32_t height_, DXGI_FORMAT format_) override;

	/// <summary>
	/// サイズ変更
	/// </summary>
	bool Resize(uint32_t width_, uint32_t height_) override;

	/// <summary>
	/// リソースを取得
	/// </summary>
	ID3D12Resource* GetTexture() const override;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle() override;

	~DepthStencilBuffer() = default;
	DepthStencilBuffer() = default;
};

bool DepthStencilBuffer::Resize(uint32_t width_, uint32_t height_)
{
	//開放
	resource->Release();

	IAdapter* lAdapter = sMultiAdapters->GetAdapter(AdaptersIndex::MAIN);
	ID3D12Device* lDevice = lAdapter->GetDevice()->Get();

	//幅
	height = height_;
	width = width_;

	//クリアビュー
	D3D12_CLEAR_VALUE lClearValue{};
	lClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	lClearValue.DepthStencil.Depth = 1.0f;
	lClearValue.DepthStencil.Stencil = 0;

	//リソースディスク
	CD3DX12_RESOURCE_DESC lResDesc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		static_cast<UINT64>(width), static_cast<UINT>(height),
		1, 1,
		DXGI_FORMAT_D32_FLOAT,
		1, 0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	//ヒーププロップ
	CD3DX12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//深度バッファ作成
	HRESULT hr = lDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&lClearValue,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	//ビューディスク
	D3D12_DEPTH_STENCIL_VIEW_DESC lDsvResDesc = {};
	lDsvResDesc.Format = DXGI_FORMAT_D32_FLOAT;
	lDsvResDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//DSV制作
	lDevice->CreateDepthStencilView(resource.Get(), &lDsvResDesc, handle);

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "深度ステンシルバッファの作成に失敗", "エラー", MB_OK);
		return false;
	}

	return true;
}

bool DepthStencilBuffer::Create(uint32_t width_, uint32_t height_, DXGI_FORMAT format_)
{
	height = height_;
	width = width_;

	IAdapter* lAdapter = sMultiAdapters->GetAdapter(AdaptersIndex::MAIN);
	ID3D12Device* lDevice = lAdapter->GetDevice()->Get();
	IDSVDescriptorHeap* lDSVHeap = lAdapter->GetDSVDescriptorHeap();

	//クリアビュー
	D3D12_CLEAR_VALUE lClearValue{};
	lClearValue.Format = format_;
	lClearValue.DepthStencil.Depth = 1.0f;
	lClearValue.DepthStencil.Stencil = 0;

	//リソースディスク
	CD3DX12_RESOURCE_DESC lResDesc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		static_cast<UINT64>(width), static_cast<UINT>(height),
		1, 1,
		format_,
		1, 0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	//ヒーププロップ
	CD3DX12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//深度バッファ作成
	HRESULT hr = lDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&lClearValue,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	//ビューディスク
	D3D12_DEPTH_STENCIL_VIEW_DESC lDsvResDesc = {};
	lDsvResDesc.Format = format_;
	lDsvResDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//DSV制作
	handle.ptr = lDSVHeap->CreateDSV(lDsvResDesc, resource.Get());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "深度ステンシルバッファの作成に失敗", "エラー", MB_OK);
		return false;
	}

	return true;
}

ID3D12Resource* DepthStencilBuffer::GetTexture() const
{
	return resource.Get();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& DepthStencilBuffer::GetHandle()
{
	return handle;
}

std::unique_ptr<IDepthStencilBuffer> CreateUniqueDepthStencilBuffer(uint32_t width_, uint32_t height_, DXGI_FORMAT format_)
{
	std::unique_ptr<IDepthStencilBuffer> lDepthStencilBuffer = std::make_unique<DepthStencilBuffer>();
	lDepthStencilBuffer->Create(width_, height_, format_);
	return std::move(lDepthStencilBuffer);
}

std::shared_ptr<IDepthStencilBuffer> CreateSharedDepthStencilBuffer(uint32_t width_, uint32_t height_, DXGI_FORMAT format_)
{
	std::shared_ptr<IDepthStencilBuffer> lDepthStencilBuffer = std::make_shared<DepthStencilBuffer>();
	lDepthStencilBuffer->Create(width_, height_, format_);
	return lDepthStencilBuffer;
}
