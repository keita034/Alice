#include "DepthStencilBuffer.h"

bool DepthStencilBuffer::Create(UINT w, UINT h, DXGI_FORMAT format)
{
	height = h;
	width = w;
	
	//クリアビュー
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	//リソースディスク
	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		w,h,
		1,1,
		format,
		1,0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	//ヒーププロップ
	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//深度バッファ作成
	HRESULT hr = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	//ビューディスク
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//DSV制作
	handle.ptr = DSVHeap->CreateDSV(dsvDesc, resource.Get());

	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "深度ステンシルバッファの作成に失敗", "エラー", MB_OK);
		return false;
	}

	return true;

}

bool DepthStencilBuffer::Resize(UINT w, UINT h)
{
	//開放
	resource.Reset();

	//幅
	height = h;
	width = w;

	//クリアビュー
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	//リソースディスク
	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		w, h,
		1, 1,
		DXGI_FORMAT_D32_FLOAT,
		1, 0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	//ヒーププロップ
	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//深度バッファ作成
	HRESULT hr = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	//ビューディスク
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//DSV制作
	device->CreateDepthStencilView(resource.Get(), &dsvDesc, handle);

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
