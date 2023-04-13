#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<vector>
#include<memory>
#include<dxgi1_6.h>

#pragma warning(pop)

#include<RenderTargetBuffer.h>
#include<RTVDescriptorHeap.h>

class SwapChain
{
private:

	//デバイス
	ID3D12Device* device;

	//スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	//バッファの数
	static const UINT FrameBufferCount = 2;

	//バックバッファ
	std::vector<std::unique_ptr<RenderTargetBuffer>> backBuffers;

	//フェンスの生成
	std::vector<UINT64> fenceValues;
	UINT64 fenceVal;

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
	/// <param name="dev">デバイス</param>
	/// <param name="heap">レンダーターゲット用ディスクリプタヒープ</param>
	/// <param name="useHDR"></param>
	SwapChain(ID3D12Device* dev, Microsoft::WRL::ComPtr<IDXGISwapChain1>& swapchain, ID3D12CommandQueue* queue, bool useHDR = false);

	~SwapChain();

	/// <summary>
	/// リサイズ
	/// </summary>
	void ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters);

	/// <summary>
	/// フルスクリーンか
	/// </summary>
	bool IsFullScreen() const;

	/// <summary>
	/// フルスクリーンをセット
	/// </summary>
	void SetFullScreen(bool toFullScreen);

	/// <summary>
	/// バッファサイズ変更
	/// </summary>
	void ResizeBuffers(UINT width, UINT height);

	void WaitPreviousFrame();

	void WaitForGpu();


	HRESULT Present(UINT SyncInterval, UINT Flags);

	void Transition(size_t index, D3D12_RESOURCE_STATES resourceStates);

	UINT GetCurrentBackBufferIndex() const;

	RenderTargetBuffer* GetRenderTarget(size_t index)const;

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetRenderTargetHandl(size_t index);

	size_t GetBackBufferCount() const;

private:

	SwapChain() = delete;
};

