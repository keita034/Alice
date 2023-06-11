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

	HRESULT result;
	char PADING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> commandAllocators;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	//RTV用のデスクプリタヒープ
	std::unique_ptr<IRTVDescriptorHeap> rtvHeap;

	//SRV用のデスクプリタヒープ
	std::unique_ptr<IDescriptorHeap> srvHeap;

	//深度バッファ
	std::unique_ptr<IDepthStencilBuffer> depthBuff;

	//DSV用のデスクプリタヒープ
	std::unique_ptr<IDSVDescriptorHeap> dsvHeap;

	//スワップチェイン
	std::unique_ptr<ISwapChain> swapChain;

	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	UINT bbIndex;

	UINT FrameBufferCount = 2;

	DXGI_FORMAT surfaceFormat;

	//クリアーカラー
	//FLOAT clearColor[4] = { 0.1f, 0.25f, 0.5f, 0.0f };
	FLOAT clearColor[4] = { 1.0f,1.0f,1.0f,1.0f};

	//ビューポート
	D3D12_VIEWPORT viewport{};

	//シザー矩形
	D3D12_RECT scissorRect{};

	static DirectX12Core* DirectX12Core_;
	
	bool tearingSupport;
	char PADING2[3]{};

	float height;
	char PADING3[4]{};

	float width;
	char PADING4[4]{};

	HWND* handle;
	IWindowsApp* windowsApp = nullptr;

	WindowMode windowMode;
	char PADING5[4]{};
public:

	//シングルトンインスタンスの取得
	static DirectX12Core* GetInstance();

	/// <summary>
	/// デバイスの取得(スタティック)
	/// </summary>
	static Microsoft::WRL::ComPtr<ID3D12Device> GetDeviceSta();

	/// <summary>
	/// コマンドリストの取得(スタティック)
	/// </summary>
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandListSta();
	
	/// <summary>
	/// リソースの状態を変える
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="beforeState">今の状態</param>
	/// <param name="afterState">変えたい状態</param>
	static void ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="h">縦幅</param>
	/// <param name="w">横幅</param>
	void DirectXInitialize(float h, float w, HWND* hwnd, IWindowsApp* windows);

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
	void ExecuteCommand(bool flip = true);

	void BeginCommand();

	/// <summary>
	/// 開放
	/// </summary>
	void Destroy();

	//背景の色変え(RGBA)
	void SetBackScreenColor(float red, float green, float blue, float alpha);

	/// <summary>
	/// デバイスの取得
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice();

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();

	/// <summary>
	/// SRV,CBV,URV用デスクプリタヒープ取得
	/// </summary>
	IDescriptorHeap* GetSRVDescriptorHeap();

	/// <summary>
	/// RTV用デスクプリタヒープ取得
	/// </summary>
	IRTVDescriptorHeap* GetRTVDescriptorHeap();

	/// <summary>
	/// DSV用デスクプリタヒープ取得
	/// </summary>
	IDSVDescriptorHeap* GetDSVDescriptorHeap();

	/// <summary>
	/// リソースの状態を変える
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="beforeState">今の状態</param>
	/// <param name="afterState">変えたい状態</param>
	void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

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
	bool SetWindowType(WindowMode mode, UINT windowHeight = 720, UINT windowWidth = 1280);

	void RenderSizeChanged(UINT width, UINT height);

private:

	//DXGIまわり初期化
	HRESULT InitializeDXGIDevice();

	//スワップチェインの生成
	HRESULT CreateSwapChain();

	//コマンドまわり初期化
	HRESULT InitializeCommand();

	//深度関係生成
	HRESULT CreatDepthBuffer();

	//デスクプリタヒープの生成
	void CreatDescriptorHeap();

	//デバッグレイヤーを有効にする
	void EnableDebugLayer();

	//インフォキューを有効にする
	void EnableInfoQueue();

	//
	void CheckTearingSupport();

	//コンストラクタ・デストラクタ
	DirectX12Core() = default;
	~DirectX12Core();

	//コピーコンストラクタ・代入演算子削除
	DirectX12Core& operator=(const DirectX12Core&) = delete;
	DirectX12Core(const DirectX12Core&) = delete;
};