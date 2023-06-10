#include "SwapChain.h"


#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<vector>

#pragma warning(pop)

#include<cassert>

class SwapChain :public ISwapChain
{
private:

	//デバイス
	ID3D12Device* device;

	//スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	//バッファの数
	static const uint32_t FrameBufferCount = 2;

	//バックバッファ
	std::vector<std::unique_ptr<IRenderTargetBuffer>> backBuffers;

	//フェンスの生成
	std::vector<uint64_t> fenceValues;
	uint64_t fenceVal;

	//ディスク
	DXGI_SWAP_CHAIN_DESC1 desc;

	//ハンドル
	HANDLE waitEvent;

	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;

	//ディスクリプタヒープ
	ID3D12CommandQueue* commandQueue;


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="swapchain">スワップチェイン</param>
	/// <param name="device_">デバイス</param>
	/// <param name="heap">レンダーターゲット用ディスクリプタヒープ</param>
	void Initialize(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_);

	SwapChain() = default;

	~SwapChain();

	/// <summary>
	/// リサイズ
	/// </summary>
	void ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters_);

	/// <summary>
	/// フルスクリーンか
	/// </summary>
	bool IsFullScreen() const;

	/// <summary>
	/// フルスクリーンをセット
	/// </summary>
	void SetFullScreen(bool toFullScreen_);

	/// <summary>
	/// バッファサイズ変更
	/// </summary>
	void ResizeBuffers(uint32_t width_, uint32_t height_);

	void WaitPreviousFrame();

	void WaitForGpu();

	HRESULT Present(uint32_t SyncInterval_, uint32_t Flags_);

	void Transition(size_t index, const D3D12_RESOURCE_STATES& resourceStates_);

	uint32_t GetCurrentBackBufferIndex() const;

	IRenderTargetBuffer* GetRenderTarget(size_t index_)const;

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderTargetHandl(size_t index_);

	size_t GetBackBufferCount() const;
};

void SwapChain::Initialize(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_)
{
	// IDXGISwapChain4 取得
	swapchain_.As(&swapChain);

	//ディスク取得
	swapChain->GetDesc1(&desc);

	//デバイス取得
	device = device_;
	commandQueue = queue_;

	//バックバッファ
	backBuffers.resize(desc.BufferCount);
	fenceValues.resize(desc.BufferCount);

	size_t lFrameIndex = swapChain->GetCurrentBackBufferIndex();

	//フェンスの生成
	if (FAILED(device->CreateFence(fenceValues[lFrameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()))))
	{
		printf("Failed to generate fence");
		assert(0);
	}

	fenceValues[lFrameIndex]++;

	// フレーム同期に使用するイベントハンドルを作成します。
	waitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (waitEvent == nullptr)
	{
		FAILED(HRESULT_FROM_WIN32(GetLastError()));
	}

	//スワップチェーンのバッファを処理
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		//生成
		backBuffers[i] = CreateUniqueRenderTargetBuffer(swapChain.Get(), static_cast<uint32_t>(i));
	}
}

SwapChain::~SwapChain()
{
	BOOL lIsFullScreen;

	swapChain->GetFullscreenState(&lIsFullScreen, nullptr);
	if (lIsFullScreen)
	{
		swapChain->SetFullscreenState(FALSE, nullptr);
	}

	CloseHandle(waitEvent);
}

void SwapChain::ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters)
{
	swapChain->ResizeTarget(pNewTargetParameters);
}

bool SwapChain::IsFullScreen() const
{
	BOOL lFullscreen;

	//フルスクリーンかどうか
	if (FAILED(swapChain->GetFullscreenState(&lFullscreen, nullptr)))
	{
		lFullscreen = FALSE;
	}

	return lFullscreen == TRUE;
}

void SwapChain::SetFullScreen(bool toFullScreen_)
{
	WaitForGpu();

	if (toFullScreen_)
	{
		//フルスクリーン
		swapChain->SetFullscreenState(TRUE, nullptr);
	}
	else
	{
		//ウィンドウ
		swapChain->SetFullscreenState(FALSE, nullptr);
	}
}

void SwapChain::ResizeBuffers(uint32_t width_, uint32_t height_)
{
	// リサイズのために全て解放.
	for (auto& v : backBuffers)
	{
		v->Reset();
	}

	if (FAILED(swapChain->ResizeBuffers(desc.BufferCount, width_, height_, desc.Format, desc.Flags)))
	{
		printf("No Resize");
		assert(0);
	}

	for (uint32_t i = 0; i < desc.BufferCount; ++i)
	{
		backBuffers[i]->Resize(swapChain.Get(), i);
	}
}

void SwapChain::WaitPreviousFrame()
{
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE lEvent = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, lEvent);
		if (lEvent != 0)
		{
			WaitForSingleObject(lEvent, INFINITE);
			CloseHandle(lEvent);
		}
	}
}

void SwapChain::WaitForGpu()
{
	uint32_t lFrameIndex = swapChain->GetCurrentBackBufferIndex();

	// キューにあるSignalコマンドをスケジュール
	if (FAILED(commandQueue->Signal(fence.Get(), fenceValues[lFrameIndex])))
	{
		printf("No Signal");
		assert(0);
	}

	// フェンスの処理が完了するまで待つ。
	if (FAILED(fence->SetEventOnCompletion(fenceValues[lFrameIndex], waitEvent)))
	{
		printf("Event could not be set");
		assert(0);
	}

	WaitForSingleObjectEx(waitEvent, INFINITE, FALSE);

	// 現在のフレームのフェンス値をインクリメントする。
	fenceValues[lFrameIndex]++;
}

HRESULT SwapChain::Present(uint32_t SyncInterval, uint32_t Flags)
{
	return swapChain->Present(SyncInterval, Flags);
}

void SwapChain::Transition(size_t index, const D3D12_RESOURCE_STATES& resourceStates)
{
	backBuffers[index]->Transition(resourceStates);
}

UINT SwapChain::GetCurrentBackBufferIndex() const
{
	return swapChain->GetCurrentBackBufferIndex();
}

IRenderTargetBuffer* SwapChain::GetRenderTarget(size_t index)const
{
	return backBuffers[index].get();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& SwapChain::GetRenderTargetHandl(size_t index)
{
	return backBuffers[index]->GetHandle();
}

size_t SwapChain::GetBackBufferCount() const
{
	return backBuffers.size();
}

std::unique_ptr<ISwapChain> CreateUniqueSwapChain(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_)
{
	std::unique_ptr<ISwapChain> lSwapChain = std::make_unique<SwapChain>();
	lSwapChain->Initialize(device_, swapchain_, queue_);
	return std::move(lSwapChain);
}

std::shared_ptr<ISwapChain> CreateSharedSwapChain(ID3D12Device* device_, const Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain_, ID3D12CommandQueue* queue_)
{
	std::shared_ptr<ISwapChain> lSwapChain = std::make_shared<SwapChain>();
	lSwapChain->Initialize(device_, swapchain_, queue_);
	return lSwapChain;
}
