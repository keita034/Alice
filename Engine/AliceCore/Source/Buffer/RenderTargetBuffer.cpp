#include "RenderTargetBuffer.h"

#include<cassert>

#include"BaseBuffer.h"

/// <summary>
/// レンダーターゲットバッファ
/// </summary>
class RenderTargetBuffer : public BaseBuffer, public IRenderTargetBuffer
{
private:

	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle{};
	//ステータス
	D3D12_RESOURCE_STATES states{};
	//幅
	uint32_t width = 0;
	//高さ
	uint32_t height = 0;
	int32_t PADING{};

public:

	/// <summary>
	/// 生成
	/// </summary>
	bool Create(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_, uint16_t arraySize_, DXGI_FORMAT format_, const std::array<float, 4>& clearColor_)override;

	/// <summary>
	/// 生成
	/// </summary>
	bool Create(IDXGISwapChain4* swapChain_, uint32_t index_)override;

	/// <summary>
	/// リソースを取得
	/// </summary>
	ID3D12Resource* GetTexture() const override;

	/// <summary>
	/// ポインタ番号を取得
	/// </summary>
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetHandle()override;

	/// <summary>
	/// リソースステータス変更
	/// </summary>
	void Transition(D3D12_RESOURCE_STATES resourceStates_)override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()override;

	void Resize(IDXGISwapChain4* swapChain_, uint32_t index_)override;

	~RenderTargetBuffer() = default;
	RenderTargetBuffer() = default;
};

bool RenderTargetBuffer::Create(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_, uint16_t arraySize_, DXGI_FORMAT format_, const std::array<float, 4>& clearColor_)
{
	width = width_;
	height = height_;
	states = resourceStates_;

	CD3DX12_RESOURCE_DESC lResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		format_,
		width, height,
		arraySize_, mipLevel_, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_CLEAR_VALUE lCleaVal = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor_.data());

	//リソースを作成。
	CD3DX12_HEAP_PROPERTIES lHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	HRESULT lResult = sDevice->CreateCommittedResource(
		&lHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&lResDesc,
		states,
		&lCleaVal,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(lResult))
	{
		MessageBoxA(nullptr, "レンダーターゲットバッファの作成に失敗", "エラー", MB_OK);
		return false;
	}

	//ターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC lRtvDesc{};
	
	//計算結果をSRGBに変換
	lRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	lRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	handle.ptr = sRTVHeap->CreateRTV(lRtvDesc, resource.Get());

	return true;
}

bool RenderTargetBuffer::Create(IDXGISwapChain4* swapChain_, uint32_t index_)
{
	//バッファを取得
	swapChain_->GetBuffer(index_, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

	//ターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC lRtvDesc{};
	//計算結果をSRGBに変換
	lRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	lRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	handle.ptr = sRTVHeap->CreateRTV(lRtvDesc, resource.Get());

	width = static_cast<uint32_t>(resource->GetDesc().Height);
	height = static_cast<uint32_t>(resource->GetDesc().Width);

	return true;
}

ID3D12Resource* RenderTargetBuffer::GetTexture() const
{
	return resource.Get();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& RenderTargetBuffer::GetHandle()
{
	return handle;
}

void RenderTargetBuffer::Transition(D3D12_RESOURCE_STATES resourceStates_)
{
	if (states != resourceStates_)
	{
		CD3DX12_RESOURCE_BARRIER lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), states, resourceStates_);
		sCommandList->ResourceBarrier(1, &lBarrier);
		states = resourceStates_;
	}
	else
	{
		assert(0);
	}
}

void RenderTargetBuffer::Reset()
{
	resource.Reset();
}

void RenderTargetBuffer::Resize(IDXGISwapChain4* swapChain_, uint32_t index_)
{
	//バッファを取得
	swapChain_->GetBuffer(index_, IID_PPV_ARGS(resource.GetAddressOf()));

	//ターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC lRtvDesc{};
	//計算結果をSRGBに変換
	lRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	lRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	sDevice->CreateRenderTargetView(resource.Get(), &lRtvDesc, handle);

	width = static_cast<uint32_t>(resource->GetDesc().Height);
	height = static_cast<uint32_t>(resource->GetDesc().Width);
}

std::unique_ptr<IRenderTargetBuffer> CreateUniqueRenderTargetBuffer(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_, uint16_t arraySize_, DXGI_FORMAT format_, const std::array<float, 4>& clearColor_)
{
	std::unique_ptr<IRenderTargetBuffer> lRenderTargetBuffer = std::make_unique<RenderTargetBuffer>();
	lRenderTargetBuffer->Create(width_, height_, resourceStates_, mipLevel_, arraySize_, format_, clearColor_);
	return std::move(lRenderTargetBuffer);
}

std::unique_ptr<IRenderTargetBuffer> CreateUniqueRenderTargetBuffer(IDXGISwapChain4* swapChain_, uint32_t index_)
{
	std::unique_ptr<IRenderTargetBuffer> lRenderTargetBuffer = std::make_unique<RenderTargetBuffer>();
	lRenderTargetBuffer->Create(swapChain_, index_);
	return std::move(lRenderTargetBuffer);
}

std::shared_ptr<IRenderTargetBuffer> CreateSharedRenderTargetBuffer(uint32_t width_, uint32_t height_, D3D12_RESOURCE_STATES resourceStates_, uint16_t mipLevel_, uint16_t arraySize_, DXGI_FORMAT format_, const std::array<float, 4>& clearColor_)
{
	std::shared_ptr<IRenderTargetBuffer> lRenderTargetBuffer = std::make_shared<RenderTargetBuffer>();
	lRenderTargetBuffer->Create(width_, height_, resourceStates_, mipLevel_, arraySize_, format_, clearColor_);
	return lRenderTargetBuffer;
}

std::shared_ptr<IRenderTargetBuffer> CreateSharedRenderTargetBuffer(IDXGISwapChain4* swapChain_, uint32_t index_)
{
	std::shared_ptr<IRenderTargetBuffer> lRenderTargetBuffer = std::make_shared<RenderTargetBuffer>();
	lRenderTargetBuffer->Create(swapChain_, index_);
	return lRenderTargetBuffer;
}
