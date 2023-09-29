#include "Adapter.h"


#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<vector>
#include<directx/d3d12.h>
#include<directx/d3dx12.h>
#include<memory>
#include<dxgi1_6.h>
#include<cassert>
#pragma warning(pop)

#include<Fence.h>
#include<DSVDescriptorHeap.h>
#include<DescriptorHeap.h>
#include<RTVDescriptorHeap.h>

class Adapter : public IAdapter
{
private:

	//コマンドリスト
	std::unique_ptr<ICommandList>commandList;
	//デバイス
	std::unique_ptr<IDevice>device;
	//フェンス
	std::unique_ptr<IFence>fence;
	//DSV用デスクプリタヒープ
	std::unique_ptr<IDSVDescriptorHeap>dsvDescriptorHeap;
	//SRV用デスクプリタヒープ
	std::unique_ptr<ISRVDescriptorHeap>srvDescriptorHeap;
	//RTV用デスクプリタヒープ
	std::unique_ptr<IRTVDescriptorHeap>rtvDescriptorHeap;

	uint32_t maxDSV = 0;
	uint32_t maxSRV = 0;
	uint32_t maxCBV = 0;
	uint32_t maxUAV = 0;
	uint32_t maxRTV = 0;

	int32_t PADING = 0;

public:

	Adapter() = default;
	virtual ~Adapter() = default;

	void Initialize(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_)override;

	IDevice* GetDevice()override;

	ICommandList* GetCommandList();


	void GraphicCommandListExecute()override;
	void GraphicCommandListReset(size_t bbIndex_)override;
	void ResourceTransition(ID3D12Resource* resource_,uint32_t beforeState_,uint32_t afterState_)override;

	ID3D12GraphicsCommandList* GetGraphicCommandList()override;
	ID3D12GraphicsCommandList** GetGraphicCommandListAddressOf()override;
	ID3D12CommandAllocator* GetGraphicCommandAllocator(size_t bbIndex_)override;
	ID3D12CommandQueue* GetGraphicCommandQueue()override;

	void CopyCommandListExecute()override;
	void CopyCommandListReset(size_t bbIndex_) override;

	ID3D12GraphicsCommandList* GetCopyCommandList()override;
	ID3D12GraphicsCommandList** GetCopyCommandListAddressOf()override;
	ID3D12CommandAllocator* GetCopyCommandAllocator(size_t bbIndex_)override;
	ID3D12CommandQueue* GetCopyCommandQueue()override;

	void ComputeCommandListExecute()override;
	void ComputeCommandListReset(size_t bbIndex_)override;

	ID3D12GraphicsCommandList* GetComputeCommandList()override;
	ID3D12GraphicsCommandList** GetComputeCommandListAddressOf()override;
	ID3D12CommandAllocator* GetComputeCommandAllocator(size_t bbIndex_) override;
	ID3D12CommandQueue* GetComputeCommandQueue()override;

	void ComputeWaitPreviousFrame()override;
	void CopyWaitPreviousFrame() override;
	void GraphicWaitPreviousFrame() override;

	void CommandListExecute() override;
	void BeginCommand(size_t bbIndex_) override;
	void WaitPreviousFrame() override;

	IDSVDescriptorHeap* GetDSVDescriptorHeap()override;
	IRTVDescriptorHeap* GetRTVDescriptorHeap()override;
	ISRVDescriptorHeap* GetSRVDescriptorHeap()override;

private:

	//コピーコンストラクタ・代入演算子削除
	Adapter& operator=(const Adapter&) = delete;
	Adapter(const Adapter&) = delete;
};



void Adapter::Initialize(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_)
{
	assert(adapter_);

	maxDSV = maxDSV_;
	maxSRV = maxSRV_;
	maxCBV = maxCBV_;
	maxUAV = maxUAV_;
	maxRTV = maxRTV_;

	device = CreateUniqueDevice(adapter_);

	commandList = CreateUniqueCommandList(device->Get());

	fence = CreateUniqueFence(device->Get());

	dsvDescriptorHeap = CreateUniqueDSVDescriptorHeap(device->Get(),maxDSV);
	srvDescriptorHeap = CreateUniqueSRVDescriptorHeap(device->Get(),maxSRV,maxUAV,maxCBV);
	rtvDescriptorHeap = CreateUniqueRTVDescriptorHeap(device->Get(),maxRTV);
}

IDevice* Adapter::GetDevice()
{
	return device.get();
}

ICommandList* Adapter::GetCommandList()
{
	return commandList.get();
}

void Adapter::GraphicCommandListExecute()
{
	commandList->GraphicCommandListExecute();
}

void Adapter::GraphicCommandListReset(size_t bbIndex_)
{
	commandList->GraphicCommandListReset(bbIndex_);
}

void Adapter::ResourceTransition(ID3D12Resource* resource_,uint32_t beforeState_,uint32_t afterState_)
{
	commandList->ResourceTransition(resource_, beforeState_,afterState_);
}

ID3D12GraphicsCommandList* Adapter::GetGraphicCommandList()
{
	return commandList->GetGraphicCommandList();
}

ID3D12GraphicsCommandList** Adapter::GetGraphicCommandListAddressOf()
{
	return commandList->GetGraphicCommandListAddressOf();
}

ID3D12CommandAllocator* Adapter::GetGraphicCommandAllocator(size_t bbIndex_)
{
	return commandList->GetGraphicCommandAllocator( bbIndex_);
}

ID3D12CommandQueue* Adapter::GetGraphicCommandQueue()
{
	return  commandList->GetGraphicCommandQueue();
}

void Adapter::CopyCommandListExecute()
{
	commandList->CopyCommandListExecute();
}

void Adapter::CopyCommandListReset(size_t bbIndex_)
{
	commandList->CopyCommandListReset( bbIndex_);
}

ID3D12GraphicsCommandList* Adapter::GetCopyCommandList()
{
	return commandList->GetCopyCommandList();
}

ID3D12GraphicsCommandList** Adapter::GetCopyCommandListAddressOf()
{
	return commandList->GetCopyCommandListAddressOf();
}

ID3D12CommandAllocator* Adapter::GetCopyCommandAllocator(size_t bbIndex_)
{
	return commandList->GetCopyCommandAllocator( bbIndex_);
}

ID3D12CommandQueue* Adapter::GetCopyCommandQueue()
{
	return commandList->GetCopyCommandQueue();
}

void Adapter::ComputeCommandListExecute()
{
	commandList->ComputeCommandListExecute();
}

void Adapter::ComputeCommandListReset(size_t bbIndex_)
{
	commandList->ComputeCommandListReset( bbIndex_);
}

ID3D12GraphicsCommandList* Adapter::GetComputeCommandList()
{
	return commandList->GetComputeCommandList();
}

ID3D12GraphicsCommandList** Adapter::GetComputeCommandListAddressOf()
{
	return commandList->GetComputeCommandListAddressOf();
}

ID3D12CommandAllocator* Adapter::GetComputeCommandAllocator(size_t bbIndex_)
{
	return commandList->GetComputeCommandAllocator( bbIndex_);
}

ID3D12CommandQueue* Adapter::GetComputeCommandQueue()
{
	return commandList->GetComputeCommandQueue();
}

void Adapter::ComputeWaitPreviousFrame()
{
	fence->WaitPreviousFrame(commandList->GetComputeCommandQueue());
}

void Adapter::CopyWaitPreviousFrame()
{
	fence->WaitPreviousFrame(commandList->GetCopyCommandQueue());
}

void Adapter::GraphicWaitPreviousFrame()
{
	fence->WaitPreviousFrame(commandList->GetGraphicCommandQueue());
}

IDSVDescriptorHeap* Adapter::GetDSVDescriptorHeap()
{
	return dsvDescriptorHeap.get();
}

IRTVDescriptorHeap* Adapter::GetRTVDescriptorHeap()
{
	return rtvDescriptorHeap.get();
}

ISRVDescriptorHeap* Adapter::GetSRVDescriptorHeap()
{
	return srvDescriptorHeap.get();
}

void Adapter::CommandListExecute()
{
	commandList->CommandListExecute();
}

void Adapter::BeginCommand(size_t bbIndex_)
{
	commandList->BeginCommand( bbIndex_);
}

void Adapter::WaitPreviousFrame()
{
	ComputeWaitPreviousFrame();
	CopyWaitPreviousFrame();
	GraphicWaitPreviousFrame();
}

std::unique_ptr<IAdapter> CreateUniqueAdapter(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_)
{
	std::unique_ptr<IAdapter>adapter = std::make_unique<Adapter>();
	adapter->Initialize(adapter_,maxDSV_,maxRTV_,maxSRV_,maxCBV_,maxUAV_);
	return std::move(adapter);
}

std::shared_ptr<IAdapter> CreateSharedAdapter(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_)
{
	std::shared_ptr<IAdapter>adapter = std::make_shared<Adapter>();
	adapter->Initialize(adapter_,maxDSV_,maxRTV_,maxSRV_,maxCBV_,maxUAV_);
	return adapter;
}
