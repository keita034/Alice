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

	enum FenceIndex
	{
		MAIN,
		SUB
	};

	static constexpr size_t NUM_DEVICES = 2;

	std::array<Microsoft::WRL::ComPtr<ID3D12Fence>,NUM_DEVICES> fences;
	std::array<HANDLE,NUM_DEVICES> fenceEvents;
	uint64_t value = 0;

public:

	void Initialize(IAdapter* mainAdapter_,IAdapter* subAdapter_)override;
	ID3D12Fence* Get(size_t index_)override;
	void Wait(ID3D12CommandQueue* queue_,size_t index_)override;
	void Signal(ID3D12CommandQueue* queue_,size_t index_)override;
	void WaitForGpu(AdaptersIndex index_)override;

	CrossAdapterFence() = default;
	~CrossAdapterFence() = default;
};

void CrossAdapterFence::Initialize(IAdapter* mainAdapter_,IAdapter* subAdapter_)
{

	// プライマリGPUのフェンスをセットアップし、セットアップが完了するまで待つ
	{
		ID3D12Device* lMainDevice = mainAdapter_->GetDevice()->Get();

		lMainDevice->CreateFence(0,D3D12_FENCE_FLAG_SHARED | D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER,IID_PPV_ARGS(&fences[ MAIN ]));

		fenceEvents[ MAIN ] = CreateEventEx(nullptr,FALSE,FALSE,EVENT_ALL_ACCESS);
	}

	// セカンダリGPUのフェンスをセットアップし、セットアップが完了するまで待つ
	{
		ID3D12Device* lSubDevice = subAdapter_->GetDevice()->Get();

		HANDLE lFenceHandle = nullptr;
		lSubDevice->CreateSharedHandle(fences[ MAIN ].Get(),nullptr,GENERIC_ALL,nullptr,&lFenceHandle);

		// secondaryDevice GPU上のフェンスへの共有ハンドルを開く
		lSubDevice->OpenSharedHandle(lFenceHandle,IID_PPV_ARGS(&fences[ SUB ]));

		fenceEvents[ SUB ] = CreateEventEx(nullptr,FALSE,FALSE,EVENT_ALL_ACCESS);
	}

}

ID3D12Fence* CrossAdapterFence::Get(size_t index_)
{
	return fences[ index_ ].Get();
}

void CrossAdapterFence::Wait(ID3D12CommandQueue* queue_,size_t index_)
{
	queue_->Wait(fences[ index_ ].Get(),++value);
}

void CrossAdapterFence::Signal(ID3D12CommandQueue* queue_,size_t index_)
{
	queue_->Signal(fences[ index_ ].Get(),value);
}

void CrossAdapterFence::WaitForGpu(AdaptersIndex index_)
{
	// フェンスの処理が終わるまで待つ。
	fences[ static_cast< size_t >( index_ ) ]->SetEventOnCompletion(value,fenceEvents[ static_cast<size_t>( index_ ) ]);
	WaitForSingleObject(fenceEvents[ static_cast< size_t >( index_ ) ],INFINITE);
}

std::unique_ptr<ICrossAdapterFence> CreateUniqueCrossAdapterFence(IAdapter* mainAdapter_,IAdapter* subAdapter_)
{
	std::unique_ptr<ICrossAdapterFence> lCrossAdapterFence = std::make_unique<CrossAdapterFence>();
	lCrossAdapterFence->Initialize(mainAdapter_,subAdapter_);
	return std::move(lCrossAdapterFence);
}

std::shared_ptr<ICrossAdapterFence> CreateSharedCrossAdapterFence(IAdapter* mainAdapter_,IAdapter* subAdapter_)
{
	std::shared_ptr<ICrossAdapterFence> lCrossAdapterFence = std::make_shared<CrossAdapterFence>();
	lCrossAdapterFence->Initialize(mainAdapter_,subAdapter_);
	return lCrossAdapterFence;
}
