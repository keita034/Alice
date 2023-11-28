#include "Fence.h"

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<wrl.h>
#include<vector>
#include<directx/d3dx12.h>
#include<dxgi1_6.h>
#include<cassert>

ALICE_SUPPRESS_WARNINGS_END

class Fence : public IFence
{
private:

	uint64_t fenceVal;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

public:

	Fence() = default;
	~Fence() = default;

	void Initialize(ID3D12Device* device_) override;
	void Signal(ID3D12CommandQueue* queue_,uint64_t value_)override;
	void Signal(ID3D12CommandQueue* queue_)override;
	void Wait()override;
	void Wait(ID3D12CommandQueue* queue_,IFence* fence,uint64_t Value)override;
	void CompulsionWait()override;

	uint64_t GetFenceValANDIncrement()override;
	uint64_t GetFenceVal()override;
	ID3D12Fence* Get() override;
};

void Fence::Initialize(ID3D12Device* device_)
{
	//フェンスの生成
	if (FAILED(device_->CreateFence(fenceVal,D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()))) )
	{
		printf("Failed to generate fence");
		assert(0);
	}
}

void Fence::Signal(ID3D12CommandQueue* queue_,uint64_t value_)
{
	queue_->Signal(fence.Get(),value_);
}

void Fence::Signal(ID3D12CommandQueue* queue_)
{
	queue_->Signal(fence.Get(),++fenceVal);
}

void Fence::Wait()
{
	if ( fence->GetCompletedValue() != fenceVal )
	{
		HANDLE lEvent = CreateEvent(nullptr,false,false,nullptr);
		fence->SetEventOnCompletion(fenceVal,lEvent);
		if ( lEvent != 0 )
		{
			WaitForSingleObject(lEvent,INFINITE);
			CloseHandle(lEvent);
		}
	}
}

void Fence::Wait(ID3D12CommandQueue* queue_,IFence* fence_,uint64_t Value)
{
	queue_->Wait(fence_->Get(),Value);
}

void Fence::CompulsionWait()
{
	HANDLE lEvent = CreateEvent(nullptr,false,false,nullptr);
	fence->SetEventOnCompletion(fenceVal,lEvent);
	if ( lEvent != 0 )
	{
		WaitForSingleObject(lEvent,INFINITE);
		CloseHandle(lEvent);
	}
}

uint64_t Fence::GetFenceValANDIncrement()
{
	return ++fenceVal;
}

uint64_t Fence::GetFenceVal()
{
	return fenceVal;
}

ID3D12Fence* Fence::Get()
{
	return fence.Get();
}

std::unique_ptr<IFence> CreateUniqueFence(ID3D12Device* device_)
{
	assert(device_);
	std::unique_ptr<IFence> lFence = std::make_unique<Fence>();
	lFence->Initialize(device_);
	return std::move(lFence);
}

std::shared_ptr<IFence> CreateSharedFence(ID3D12Device* device_)
{
	assert(device_);
	std::shared_ptr<IFence> lFence = std::make_shared<Fence>();
	lFence->Initialize(device_);
	return lFence;
}