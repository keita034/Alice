#include "CrossAdapterFence.h"

ALICE_SUPPRESS_WARNINGS_BEGIN
#include<wrl.h>
#include<vector>
#include<directx/d3dx12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<array>
ALICE_SUPPRESS_WARNINGS_END

class CrossAdapterFence : public ICrossAdapterFence
{
private:

	Microsoft::WRL::ComPtr<ID3D12Fence>fence;

	uint64_t value = 0;

public:

	virtual void Initialize(IAdapter* mainAdapter_)override;

	virtual ID3D12Fence* Get()override;

	virtual void Wait(ID3D12CommandQueue* queue_)override;

	virtual void Signal(ID3D12CommandQueue* queue_)override;

	CrossAdapterFence() = default;
	~CrossAdapterFence() = default;

};

void CrossAdapterFence::Initialize(IAdapter* mainAdapter_)
{
	ID3D12Device* lMainDevice = mainAdapter_->GetDevice()->Get();

	lMainDevice->CreateFence(0,D3D12_FENCE_FLAG_SHARED | D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER,IID_PPV_ARGS(&fence));
}

ID3D12Fence* CrossAdapterFence::Get()
{
	return fence.Get();
}

void CrossAdapterFence::Wait(ID3D12CommandQueue* queue_)
{
	queue_->Wait(fence.Get(),value);
}

void CrossAdapterFence::Signal(ID3D12CommandQueue* queue_)
{
	queue_->Signal(fence.Get(),++value);
}

std::unique_ptr<ICrossAdapterFence> CreateUniqueCrossAdapterFence(IAdapter* mainAdapter_)
{
	std::unique_ptr<ICrossAdapterFence> lCrossAdapterFence = std::make_unique<CrossAdapterFence>();
	lCrossAdapterFence->Initialize(mainAdapter_);
	return std::move(lCrossAdapterFence);
}

std::shared_ptr<ICrossAdapterFence> CreateSharedCrossAdapterFence(IAdapter* mainAdapter_)
{
	std::shared_ptr<ICrossAdapterFence> lCrossAdapterFence = std::make_shared<CrossAdapterFence>();
	lCrossAdapterFence->Initialize(mainAdapter_);
	return lCrossAdapterFence;
}
