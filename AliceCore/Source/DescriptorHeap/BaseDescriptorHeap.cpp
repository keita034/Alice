#include "BaseDescriptorHeap.h"

ID3D12Device* BaseDescriptorHeap::device = nullptr;

void BaseDescriptorHeap::SetDevice(ID3D12Device* dev)
{
	if (!device)
	{
		device = dev;
	}

}
