#include "SwapChain.h"

SwapChain::SwapChain(ID3D12Device* dev, Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain, ID3D12CommandQueue* queue, bool useHDR)
{
	// IDXGISwapChain4 取得
	swapchain.As(&swapChain);

	//ディスク取得
	swapChain->GetDesc1(&desc);

	//デバイス取得
	device = dev;
	commandQueue = queue;

	//バックバッファ
	backBuffers.resize(desc.BufferCount);
	fenceValues.resize(desc.BufferCount);

	UINT frameIndex = swapChain->GetCurrentBackBufferIndex();

	//フェンスの生成
	if (FAILED(device->CreateFence(fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()))))
	{
		printf("Failed to generate fence");
		assert(0);
	}

	fenceValues[frameIndex]++;

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
		backBuffers[i] = std::make_unique<RenderTargetBuffer>();
		backBuffers[i]->Create(swapChain.Get(), static_cast<UINT>(i));
	}

	if (useHDR)
	{

	}
}

SwapChain::~SwapChain()
{
	BOOL isFullScreen;
	swapChain->GetFullscreenState(&isFullScreen, nullptr);
	if (isFullScreen)
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
	BOOL fullscreen;

	//フルスクリーンかどうか
	if (FAILED(swapChain->GetFullscreenState(&fullscreen, nullptr)))
	{
		fullscreen = FALSE;
	}

	return fullscreen == TRUE;
}

void SwapChain::SetFullScreen(bool toFullScreen)
{
	WaitForGpu();

	if (toFullScreen)
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

void SwapChain::ResizeBuffers(UINT width, UINT height)
{
	// リサイズのために全て解放.
	for (auto& v : backBuffers)
	{
		v->Reset();
	}

	if (FAILED(swapChain->ResizeBuffers(desc.BufferCount, width, height, desc.Format, desc.Flags)))
	{
		printf("No Resize");
		assert(0);
	}

	for (UINT i = 0; i < desc.BufferCount; ++i)
	{
		backBuffers[i]->Resize(swapChain.Get(), i);
	}
}

void SwapChain::WaitPreviousFrame()
{
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
}

void SwapChain::WaitForGpu()
{
	UINT frameIndex = swapChain->GetCurrentBackBufferIndex();

	// キューにあるSignalコマンドをスケジュール
	if (FAILED(commandQueue->Signal(fence.Get(), fenceValues[frameIndex])))
	{
		printf("No Signal");
		assert(0);
	}

	// フェンスの処理が完了するまで待つ。
	if (FAILED(fence->SetEventOnCompletion(fenceValues[frameIndex], waitEvent)))
	{
		printf("Event could not be set");
		assert(0);
	}

	WaitForSingleObjectEx(waitEvent, INFINITE, FALSE);

	// 現在のフレームのフェンス値をインクリメントする。
	fenceValues[frameIndex]++;
}

HRESULT SwapChain::Present(UINT SyncInterval, UINT Flags)
{
	return swapChain->Present(SyncInterval, Flags);
}

void SwapChain::Transition(size_t index, D3D12_RESOURCE_STATES resourceStates)
{
	backBuffers[index]->Transition(resourceStates);
}

UINT SwapChain::GetCurrentBackBufferIndex() const
{
	return swapChain->GetCurrentBackBufferIndex();
}

RenderTargetBuffer* SwapChain::GetRenderTarget(size_t index)const
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

