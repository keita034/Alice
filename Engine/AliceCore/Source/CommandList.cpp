#include "CommandList.h"



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

struct  CommandListSet
{
	//アロケーター
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocators;
	//リスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list;
	//キュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue;
};

class CommandList : public ICommandList
{
private:

	//グラフィックス用
	CommandListSet graphic;
	//コピー用
	CommandListSet copy;
	//コンピュート用
	CommandListSet compute;

	const size_t FRAME_BUFFER_COUNT = 2;

	HRESULT result;

	int32_t PADING;

public:

	CommandList() = default;
	virtual ~CommandList() = default;

	void Initialize(ID3D12Device* device_)override;

	void ResourceTransition(ID3D12Resource* resource_,uint32_t beforeState_,uint32_t afterState_)override;

	void GraphicCommandListExecute()override;
	void CopyCommandListExecute()override;
	void ComputeCommandListExecute()override;

	void GraphicCommandListReset(size_t bbIndex_)override;
	void ComputeCommandListReset(size_t bbIndex_)override;
	void CopyCommandListReset(size_t bbIndex_)override;

	ID3D12GraphicsCommandList* GetGraphicCommandList()override;
	ID3D12GraphicsCommandList* GetComputeCommandList()override;
	ID3D12GraphicsCommandList* GetCopyCommandList()override;

	ID3D12GraphicsCommandList** GetGraphicCommandListAddressOf()override;
	ID3D12GraphicsCommandList** GetComputeCommandListAddressOf()override;
	ID3D12GraphicsCommandList** GetCopyCommandListAddressOf()override;

	ID3D12CommandAllocator* GetGraphicCommandAllocator(size_t bbIndex_)override;
	ID3D12CommandAllocator* GetComputeCommandAllocator(size_t bbIndex_)override;
	ID3D12CommandAllocator* GetCopyCommandAllocator(size_t bbIndex_) override;

	ID3D12CommandQueue* GetComputeCommandQueue()override;
	ID3D12CommandQueue* GetCopyCommandQueue()override;
	ID3D12CommandQueue* GetGraphicCommandQueue() override;

	void CommandListExecute() override;
	void BeginCommand(size_t bbIndex_) override;

private:

	//コピーコンストラクタ・代入演算子削除
	CommandList& operator=(const CommandList&) = delete;
	CommandList(const CommandList&) = delete;
};

void CommandList::Initialize(ID3D12Device* device_)
{
	D3D12_COMMAND_LIST_TYPE lListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//グラフィックス用
	{
		//コマンドアロケーターを生成
		graphic.allocators.resize(FRAME_BUFFER_COUNT);
		for ( size_t i = 0; i < FRAME_BUFFER_COUNT; ++i )
		{
			result = device_->CreateCommandAllocator(lListType,IID_PPV_ARGS(&graphic.allocators[ i ])
			);
			if ( FAILED(result) )
			{
				printf("Failed CreateCommandAllocator");
			}
		}

		//コマンドキューの設定＆生成
		D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{};
		lCommandQueueDesc.Type = lListType;
		result = device_->CreateCommandQueue(&lCommandQueueDesc,IID_PPV_ARGS(graphic.queue.ReleaseAndGetAddressOf()));
		if ( FAILED(result) )
		{
			printf("Failed CreateCommandQueue");
		}

		//コマンドリストを生成
		result = device_->CreateCommandList(0,lListType,graphic.allocators[ 0 ].Get(),nullptr,IID_PPV_ARGS(graphic.list.ReleaseAndGetAddressOf()));
		if ( FAILED(result) )
		{
			printf("Failed CreateCommandList");
		}

		graphic.list->Close();
	}

	lListType = D3D12_COMMAND_LIST_TYPE_COPY;
	//コピー用
	{
		//コマンドアロケーターを生成
		copy.allocators.resize(FRAME_BUFFER_COUNT);
		for ( size_t i = 0; i < FRAME_BUFFER_COUNT; ++i )
		{
			result = device_->CreateCommandAllocator(lListType,IID_PPV_ARGS(&copy.allocators[ i ])
			);
			if ( FAILED(result) )
			{
				printf("Failed CreateCommandAllocator");
			}
		}

		//コマンドキューの設定＆生成
		D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{};
		lCommandQueueDesc.Type = lListType;
		result = device_->CreateCommandQueue(&lCommandQueueDesc,IID_PPV_ARGS(copy.queue.ReleaseAndGetAddressOf()));
		if ( FAILED(result) )
		{
			printf("Failed CreateCommandQueue");
		}

		//コマンドリストを生成
		result = device_->CreateCommandList(0,lListType,copy.allocators[ 0 ].Get(),nullptr,IID_PPV_ARGS(copy.list.ReleaseAndGetAddressOf()));
		if ( FAILED(result) )
		{
			printf("Failed CreateCommandList");
		}

		copy.list->Close();
	}

	lListType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	//コンピュート用
	{
		//コマンドアロケーターを生成
		compute.allocators.resize(FRAME_BUFFER_COUNT);
		for ( size_t i = 0; i < FRAME_BUFFER_COUNT; ++i )
		{
			result = device_->CreateCommandAllocator(lListType,IID_PPV_ARGS(&compute.allocators[ i ])
			);
			if ( FAILED(result) )
			{
				printf("Failed CreateCommandAllocator");
			}
		}

		//コマンドキューの設定＆生成
		D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{};
		lCommandQueueDesc.Type = lListType;
		result = device_->CreateCommandQueue(&lCommandQueueDesc,IID_PPV_ARGS(compute.queue.ReleaseAndGetAddressOf()));
		if ( FAILED(result) )
		{
			printf("Failed CreateCommandQueue");
		}

		//コマンドリストを生成
		result = device_->CreateCommandList(0,lListType,compute.allocators[ 0 ].Get(),nullptr,IID_PPV_ARGS(compute.list.ReleaseAndGetAddressOf()));
		if ( FAILED(result) )
		{
			printf("Failed CreateCommandList");
		}

		compute.list->Close();
	}
}

#pragma region グラフィックス

void CommandList::GraphicCommandListExecute()
{
	result = graphic.list->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* lCommandListts[ ] = { graphic.list.Get() };
	graphic.queue->ExecuteCommandLists(1,lCommandListts);
}

void CommandList::GraphicCommandListReset(size_t bbIndex_)
{
	//キューをクリア
	result = graphic.allocators[ bbIndex_ ]->Reset();
	assert(SUCCEEDED(result));

	//コマンドリストを貯める準備
	result = graphic.list->Reset(graphic.allocators[ bbIndex_ ].Get(),nullptr);
	assert(SUCCEEDED(result));
}

void CommandList::ResourceTransition(ID3D12Resource* resource_,uint32_t beforeState_,uint32_t afterState_)
{
	auto lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource_,static_cast< D3D12_RESOURCE_STATES >( beforeState_ ),static_cast< D3D12_RESOURCE_STATES >( afterState_ ));
	graphic.list->ResourceBarrier(1,&lBarrier);
}

ID3D12GraphicsCommandList* CommandList::GetGraphicCommandList()
{
	return graphic.list.Get();
}

ID3D12GraphicsCommandList** CommandList::GetGraphicCommandListAddressOf()
{
	return graphic.list.GetAddressOf();
}

ID3D12CommandAllocator* CommandList::GetGraphicCommandAllocator(size_t bbIndex_)
{
	return graphic.allocators[ bbIndex_ ].Get();
}

ID3D12CommandQueue* CommandList::GetGraphicCommandQueue()
{
	return graphic.queue.Get();
}
#pragma endregion

#pragma region コピー

void CommandList::CopyCommandListExecute()
{
	result = copy.list->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* lCommandListts[ ] = { copy.list.Get() };
	copy.queue->ExecuteCommandLists(1,lCommandListts);
}

void CommandList::CopyCommandListReset(size_t bbIndex_)
{
	//キューをクリア
	result = copy.allocators[ bbIndex_ ]->Reset();
	assert(SUCCEEDED(result));

	//コマンドリストを貯める準備
	result = copy.list->Reset(copy.allocators[ bbIndex_ ].Get(),nullptr);
	assert(SUCCEEDED(result));
}

ID3D12GraphicsCommandList* CommandList::GetCopyCommandList()
{
	return copy.list.Get();
}

ID3D12GraphicsCommandList** CommandList::GetCopyCommandListAddressOf()
{
	return copy.list.GetAddressOf();
}

ID3D12CommandAllocator* CommandList::GetCopyCommandAllocator(size_t bbIndex_)
{
	return copy.allocators[ bbIndex_ ].Get();
}

ID3D12CommandQueue* CommandList::GetCopyCommandQueue()
{
	return copy.queue.Get();
}
#pragma endregion

#pragma region コンピュート

ID3D12GraphicsCommandList* CommandList::GetComputeCommandList()
{
	return compute.list.Get();
}

void CommandList::ComputeCommandListExecute()
{
	result = compute.list->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* lCommandListts[ ] = { compute.list.Get() };
	compute.queue->ExecuteCommandLists(1,lCommandListts);
}

void CommandList::ComputeCommandListReset(size_t bbIndex_)
{
	//キューをクリア
	result = compute.allocators[ bbIndex_ ]->Reset();
	assert(SUCCEEDED(result));

	//コマンドリストを貯める準備
	result = compute.list->Reset(compute.allocators[ bbIndex_ ].Get(),nullptr);
	assert(SUCCEEDED(result));
}

ID3D12GraphicsCommandList** CommandList::GetComputeCommandListAddressOf()
{
	return compute.list.GetAddressOf();
}

ID3D12CommandAllocator* CommandList::GetComputeCommandAllocator(size_t bbIndex_)
{
	return compute.allocators[ bbIndex_ ].Get();
}

ID3D12CommandQueue* CommandList::GetComputeCommandQueue()
{
	return compute.queue.Get();
}

#pragma endregion

void CommandList::CommandListExecute()
{
	GraphicCommandListExecute();
	CopyCommandListExecute();
	ComputeCommandListExecute();
}

void CommandList::BeginCommand(size_t bbIndex_)
{
	GraphicCommandListReset(bbIndex_);
	CopyCommandListReset(bbIndex_);
	ComputeCommandListReset(bbIndex_);
}


std::unique_ptr<ICommandList> CreateUniqueCommandList(ID3D12Device* device_)
{
	assert(device_);
	std::unique_ptr<ICommandList> lCommandList = std::make_unique<CommandList>();
	lCommandList->Initialize(device_);
	return std::move(lCommandList);
}

std::shared_ptr<ICommandList> CreateSharedCommandList(ID3D12Device* device_)
{
	assert(device_);
	std::shared_ptr<ICommandList> lCommandList = std::make_shared<CommandList>();
	lCommandList->Initialize(device_);
	return lCommandList;
}