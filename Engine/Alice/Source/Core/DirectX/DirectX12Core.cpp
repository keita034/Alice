#include<DirectX12Core.h>

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4514)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<directx/d3dx12.h>
#include<cassert>

#pragma warning(pop)

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include<BaseBuffer.h>
#include<BaseDescriptorHeap.h>

HRESULT DirectX12Core::PCreateSwapChain()
{
	swapChainDesc.Width = static_cast<UINT>(width);//横幅
	swapChainDesc.Height = static_cast<UINT>(height);//縦幅
	swapChainDesc.Format = surfaceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルなし
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//バックバッファ用
	swapChainDesc.BufferCount = 2;//バッファ数2
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;//フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC lFsDesc{};
	lFsDesc.Windowed = false ? FALSE : TRUE;
	lFsDesc.RefreshRate.Denominator = 1000;
	lFsDesc.RefreshRate.Numerator = 60317;
	lFsDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	lFsDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;

	//生成
	if (commandQueue != 0)
	{
		Microsoft::WRL::ComPtr<IDXGISwapChain1> lTmpSwapChain;
		result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), *handle, &swapChainDesc, &lFsDesc, nullptr, lTmpSwapChain.ReleaseAndGetAddressOf());
		swapChain = CreateUniqueSwapChain(device.Get(), lTmpSwapChain, commandQueue.Get());
	}
	else
	{
		assert(SUCCEEDED(0));
	}

	return result;
}

size_t DirectX12Core::GetBackBufferCount() const
{
	return swapChain->GetBackBufferCount();
}

bool DirectX12Core::SetWindowType(WindowMode mode_, uint32_t windowHeight_, uint32_t windowWidth_)
{
	switch (mode_)
	{
	case WindowMode::WINDOW:

		windowMode = WindowMode::WINDOW;

		// 処理の完了を待ってからサイズ変更の処理を開始.
		swapChain->WaitForGpu();

		swapChain->SetFullScreen(false);

		windowsApp->ShowDefaultWindow(static_cast<uint64_t>(windowHeight_), static_cast<uint64_t>(windowWidth_));

		IWindowsApp::WindowsSize lWinSize = windowsApp->GetNowWindowSize();

		width = static_cast<float>(lWinSize.width);
		height = static_cast<float>(lWinSize.height);

		swapChain->ResizeBuffers(lWinSize.width, lWinSize.height);

		//レンダーサイズ変更
		RenderSizeChanged(lWinSize.width, lWinSize.height);

		break;

	case WindowMode::FULLSCREEN:

		if (windowMode != WindowMode::FULLSCREEN)
		{
			windowMode = WindowMode::FULLSCREEN;

			swapChain->SetFullScreen(true);

			IWindowsApp::WindowsSize lSize = windowsApp->GetNowWindowSize();

			width = static_cast<float>(lSize.width);
			height = static_cast<float>(lSize.height);

			DXGI_MODE_DESC lModeDesc{};
			lModeDesc.Format = surfaceFormat;
			lModeDesc.Width = lSize.width;
			lModeDesc.Height = lSize.height;
			lModeDesc.RefreshRate.Denominator = 1;
			lModeDesc.RefreshRate.Numerator = 60;
			lModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			lModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChain->ResizeTarget(&lModeDesc);

			swapChain->ResizeBuffers(lSize.width, lSize.height);

			//レンダーサイズ変更
			RenderSizeChanged(lSize.width, lSize.height);
		}

		break;

	case WindowMode::BRDERLESSWINDOW:

		if (windowMode != WindowMode::BRDERLESSWINDOW)
		{
			windowMode = WindowMode::BRDERLESSWINDOW;

			windowsApp->ShowFullScreen();

			IWindowsApp::WindowsSize lSize = windowsApp->GetNowWindowSize();

			width = static_cast<float>(lSize.width);
			height = static_cast<float>(lSize.height);

			swapChain->ResizeBuffers(lSize.width, lSize.height);

			//レンダーサイズ変更
			RenderSizeChanged(lSize.width, lSize.height);
		}

		break;
	default:
		break;
	}

	return true;
}

void DirectX12Core::RenderSizeChanged(uint32_t width_, uint32_t height_)
{
	width = static_cast<float>(width_);
	height = static_cast<float>(height_);

	//バッファのサイズ変更
	swapChain->ResizeBuffers(width_, height_);

	// デプスバッファの作り直し.
	depthBuff->Resize(width_, height_);

	bbIndex = swapChain->GetCurrentBackBufferIndex();

	viewport.Width = width;
	viewport.Height = height;

	scissorRect.right = scissorRect.left + static_cast<LONG>(width);
	scissorRect.bottom = scissorRect.top + static_cast<LONG>(height);

}

HRESULT DirectX12Core::PInitializeDXGIDevice()
{
	//対応レベルの配列
	D3D_FEATURE_LEVEL lLevels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	//DXGIファクトリー
	result = CreateDXGIFactory(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	//アダプター列挙用
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter4>> lAdapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	Microsoft::WRL::ComPtr<IDXGIAdapter4> lTmpAdapter;

	//パフォーマンスが高いのもから全て列挙
	for (uint32_t i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(lTmpAdapter.ReleaseAndGetAddressOf())) != DXGI_ERROR_NOT_FOUND; i++)
	{
		lAdapters.push_back(lTmpAdapter);
	}

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < lAdapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 lAdapterDesc;
		// アダプターの情報を取得する
		lAdapters[i]->GetDesc3(&lAdapterDesc);

		// ソフトウェアデバイスを回避
		if (!(lAdapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			lTmpAdapter = lAdapters[i];
			break;
		}
	}

	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL lFeatureLevel;

	for (size_t i = 0; i < _countof(lLevels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(lTmpAdapter.Get(), lLevels[i], IID_PPV_ARGS(device.GetAddressOf()));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			lFeatureLevel = lLevels[i];
			break;
		}
	}

	return result;
}

HRESULT DirectX12Core::PInitializeCommand()
{
	//コマンドアロケータを生成
	commandAllocators.resize(FrameBufferCount);
	for (size_t i = 0; i < FrameBufferCount; ++i)
	{
		result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i])
		);
		if (FAILED(result))
		{
			printf("Failed CreateCommandAllocator");
		}
	}

	//コマンドキューの設定＆生成
	D3D12_COMMAND_QUEUE_DESC lCommandQueueDesc{};
	result = device->CreateCommandQueue(&lCommandQueueDesc, IID_PPV_ARGS(commandQueue.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	//コマンドリストを生成
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(commandList.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	commandList->Close();

	return result;
}

HRESULT DirectX12Core::PCreatDepthBuffer()
{
	//深度バッファ生成
	depthBuff = CreateUniqueDepthStencilBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), DXGI_FORMAT_D32_FLOAT);

	return result;
}

void DirectX12Core::PCreatDescriptorHeap()
{
	BaseDescriptorHeap::SSetDevice(device.Get());

	//RTVデスクリプタヒープの生成
	rtvHeap = CreateUniqueRTVDescriptorHeap();

	BaseBuffer::SSetRTVDescriptorHeap(rtvHeap.get());

	//DSVデスクプリタヒープ生成
	dsvHeap = CreateUniqueDSVDescriptorHeap();

	BaseBuffer::SSetDSVDescriptorHeap(dsvHeap.get());

	//SRVデスクプリタヒープ生成
	srvHeap = CreateUniqueDescriptorHeap();

	BaseBuffer::SSetSRVDescriptorHeap(srvHeap.get());

}

void DirectX12Core::ResourceTransition(ID3D12Resource* resource_, D3D12_RESOURCE_STATES beforeState_, D3D12_RESOURCE_STATES afterState_)
{
	auto lBarrier = CD3DX12_RESOURCE_BARRIER::Transition(resource_, beforeState_, afterState_);
	commandList->ResourceBarrier(1, &lBarrier);
}

void DirectX12Core::DirectXInitialize(float width_, float height_, HWND* hwnd_, IWindowsApp* windowsApp_)
{
	height = height_;
	width = width_;
	handle = hwnd_;
	windowsApp = windowsApp_;

#ifdef _DEBUG
	PEnableDebugLayer();
#endif

	PCheckTearingSupport();

	//DirectX12関連初期化
	if (FAILED(PInitializeDXGIDevice()))
	{
		assert(0);
		return;
	}

#ifdef _DEBUG
	PEnableInfoQueue();
#endif

	PCreatDescriptorHeap();

	if (FAILED(PInitializeCommand()))
	{
		assert(0);
		return;
	}

	if (FAILED(PCreateSwapChain()))
	{
		assert(0);
		return;
	}

	BaseBuffer::SSetDevice(device.Get());
	BaseBuffer::SSetGraphicsCommandList(commandList.Get());

	if (FAILED(PCreatDepthBuffer()))
	{
		assert(0);
		return;
	}

	//ビューポート設定
	viewport.Width = width;
	viewport.Height = height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// シザー矩形設定
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + static_cast<LONG>(width); // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + static_cast<LONG>(height); // 切り抜き座標下
}

void DirectX12Core::PEnableDebugLayer()
{
	Microsoft::WRL::ComPtr <ID3D12Debug1> lDebugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(lDebugController.ReleaseAndGetAddressOf()))))
	{
		lDebugController->EnableDebugLayer();
		lDebugController->SetEnableGPUBasedValidation(true);
	}
}

void DirectX12Core::PEnableInfoQueue()
{
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> lInfoQueue;
	result = device->QueryInterface(IID_PPV_ARGS(&lInfoQueue));
	if (SUCCEEDED(result))
	{
		lInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		lInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		lInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}

	//抑制するエラー
	D3D12_MESSAGE_ID lDenyIds[] =
	{
		/*
		*windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤー
		*相互作用バグによるエラーメッセージ
		*/
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE

	};
		//抑制する表示レベル
	D3D12_MESSAGE_SEVERITY lSeverities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER lFilter{};
	lFilter.DenyList.NumIDs = _countof(lDenyIds);
	lFilter.DenyList.pIDList = lDenyIds;
	lFilter.DenyList.NumSeverities = _countof(lSeverities);
	lFilter.DenyList.pSeverityList = lSeverities;
	//指定したエラーの表示を抑制する
	lInfoQueue->PushStorageFilter(&lFilter);
}

void DirectX12Core::PCheckTearingSupport()
{
	Microsoft::WRL::ComPtr<IDXGIFactory6> lFactory;
	result = CreateDXGIFactory1(IID_PPV_ARGS(&lFactory));

	bool lAllowTearing = FALSE;

	if (SUCCEEDED(result))
	{
		result = lFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &lAllowTearing, sizeof(lAllowTearing));
	}

	tearingSupport = SUCCEEDED(result) && lAllowTearing;
}

DirectX12Core::~DirectX12Core()
{
}

void DirectX12Core::BeginDraw()
{
	BeginCommand();

	//書き込み可能に変更
	swapChain->Transition(bbIndex, D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandList->OMSetRenderTargets(1, &swapChain->GetRenderTargetHandl(bbIndex), false, &depthBuff->GetHandle());

	//3画面クリア
	commandList->ClearRenderTargetView(swapChain->GetRenderTargetHandl(bbIndex), clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(depthBuff->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1, &viewport);

	// シザー矩形設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1, &scissorRect);

}

void DirectX12Core::EndDraw()
{
	//5元に戻す
	swapChain->Transition(bbIndex, D3D12_RESOURCE_STATE_PRESENT);

	ExecuteCommand();

	windowsApp->RenderShouldFalse();
}

void DirectX12Core::ExecuteCommand(bool flip_)
{
	//命令のクローズ
	result = commandList->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* lCommandListts[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, lCommandListts);

	if (flip_)
	{
		//フリップ
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
	}

	//コマンド実行完了を待つ
	swapChain->WaitPreviousFrame();
}

void DirectX12Core::BeginCommand()
{
	//1バックバッファ番号を取得
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	//キューをクリア
	result = commandAllocators[bbIndex]->Reset();
	assert(SUCCEEDED(result));

	//コマンドリストを貯める準備
	result = commandList->Reset(commandAllocators[bbIndex].Get(), nullptr);
	assert(SUCCEEDED(result));
}

void DirectX12Core::SetBackScreenColor(float red_, float green_, float blue_, float alpha_)
{
	clearColor[0] = red_;
	clearColor[1] = green_;
	clearColor[2] = blue_;
	clearColor[3] = alpha_;
}

#pragma region ゲッター

ID3D12Device* DirectX12Core::GetDevice()const
{
	return device.Get();
}

ID3D12GraphicsCommandList* DirectX12Core::GetCommandList()const
{
	return commandList.Get();
}

IDescriptorHeap* DirectX12Core::GetSRVDescriptorHeap()const
{
	return srvHeap.get();
}

IRTVDescriptorHeap* DirectX12Core::GetRTVDescriptorHeap()const
{
	return rtvHeap.get();
}

IDSVDescriptorHeap* DirectX12Core::GetDSVDescriptorHeap()const
{
	return dsvHeap.get();
}
#pragma endregion
