#include "GPUTimeStamp.h"

void GPUTimeStamp::Initialize(IMultiAdapters* adapters_)
{
	D3D12_QUERY_HEAP_DESC  lDesc{};
	lDesc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
	lDesc.Count = QUERY_COUNT * FRAME_COUNT;

	D3D12_RESOURCE_DESC desc;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = sizeof(uint64_t) * QUERY_COUNT * FRAME_COUNT;
	desc.Height = 1;
	desc.MipLevels = 1;
	desc.DepthOrArraySize = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES heap;
	heap.Type = D3D12_HEAP_TYPE_READBACK;

	{
		IDevice* mainDev = adapters_->GetMainAdapter()->GetDevice();
		mainDev->Get()->CreateQueryHeap(&lDesc,IID_PPV_ARGS(&mainHeap));

		mainDev->Get()->CreateCommittedResource(&heap,D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,&desc,D3D12_RESOURCE_STATE_COPY_DEST,nullptr,IID_PPV_ARGS(&mainResource));

		mainDev->Get()->SetStablePowerState(true);
	}



}

void GPUTimeStamp::Update()
{
}

void GPUTimeStamp::Finalize()
{
}

void GPUTimeStamp::Draw()
{
}
