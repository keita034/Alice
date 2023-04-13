#include "BaseBuffer.h"

ID3D12Device* BaseBuffer::device = nullptr;
DescriptorHeap* BaseBuffer::SRVHeap = nullptr;
RTVDescriptorHeap* BaseBuffer::RTVHeap = nullptr;
DSVDescriptorHeap* BaseBuffer::DSVHeap = nullptr;
ID3D12GraphicsCommandList* BaseBuffer::commandList = nullptr;

void BaseBuffer::SetDevice(ID3D12Device* dev)
{
	if (!device)
	{
		device = dev;
	}
}

void BaseBuffer::SetGraphicsCommandList(ID3D12GraphicsCommandList* cmdList)
{
	if (!commandList)
	{
		commandList = cmdList;
	}
}

void BaseBuffer::SetSRVDescriptorHeap(DescriptorHeap* srv)
{
	if (!SRVHeap)
	{
		SRVHeap = srv;
	}
}

void BaseBuffer::SetRTVDescriptorHeap(RTVDescriptorHeap* rtv)
{
	if (!RTVHeap)
	{
		RTVHeap = rtv;
	}
}

void BaseBuffer::SetDSVDescriptorHeap(DSVDescriptorHeap* dsv)
{
	if (!DSVHeap)
	{
		DSVHeap = dsv;
	}
}
