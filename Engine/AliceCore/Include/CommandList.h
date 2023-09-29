#pragma once

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

struct ID3D12Device;
struct ID3D12Resource;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandAllocator;
struct ID3D12CommandQueue;

/// <summary>
/// コマンドリスト
/// </summary>
class ICommandList
{
public:

	ICommandList() = default;
	virtual ~ICommandList() = default;

	virtual void Initialize(ID3D12Device* device_) = 0;

	virtual void GraphicCommandListExecute() = 0;
	virtual void GraphicCommandListReset(size_t bbIndex_) = 0;
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

	virtual ID3D12GraphicsCommandList* GetComputeCommandList() = 0;
	virtual ID3D12GraphicsCommandList** GetComputeCommandListAddressOf() = 0;
	virtual ID3D12CommandAllocator* GetComputeCommandAllocator(size_t bbIndex_) = 0;
	virtual ID3D12CommandQueue* GetComputeCommandQueue() = 0;

	virtual void CommandListExecute() = 0;
	virtual void BeginCommand(size_t bbIndex_) = 0;

};

/// <summary>
/// コマンドリストの生成(ユニーク)
/// </summary>
/// <param name="device_">デバイス</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<ICommandList> CreateUniqueCommandList(ID3D12Device* device_);

/// <summary>
/// コマンドリストの生成(シェアード)
/// </summary>
/// <param name="device_">デバイス</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<ICommandList> CreateSharedCommandList(ID3D12Device* device_);