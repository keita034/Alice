#pragma once

#include<Adapter.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

struct ID3D12Fence;
struct ID3D12CommandQueue;

class ICrossAdapterFence
{
public:

	virtual void Initialize(IAdapter* mainAdapter_,IAdapter* subAdapter_) = 0;

	virtual ID3D12Fence* Get(size_t index_) = 0;

	virtual void Wait(ID3D12CommandQueue* queue_,size_t index_) = 0;

	virtual void Signal(ID3D12CommandQueue* queue_,size_t index_) = 0;

	virtual void WaitForGpu(AdaptersIndex index_) = 0;

	ICrossAdapterFence() = default;
	virtual ~ICrossAdapterFence() = default;
};

std::unique_ptr<ICrossAdapterFence> CreateUniqueCrossAdapterFence(IAdapter* mainAdapter_,IAdapter* subAdapter_);

std::shared_ptr<ICrossAdapterFence> CreateSharedCrossAdapterFence(IAdapter* mainAdapter_,IAdapter* subAdapter_);
