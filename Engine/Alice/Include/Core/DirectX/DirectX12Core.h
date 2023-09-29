#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)

#include<string>

#pragma warning(pop)

#include<WindowsApp.h>
#include<DepthStencilBuffer.h>
#include<SwapChain.h>
#include<RTVDescriptorHeap.h>
#include<DescriptorHeap.h>
#include<DSVDescriptorHeap.h>
#include<MultiAdapters.h>

enum class WindowMode
{
	//ウィンドウ
	WINDOW,
	//フルスクリーン
	FULLSCREEN,
	//ボーダーレスウィンドウ
	BRDERLESSWINDOW
};

/// <summary>
/// コア
/// </summary>
class DirectX12Core
{
private:

	HWND* handle;
	IWindowsApp* windowsApp = nullptr;

	std::unique_ptr <IMultiAdapters>multiAdapters;
	
	IAdapter* mainAdapters = nullptr;

	std::unique_ptr<IDepthStencilBuffer> depthBuff;

	//スワップチェイン
	std::unique_ptr<ISwapChain> swapChain;

	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	uint32_t bbIndex;

	uint32_t FrameBufferCount = 2;

	DXGI_FORMAT surfaceFormat;

	//クリアーカラー
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f};

	//ビューポート
	D3D12_VIEWPORT viewport{};

	//シザー矩形
	D3D12_RECT scissorRect{};
	
	float height;

	float width;

	WindowMode windowMode;
	HRESULT result = S_OK;

	bool tearingSupport;
	int8_t PADING[3];

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="width_">横幅</param>
	/// <param name="height_">縦幅</param>
	void DirectXInitialize(float width_, float height_, HWND* hwnd_, IWindowsApp* windowsApp_);

	/// <summary>
	/// 描画準備
	/// </summary>
	void BeginDraw();

	/// <summary>
	/// 描画後始末
	/// </summary>
	void EndDraw();

	/// <summary>
	/// コマンド後始末
	/// </summary>
	void ExecuteCommand(bool flip_ = true);

	void BeginCommand();

	void GraphicBeginCommand();
	void GraphicExecuteCommand();

	//背景の色変え(RGBA)
	void SetBackScreenColor(float red_, float green_, float blue_, float alpha_);

	/// <summary>
	/// デバイスの取得
	/// </summary>
	IDevice* GetDevice()const;

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	ICommandList* GetCommandList()const;

	/// <summary>
	/// SRV,CBV,URV用デスクプリタヒープ取得
	/// </summary>
	ISRVDescriptorHeap* GetSRVDescriptorHeap()const;

	/// <summary>
	/// RTV用デスクプリタヒープ取得
	/// </summary>
	IRTVDescriptorHeap* GetRTVDescriptorHeap()const;

	/// <summary>
	/// DSV用デスクプリタヒープ取得
	/// </summary>
	IDSVDescriptorHeap* GetDSVDescriptorHeap()const;

	/// <summary>
	/// リソースの状態を変える
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="beforeState">今の状態</param>
	/// <param name="afterState">変えたい状態</param>
	void ResourceTransition(ID3D12Resource* resource_, D3D12_RESOURCE_STATES beforeState_, D3D12_RESOURCE_STATES afterState_);

	/// <summary>
	/// バックバッファの数を取得
	/// </summary>
	/// <returns>バックバッファの数</returns>
	size_t GetBackBufferCount() const;

	/// <summary>
	/// ウィンドウモードをセット
	/// </summary>
	/// <param name="mode">モード</param>
	/// <param name="windowHeight">ウィンドウ縦幅(ウィンドウモードの時のみ有効)</param>
	/// <param name="windowWidth">ウィンドウ横幅(ウィンドウモードの時のみ有効)</param>
	/// <returns>成功したか</returns>
	bool SetWindowType(WindowMode mode_, uint32_t windowHeight_ = 720, uint32_t windowWidth_ = 1280);

	void RenderSizeChanged(uint32_t width_, uint32_t height_);

	//コンストラクタ・デストラクタ
	DirectX12Core() = default;
	~DirectX12Core();

private:

	//DXGIまわり初期化
	HRESULT PInitializeDXGIDevice();

	//スワップチェインの生成
	HRESULT PCreateSwapChain();

	//コマンドまわり初期化
	HRESULT PInitializeCommand();

	//深度関係生成
	HRESULT PCreatDepthBuffer();

	//デスクプリタヒープの生成
	void PCreatDescriptorHeap();

	//デバッグレイヤーを有効にする
	void PEnableDebugLayer();

	//インフォキューを有効にする
	void PEnableInfoQueue();

	void PCheckTearingSupport();

	//コピーコンストラクタ・代入演算子削除
	DirectX12Core& operator=(const DirectX12Core&) = delete;
	DirectX12Core(const DirectX12Core&) = delete;
};