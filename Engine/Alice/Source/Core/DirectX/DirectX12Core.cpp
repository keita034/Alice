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

#include<BaseBuffer.h>
#include<BaseDescriptorHeap.h>

HRESULT DirectX12Core::PCreateSwapChain()
{
	swapChainDesc.Width = static_cast< UINT >( width );//横幅
	swapChainDesc.Height = static_cast< UINT >( height );//縦幅
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

	Microsoft::WRL::ComPtr<IDXGISwapChain1> lTmpSwapChain;
	ID3D12Device* lDevice = mainAdapters->GetDevice()->Get();

	result = multiAdapters->GetFactory()->CreateSwapChainForHwnd(mainAdapters->GetGraphicCommandQueue(),*handle,&swapChainDesc,&lFsDesc,nullptr,lTmpSwapChain.ReleaseAndGetAddressOf());
	swapChain = CreateUniqueSwapChain(lDevice,lTmpSwapChain,mainAdapters->GetGraphicCommandQueue());

	return result;
}

size_t DirectX12Core::GetBackBufferCount() const
{
	return swapChain->GetBackBufferCount();
}

bool DirectX12Core::SetWindowType(WindowMode mode_,uint32_t windowHeight_,uint32_t windowWidth_)
{
	switch ( mode_ )
	{
	case WindowMode::WINDOW:

		windowMode = WindowMode::WINDOW;

		// 処理の完了を待ってからサイズ変更の処理を開始.
		multiAdapters->WaitPreviousFrame();

		swapChain->SetFullScreen(false);

		windowsApp->ShowDefaultWindow(static_cast< uint64_t >( windowHeight_ ),static_cast< uint64_t >( windowWidth_ ));

		IWindowsApp::WindowsSize lWinSize = windowsApp->GetNowWindowSize();

		width = static_cast< float >( lWinSize.width );
		height = static_cast< float >( lWinSize.height );

		swapChain->ResizeBuffers(lWinSize.width,lWinSize.height);

		//レンダーサイズ変更
		RenderSizeChanged(lWinSize.width,lWinSize.height);

		break;

	case WindowMode::FULLSCREEN:


		break;

	case WindowMode::BRDERLESSWINDOW:

		if ( windowMode != WindowMode::BRDERLESSWINDOW )
		{
			windowMode = WindowMode::BRDERLESSWINDOW;

			windowsApp->ShowFullScreen();

			IWindowsApp::WindowsSize lSize = windowsApp->GetNowWindowSize();

			width = static_cast< float >( lSize.width );
			height = static_cast< float >( lSize.height );

			swapChain->ResizeBuffers(lSize.width,lSize.height);

			//レンダーサイズ変更
			RenderSizeChanged(lSize.width,lSize.height);
		}

		break;
	default:
		break;
	}

	return true;
}

void DirectX12Core::RenderSizeChanged(uint32_t width_,uint32_t height_)
{
	width = static_cast< float >( width_ );
	height = static_cast< float >( height_ );

	//バッファのサイズ変更
	swapChain->ResizeBuffers(width_,height_);

	// デプスバッファの作り直し.
	depthBuff->Resize(width_,height_);

	bbIndex = swapChain->GetCurrentBackBufferIndex();

	viewport.Width = width;
	viewport.Height = height;

	scissorRect.right = scissorRect.left + static_cast< LONG >( width );
	scissorRect.bottom = scissorRect.top + static_cast< LONG >( height );

}

HRESULT DirectX12Core::PInitializeDXGIDevice()
{
	return result;
}

HRESULT DirectX12Core::PInitializeCommand()
{
	return result;
}

HRESULT DirectX12Core::PCreatDepthBuffer()
{
	//深度バッファ生成
	depthBuff = CreateUniqueDepthStencilBuffer(static_cast< uint32_t >( width ),static_cast< uint32_t >( height ),DXGI_FORMAT_D32_FLOAT);

	return result;
}

void DirectX12Core::PCreatDescriptorHeap()
{

}

void DirectX12Core::ResourceTransition(ID3D12Resource* resource_,D3D12_RESOURCE_STATES beforeState_,D3D12_RESOURCE_STATES afterState_)
{
	mainAdapters->ResourceTransition(resource_,beforeState_,afterState_);
}

void DirectX12Core::DirectXInitialize(float width_,float height_,HWND* hwnd_,IWindowsApp* windowsApp_)
{
	height = height_;
	width = width_;
	handle = hwnd_;
	windowsApp = windowsApp_;

#ifdef _DEBUG
	PEnableDebugLayer();
#endif

	PCheckTearingSupport();

	multiAdapters = CreateUniqueMultiAdapters();
	mainAdapters = multiAdapters->GetMainAdapter();
	subAdapters = multiAdapters->GetSubAdapter();

#ifdef _DEBUG
	PEnableInfoQueue();
#endif

	BaseBuffer::SSetMultiAdapters(multiAdapters.get());

	if ( FAILED(PCreateSwapChain()) )
	{
		assert(0);
		return;
	}


	if ( FAILED(PCreatDepthBuffer()) )
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
	scissorRect.right = scissorRect.left + static_cast< LONG >( width ); // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + static_cast< LONG >( height ); // 切り抜き座標下
}

void DirectX12Core::PEnableDebugLayer()
{
	Microsoft::WRL::ComPtr <ID3D12Debug1> lDebugController;
	if ( SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(lDebugController.ReleaseAndGetAddressOf()))) )
	{
		lDebugController->EnableDebugLayer();
		lDebugController->SetEnableGPUBasedValidation(true);
	}
}

void DirectX12Core::PEnableInfoQueue()
{
	{
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> lInfoQueue;
		ID3D12Device* lDevice = mainAdapters->GetDevice()->Get();

		result = lDevice->QueryInterface(IID_PPV_ARGS(&lInfoQueue));
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

	{
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> lInfoQueue;
		ID3D12Device* lDevice = multiAdapters->GetSubAdapter()->GetDevice()->Get();

		result = lDevice->QueryInterface(IID_PPV_ARGS(&lInfoQueue));
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
}

void DirectX12Core::PCheckTearingSupport()
{
	Microsoft::WRL::ComPtr<IDXGIFactory6> lFactory;
	result = CreateDXGIFactory1(IID_PPV_ARGS(&lFactory));

	bool lAllowTearing = FALSE;

	if ( SUCCEEDED(result) )
	{
		result = lFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,&lAllowTearing,sizeof(lAllowTearing));
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
	swapChain->Transition(bbIndex,D3D12_RESOURCE_STATE_RENDER_TARGET);

	ID3D12GraphicsCommandList* commandList = mainAdapters->GetGraphicCommandList();

	commandList->OMSetRenderTargets(1,&swapChain->GetRenderTargetHandl(bbIndex),false,&depthBuff->GetHandle());

	//3画面クリア
	commandList->ClearRenderTargetView(swapChain->GetRenderTargetHandl(bbIndex),clearColor,0,nullptr);
	commandList->ClearDepthStencilView(depthBuff->GetHandle(),D3D12_CLEAR_FLAG_DEPTH,1.0f,0,0,nullptr);

	// ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1,&viewport);

	// シザー矩形設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1,&scissorRect);

}

void DirectX12Core::EndDraw()
{
	//5元に戻す
	swapChain->Transition(bbIndex,D3D12_RESOURCE_STATE_PRESENT);

	ExecuteCommand();

	windowsApp->RenderShouldFalse();
}

void DirectX12Core::ExecuteCommand(bool flip_)
{
	multiAdapters->ExecuteCommand();

	if ( flip_ )
	{
		//フリップ
		result = swapChain->Present(1,0);
		assert(SUCCEEDED(result));
	}

	//コマンド実行完了を待つ
	multiAdapters->WaitPreviousFrame();
}

void DirectX12Core::BeginCommand()
{
	//1バックバッファ番号を取得
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	multiAdapters->BeginCommand(bbIndex);
}

void DirectX12Core::GraphicBeginCommand(AdaptersIndex index_)
{
	//1バックバッファ番号を取得
	bbIndex = swapChain->GetCurrentBackBufferIndex();

	if (  index_ == AdaptersIndex::MAIN )
	{
		mainAdapters->GraphicCommandListReset(bbIndex);
	}
	else
	{
		subAdapters->GraphicCommandListReset(bbIndex);
	}
}

void DirectX12Core::GraphicExecuteCommand(AdaptersIndex index_)
{
	if ( index_ == AdaptersIndex::MAIN )
	{
		mainAdapters->GraphicCommandListExecute();
		mainAdapters->GraphicWaitPreviousFrame();
	}
	else
	{
		subAdapters->GraphicCommandListExecute();
		subAdapters->GraphicWaitPreviousFrame();
	}

}

void DirectX12Core::SetBackScreenColor(float red_,float green_,float blue_,float alpha_)
{
	clearColor[ 0 ] = red_;
	clearColor[ 1 ] = green_;
	clearColor[ 2 ] = blue_;
	clearColor[ 3 ] = alpha_;
}

#pragma region ゲッター

IDevice* DirectX12Core::GetDevice()const
{
	return mainAdapters->GetDevice();
}

ICommandList* DirectX12Core::GetCommandList()const
{
	return mainAdapters->GetCommandList();
}

ISRVDescriptorHeap* DirectX12Core::GetSRVDescriptorHeap()const
{
	return mainAdapters->GetSRVDescriptorHeap();
}

IRTVDescriptorHeap* DirectX12Core::GetRTVDescriptorHeap()const
{
	return mainAdapters->GetRTVDescriptorHeap();
}

IDSVDescriptorHeap* DirectX12Core::GetDSVDescriptorHeap()const
{
	return mainAdapters->GetDSVDescriptorHeap();
}
IMultiAdapters* DirectX12Core::GetMultiAdapters() const
{
	return multiAdapters.get();
}
ISwapChain* DirectX12Core::GetSwapChain() const
{
	return swapChain.get();
}
#pragma endregion
