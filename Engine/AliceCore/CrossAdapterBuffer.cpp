#include "CrossAdapterBuffer.h"

void CrossAdapterBuffer::Create(size_t length_,size_t singleSize_)
{
	CD3DX12_HEAP_DESC lHeapDesc = CD3DX12_HEAP_DESC(singleSize_ * length_,D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT,0,D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER);


}
