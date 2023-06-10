#include "BaseDescriptorHeap.h"

ID3D12Device* BaseDescriptorHeap::sDevice = nullptr;

void BaseDescriptorHeap::SSetDevice(ID3D12Device* device_)
{
	if (!sDevice)
	{
		sDevice = device_;
	}
}
