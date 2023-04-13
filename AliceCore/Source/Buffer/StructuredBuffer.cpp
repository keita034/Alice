#include "StructuredBuffer.h"

void StructuredBuffer::Create(size_t length, size_t singleSize, const void* data)
{
	bufferLength = length;
	bufferSingleSize = singleSize;

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSingleSize * length,D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;
	device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(resource.GetAddressOf())
	);

	// 構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
	resource->Map(0, nullptr, reinterpret_cast<void**>(&BufferMappedPtr));
	if (data != nullptr)
	{
		memcpy(BufferMappedPtr, data, static_cast<size_t>(singleSize * length));
	}

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

void StructuredBuffer::Update(void* data)
{
	if (data)
	{
		// 頂点データをマッピング先に設定
		memcpy(BufferMappedPtr, data, static_cast<size_t>(bufferSingleSize * bufferLength));
	}
}

bool StructuredBuffer::IsValid()
{
	return isValid;
}

D3D12_GPU_DESCRIPTOR_HANDLE StructuredBuffer::GetAddress() const
{
	return structuredBufferHandle;
}

D3D12_SHADER_RESOURCE_VIEW_DESC StructuredBuffer::GetViewDesc()
{
	return D3D12_SHADER_RESOURCE_VIEW_DESC();
}

void* StructuredBuffer::GetPtr()const
{
	return BufferMappedPtr;
}