﻿#include "BaseBuffer.h"

ID3D12Device* BaseBuffer::sDevice = nullptr;
IDescriptorHeap* BaseBuffer::sSRVHeap = nullptr;
IRTVDescriptorHeap* BaseBuffer::sRTVHeap = nullptr;
IDSVDescriptorHeap* BaseBuffer::sDSVHeap = nullptr;
ID3D12GraphicsCommandList* BaseBuffer::sCommandList = nullptr;

void BaseBuffer::SSetDevice(ID3D12Device* dev)
{
	if (!sDevice)
	{
		sDevice = dev;
	}
}

void BaseBuffer::SSetGraphicsCommandList(ID3D12GraphicsCommandList* commandList_)
{
	if (!sCommandList)
	{
		sCommandList = commandList_;
	}
}

void BaseBuffer::SSetSRVDescriptorHeap(IDescriptorHeap* descriptorHeap_)
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