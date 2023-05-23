#include"RWStructuredBuffer.h"

void RWStructuredBuffer::CreateSRV(size_t length, size_t singleSize, const void* data)
{
	bufferLength = length;
	bufferSingleSize = singleSize;

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(singleSize * length);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;
	HRESULT result = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(result))
	{
		printf("Failed to create buffer resource");
		return;
	}

	// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
	result = resource->Map(0, nullptr, reinterpret_cast<void**>(&BufferMappedPtr));
	if (FAILED(result))
	{
		printf("Buffer mapping failed");
		return;
	}

	if (data != nullptr)
	{
		memcpy(BufferMappedPtr, data, static_cast<size_t>(singleSize * length));
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//バッファとして
	srvDesc.Buffer.NumElements = static_cast<UINT>(length);//要素の総数
	srvDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize);//1個当たりの大きさ
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	structuredBufferHandle.ptr = SRVHeap->CreateSRV(srvDesc, resource.Get());

	isValid = true;
}

void RWStructuredBuffer::CreateUAV(size_t length, size_t singleSize, const void* data)
{
	bufferLength = length;
	bufferSingleSize = singleSize;
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(singleSize * length);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;
	HRESULT result = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(result))
	{
		printf("Failed to create buffer resource");
		return;
	}

	// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
	resource->Map(0, nullptr, (void**)&BufferMappedPtr);
	if (FAILED(result))
	{
		printf("Buffer mapping failed");
		return;
	}
	if (data)
	{
		memcpy(BufferMappedPtr, data, static_cast<size_t>(singleSize * length));
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = static_cast<UINT>(length);
	uavDesc.Buffer.StructureByteStride = static_cast<UINT>(singleSize);

	structuredBufferHandle.ptr = SRVHeap->CreateUAV(uavDesc, resource.Get());

	isValid = true;
}

void RWStructuredBuffer::CreateUAV(CD3DX12_RESOURCE_DESC* texresDesc)
{
	texresDesc->Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;
	HRESULT result = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		texresDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	if (FAILED(result))
	{
		printf("Failed to create buffer resource");
		return;
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;//テクスチャとして　
	uavDesc.Texture2D.MipSlice = 0;
	uavDesc.Texture2D.PlaneSlice = 0;

	structuredBufferHandle.ptr = SRVHeap->CreateUAV(uavDesc, resource.Get());

	isValid = true;
}

void RWStructuredBuffer::Update(void* data)
{
	void* ptr_ = nullptr;
	HRESULT result = resource->Map(0, nullptr, &ptr_);
	if (FAILED(result))
	{
		printf("Buffer mapping failed");
		return;
	}

	// 頂点データをマッピング先に設定
	memcpy(ptr_, data, bufferSingleSize * bufferLength);

	// マッピング解除
	resource->Unmap(0, nullptr);
}

bool RWStructuredBuffer::IsValid()
{
	return isValid;
}

D3D12_GPU_DESCRIPTOR_HANDLE& RWStructuredBuffer::GetAddress()
{
	return structuredBufferHandle;
}

void* RWStructuredBuffer::GetPtr()const
{
	return BufferMappedPtr;
}

D3D12_SHADER_RESOURCE_VIEW_DESC RWStructuredBuffer::GetViewDesc()
{
	return D3D12_SHADER_RESOURCE_VIEW_DESC();
}

ID3D12Resource* RWStructuredBuffer::GetResource()
{
	return resource.Get();
}