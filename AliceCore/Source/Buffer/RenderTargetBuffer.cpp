#include "RenderTargetBuffer.h"

bool RenderTargetBuffer::Create(UINT w, UINT h, D3D12_RESOURCE_STATES resourceStates, UINT16 mipLevel, UINT16 arraySize, DXGI_FORMAT format, const std::array<float, 4>& clearColor)
{
	width = w;
	height = h;
	states = resourceStates;

	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(
		format,
		w, h,
		arraySize, mipLevel, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_CLEAR_VALUE cleaVal = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor.data());

	//リソースを作成。
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);;

	HRESULT hr = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		resourceStates,
		&cleaVal,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "レンダーターゲットバッファの作成に失敗", "エラー", MB_OK);
		return false;
	}

	//ターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//計算結果をSRGBに変換
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	handle.ptr = RTVHeap->CreateRTV(rtvDesc, resource.Get());

	return true;
}

bool RenderTargetBuffer::Create(IDXGISwapChain4* swapChain, UINT index)
{
	//バッファを取得
	swapChain->GetBuffer(index, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

	//ターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//計算結果をSRGBに変換
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	handle.ptr = RTVHeap->CreateRTV(rtvDesc, resource.Get());

	width = static_cast<UINT>(resource->GetDesc().Height);
	height = static_cast<UINT>(resource->GetDesc().Width);

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

void RenderTargetBuffer::Transition(D3D12_RESOURCE_STATES resourceStates)
{
	if (states != resourceStates)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), states, resourceStates);
		commandList->ResourceBarrier(1, &barrier);
		states = resourceStates;
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

void RenderTargetBuffer::Resize(IDXGISwapChain4* swapChain, UINT index)
{

	//バッファを取得
	swapChain->GetBuffer(index, IID_PPV_ARGS(resource.GetAddressOf()));

	//ターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//計算結果をSRGBに変換
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(resource.Get(), &rtvDesc, handle);

	width = static_cast<UINT>(resource->GetDesc().Height);
	height = static_cast<UINT>(resource->GetDesc().Width);
}


