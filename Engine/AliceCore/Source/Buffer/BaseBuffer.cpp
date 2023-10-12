#include "BaseBuffer.h"

IMultiAdapters* BaseBuffer::sMultiAdapters = nullptr;

void BaseBuffer::SSetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	if (!sMultiAdapters)
	{
		sMultiAdapters = multiAdapters_;
	}
}

IDevice* BaseBuffer::SGetDevice(AdaptersIndex index_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(index_);
	return lAdapter->GetDevice();
}

ICommandList* BaseBuffer::SGetGraphicsCommandList(AdaptersIndex index_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(index_);
	return lAdapter->GetCommandList();
}

ISRVDescriptorHeap* BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex index_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(index_);
	return lAdapter->GetSRVDescriptorHeap();
}

IRTVDescriptorHeap* BaseBuffer::SGetRTVDescriptorHeap(AdaptersIndex index_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(index_);
	return lAdapter->GetRTVDescriptorHeap();
}

IDSVDescriptorHeap* BaseBuffer::SGetDSVDescriptorHeap(AdaptersIndex index_)
{
	IAdapter* lAdapter = sMultiAdapters->GetAdapter(index_);
	return lAdapter->GetDSVDescriptorHeap();
}

void BaseBuffer::Finalize()
{
	sMultiAdapters = nullptr;
}