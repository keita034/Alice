#include "ConstantBuffer.h"

void ConstantBuffer::Create(size_t size)
{
	bufferSize = size;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// リソースの設定
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer((bufferSize + 0xff) & ~0xff);

	// リソースを生成
	HRESULT result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

	if (FAILED(result))
	{
		printf("Failed to create constant buffer resource\n");
		return;
	}

	result = resource->Map(0, nullptr, &bufferMappedPtr);

	if (FAILED(result))
	{
		printf("Failed to map constant buffer\n");
		return;
	}

	constantBufferView = {};
	constantBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	constantBufferView.SizeInBytes = static_cast<UINT>(resDesc.Width);

	SRVHeap->CreateCBV(constantBufferView);

	isValid = true;
}

bool ConstantBuffer::IsValid()
{
	return isValid;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetAddress() const
{
	return constantBufferView.BufferLocation;
}

D3D12_CONSTANT_BUFFER_VIEW_DESC ConstantBuffer::GetViewDesc()
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