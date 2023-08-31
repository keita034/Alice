#include "Fence.h"

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)

#include<wrl.h>
#include<vector>
#include<directx/d3dx12.h>
#include<dxgi1_6.h>
#include<cassert>

#pragma warning(pop)

class Fence : public IFence
{
private:

	uint64_t fenceVal;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

public:

	Fence() = default;
	~Fence() = default;

	void Initialize(ID3D12Device* device_);

	void WaitPreviousFrame(ID3D12CommandQueue* queue_);
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

void Fence::WaitPreviousFrame(ID3D12CommandQueue* queue_)
{
	queue_->Signal(fence.Get(),++fenceVal);
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
