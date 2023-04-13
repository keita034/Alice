#include "ComputeVertexBuffer.h"

void ComputeVertexBuffer::Create(size_t length, size_t singleSize, const void* data)
{
	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.CreationNodeMask = 1;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.VisibleNodeMask = 1;

	// リソースの設定
	D3D12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(singleSize * length);
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	// リソースを生成
	HRESULT result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));

	if (FAILED(result))
	{
		printf("Failed to create vertex buffer resource");
		return;
	}

	// 頂点バッファビューの設定
	vertexBufferView.BufferLocation = resource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = static_cast<UINT>(singleSize * length);
	vertexBufferView.StrideInBytes = static_cast<UINT>(singleSize);

	result = resource->Map(0, nullptr, &bufferMappedPtr);
	if (FAILED(result))
	{
		printf("Vertex buffer mapping failed");
		return;
	}

	// マッピングする
	if (data != nullptr)
	{
		// 頂点データをマッピング先に設定
		memcpy(bufferMappedPtr, data, singleSize * length);
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = static_cast<UINT>(length);
	uavDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize);

	handl.ptr = SRVHeap->CreateUAV(uavDesc, resource.Get());

	isValid = true;

	resource->Unmap(0, nullptr);
}

void ComputeVertexBuffer::Update(void* data)
{
	if (data == nullptr)
	{
		return;
	}

	HRESULT result = resource->Map(0, nullptr, &bufferMappedPtr);

	if (FAILED(result))
	{
		printf("Vertex buffer mapping failed");
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(bufferMappedPtr, data, vertexBufferView.SizeInBytes);

	resource->Unmap(0, nullptr);
}

D3D12_VERTEX_BUFFER_VIEW ComputeVertexBuffer::GetView() const
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

void ComputeVertexBuffer::Transition(D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), beforeState, afterState);
	commandList->ResourceBarrier(1, &barrier);
}

D3D12_GPU_DESCRIPTOR_HANDLE ComputeVertexBuffer::GetAddress() const
{
	return handl;
}