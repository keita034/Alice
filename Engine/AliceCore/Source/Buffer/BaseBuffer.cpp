#include "BaseBuffer.h"

IDevice* BaseBuffer::sDevice = nullptr;
ISRVDescriptorHeap* BaseBuffer::sSRVHeap = nullptr;
IRTVDescriptorHeap* BaseBuffer::sRTVHeap = nullptr;
IDSVDescriptorHeap* BaseBuffer::sDSVHeap = nullptr;
ICommandList* BaseBuffer::sCommandList = nullptr;

void BaseBuffer::SSetDevice(IDevice* dev)
{
	if (!sDevice)
	{
		sDevice = dev;
	}
}

void BaseBuffer::SSetGraphicsCommandList(ICommandList* commandList_)
{
	if (!sCommandList)
	{
		sCommandList = commandList_;
	}
}

void BaseBuffer::SSetSRVDescriptorHeap(ISRVDescriptorHeap* descriptorHeap_)
{
	if (!sSRVHeap)
	{
		sSRVHeap = descriptorHeap_;
	}
}

void BaseBuffer::SSetRTVDescriptorHeap(IRTVDescriptorHeap* rtvDescriptorHeap_)
{
	if (!sRTVHeap)
	{
		sRTVHeap = rtvDescriptorHeap_;
	}
}

void BaseBuffer::SSetDSVDescriptorHeap(IDSVDescriptorHeap* dsvDescriptorHeap_)
{
	if (!sDSVHeap)
	{
		sDSVHeap = dsvDescriptorHeap_;
	}
}

void BaseBuffer::Finalize()
{
	sDSVHeap = nullptr;
	sRTVHeap = nullptr;
	sSRVHeap = nullptr;
	sCommandList = nullptr;
	sDevice = nullptr;
}