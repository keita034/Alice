#pragma once
#include<stdint.h>

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>

#pragma warning(pop)


class IDSVDescriptorHeap;
class IRTVDescriptorHeap;
class ISRVDescriptorHeap;

struct IDXGIAdapter;
struct ID3D12Device;
struct ID3D12Resource;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct ID3D12CommandAllocator;

#include<Device.h>
#include<CommandList.h>
#include<Fence.h>
#include<AdaptersIndex.h>

/// <summary>
/// アダプター
/// </summary>
class IAdapter
{
public:

	IAdapter() = default;
	virtual ~IAdapter() = default;

	virtual void Initialize(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_) = 0;

	virtual IDevice* GetDevice() = 0;
	virtual ICommandList* GetCommandList() = 0;
	virtual IFence* GetFence() =0;

	virtual void GraphicCommandListExecute() = 0;
	virtual void GraphicCommandListReset(size_t bbIndex_) = 0;
	virtual void GraphicCommandListReset() = 0;
	virtual void ResourceTransition(ID3D12Resource* resource_,uint32_t beforeState_,uint32_t afterState_) = 0;

	virtual ID3D12GraphicsCommandList* GetGraphicCommandList() = 0;
	virtual ID3D12GraphicsCommandList** GetGraphicCommandListAddressOf() = 0;
	virtual ID3D12CommandAllocator* GetGraphicCommandAllocator(size_t bbIndex_) = 0;
	virtual ID3D12CommandQueue* GetGraphicCommandQueue() = 0;

	virtual void CopyCommandListExecute() = 0;
	virtual void CopyCommandListReset(size_t bbIndex_) = 0;

	virtual ID3D12GraphicsCommandList* GetCopyCommandList() = 0;
	virtual ID3D12GraphicsCommandList** GetCopyCommandListAddressOf() = 0;
	virtual ID3D12CommandAllocator* GetCopyCommandAllocator(size_t bbIndex_) = 0;
	virtual ID3D12CommandQueue* GetCopyCommandQueue() = 0;

	virtual void ComputeCommandListExecute() = 0;
	virtual void ComputeCommandListReset(size_t bbIndex_) = 0;
	virtual void ComputeCommandListReset() = 0;

	virtual ID3D12GraphicsCommandList* GetComputeCommandList() = 0;
	virtual ID3D12GraphicsCommandList** GetComputeCommandListAddressOf() = 0;
	virtual ID3D12CommandAllocator* GetComputeCommandAllocator(size_t bbIndex_) = 0;
	virtual ID3D12CommandQueue* GetComputeCommandQueue() = 0;

	virtual void ComputeWaitPreviousFrame() = 0;
	virtual void ComputeCompulsionWait() = 0;
	virtual void CopyWaitPreviousFrame() = 0;
	virtual	 void GraphicWaitPreviousFrame() = 0;

	virtual void CommandListExecute() = 0;
	virtual void BeginCommand(size_t bbIndex_) =0;
	virtual void WaitPreviousFrame() = 0;

	virtual IDSVDescriptorHeap* GetDSVDescriptorHeap() = 0;
	virtual IRTVDescriptorHeap* GetRTVDescriptorHeap() = 0;
	virtual ISRVDescriptorHeap* GetSRVDescriptorHeap() = 0;

	virtual void SetIndex(AdaptersIndex index_) =0;
	virtual AdaptersIndex GetIndex() =0;
};

/// <summary>
/// アダプターの生成(ユニーク)
/// </summary>
/// <param name="adapter_">アダプター</param>
/// <param name="maxDSV_">DSVの最大数</param>
/// <param name="maxRTV_">RTVの最大数</param>
/// <param name="maxSRV_">SRVの最大数</param>
/// <param name="maxCBV_">CBVの最大数</param>
/// <param name="maxUAV_">UAVの最大数</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IAdapter> CreateUniqueAdapter(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_);

/// <summary>
/// アダプターの生成(シェアード)
/// </summary>
/// <param name="adapter_">アダプター</param>
/// <param name="maxDSV_">DSVの最大数</param>
/// <param name="maxRTV_">RTVの最大数</param>
/// <param name="maxSRV_">SRVの最大数</param>
/// <param name="maxCBV_">CBVの最大数</param>
/// <param name="maxUAV_">UAVの最大数</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IAdapter> CreateSharedAdapter(IDXGIAdapter* adapter_,uint32_t maxDSV_,uint32_t maxRTV_,uint32_t maxSRV_,uint32_t maxCBV_,uint32_t maxUAV_);

