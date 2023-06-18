#include "WindowsApp.h"

#pragma comment( lib, "shell32.lib" )

#pragma once
#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<imgui.h>
#pragma warning(pop)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{

	case WM_PAINT:
		return 0;

	case WM_SIZE:
		return 0;

	//ウィンドウが破棄された
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/// <summary>
/// ウィンドウクラス
/// </summary>
class WindowsApp :public IWindowsApp
{
private:
	//ウィンドウクラス
	WNDCLASSEX wndclassex{};
	//ハンドル
	HWND hwnd;
	//メッセージ
	MSG massege{};
	//横幅
	uint32_t windowWidth;
	//縦幅
	uint32_t windowHeight;
	//ウインドウスタイル(サイズ変更できない・最大化できない)
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT windowRect;

	//レンダーすべきか
	bool renderShould = false;
	//サイズが変更されたか
	bool isSizeChanged = false;
	int16_t PADING;

public:

	/// <summary>
	/// ウィンドウ生成
	/// </summary>
	void CreatWindow(const std::wstring& title_, uint32_t width_, uint32_t height_) override;

	/// <summary>
	/// メッセージ処理
	/// </summary>
	bool MessageWindow() override;

	/// <summary>
	/// 後始末
	/// </summary>
	void Break() override;

	/// <summary>
	/// ウィンドウクラスを取得
	/// </summary>
	WNDCLASSEX* GetWndclassex() override;

	/// <summary>
	/// ハンドルを取得
	/// </summary>
	HWND* GetHwnd() override;

	/// <summary>
	/// ウィンドサイズを取得
	/// </summary>
	WindowsSize GetWindowSize() override;

	/// <summary>
	/// ボーダーレスのフルスクリーン
	/// </summary>
	void ShowFullScreen() override;

	/// <summary>
	/// ボーダーレスのフルスクリーン
	/// </summary>
	void ShowDefaultWindow(uint64_t width_, uint64_t height_) override;

	/// <summary>
	/// 描画するか(画面)
	/// </summary>
	const bool& RenderShould() override;

	/// <summary>
	/// フラグをfalseに
	/// </summary>
	void RenderShouldFalse() override;

	/// <summary>
	/// サイズが変更されたか
	/// </summary>
	const bool& IsSizeChanged() override;

	/// <summary>
	/// フラグをfalseに
	/// </summary>
	void IsSizeChangedFalse() override;

	/// <summary>
	/// 現在のウィンドウサイズ
	/// </summary>
	/// <returns></returns>
	WindowsSize GetNowWindowSize() override;

	//コンストラクタ・デストラクタ
	WindowsApp() = default;
	~WindowsApp() = default;
};

const bool& WindowsApp::RenderShould()
{
	return renderShould;
}

void WindowsApp::RenderShouldFalse()
{
	renderShould = false;
}

const bool& WindowsApp::IsSizeChanged()
{
	return isSizeChanged;
}

void WindowsApp::IsSizeChangedFalse()
{
	isSizeChanged = false;
}

WindowsApp::WindowsSize WindowsApp::GetNowWindowSize()
{
	RECT rect{};
	GetClientRect(hwnd, &rect);
	windowHeight = static_cast<UINT>(rect.bottom - rect.top);
	windowWidth = static_cast<UINT>(rect.right - rect.left);
	return WindowsSize(windowWidth, windowHeight);
}


void WindowsApp::CreatWindow(const std::wstring& title_, uint32_t width_, uint32_t height_)
{
	windowHeight = height_;
	windowWidth = width_;

	//ウインドウクラスの設定
	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロシージャを設定
	wndclassex.lpszClassName = title_.c_str();//ウインドウクラス名
	wndclassex.hInstance = GetModuleHandle(nullptr);//ウインドウハンドル
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定
	wndclassex.hIcon = (HICON)LoadImage(NULL, L"ICON.ICO", IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
	//wndclassex.hIconSm = (HICON)LoadImage(NULL, L"ICON1.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	//ウインドウクラスをOSに登録する
	RegisterClassEx(&wndclassex);


	//ウインドウサイズ{X座標　Y座標 横幅　縦幅}
	windowRect = { 0,0,static_cast<LONG>(windowWidth),static_cast<LONG>(windowHeight) };
	//自動でサイズを補正する
	AdjustWindowRect(&windowRect, windowStyle, false);

	//ウインドウオブジェクトの生成
	hwnd = CreateWindow(wndclassex.lpszClassName,//クラス名
		title_.c_str(),				//タイトルバーの文字
		windowStyle,		//標準的なウインドウスタイル
		CW_USEDEFAULT,				//表示X座標(OSに任せる)
		CW_USEDEFAULT,				//表示Y座標(OSに任せる)
		windowRect.right - windowRect.left,		//ウインドウ横幅
		windowRect.bottom - windowRect.top,		//ウインドウ縦幅
		nullptr,					//親ウインドウハンドル
		nullptr,					//メニューハンドル
		wndclassex.hInstance,				//呼び出しアプリケーションハンドル
		nullptr);					//オプション

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_NORMAL);

#ifdef _DEBUG

	system("AliceCore/Debug/CommandPromptUTF-8.bat");

#endif _DEBUG

}

bool WindowsApp::MessageWindow()
{
	//メッセージがある？
	if (PeekMessage(&massege, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&massege); //キー入力メッセージの処理
		DispatchMessage(&massege); //プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (massege.message == WM_QUIT)
	{
		return false;
	}
	return true;
}

void WindowsApp::Break()
{
	//ウインドウクラスを登録解除
	UnregisterClass(wndclassex.lpszClassName, wndclassex.hInstance);
}

WNDCLASSEX* WindowsApp::GetWndclassex()
{
	return &wndclassex;
}

HWND* WindowsApp::GetHwnd()
{
	return &hwnd;
}

WindowsApp::WindowsSize WindowsApp::GetWindowSize()
{
	return { windowWidth,windowHeight };
}

void WindowsApp::ShowFullScreen()
{
	SetWindowLong(hwnd, GWL_STYLE, static_cast<LONG>(windowStyle) & ~(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU));

	DEVMODE lDevMode = {};
	lDevMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lDevMode);

	windowRect = {
		lDevMode.dmPosition.x,
		lDevMode.dmPosition.y,
		lDevMode.dmPosition.x + static_cast<LONG>(lDevMode.dmPelsWidth),
		lDevMode.dmPosition.y + static_cast<LONG>(lDevMode.dmPelsHeight)
	};

	SetWindowPos(
		hwnd,
		HWND_TOP,
		windowRect.left,
		windowRect.top,
		windowRect.right,
		windowRect.bottom,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_NORMAL);
}

void WindowsApp::ShowDefaultWindow(uint64_t width_, uint64_t height_)
{
	SetWindowLong(hwnd, GWL_STYLE, static_cast<LONG>(windowStyle));

	RECT lRX; //ウィンドウ領域
	RECT lCX; //クライアント領域
	GetWindowRect(hwnd, &lRX);
	GetClientRect(hwnd, &lCX);

	uint64_t lNewWidth = width_ + (lRX.right - lRX.left) - (lCX.right - lCX.left);
	uint64_t lNewHeight = height_ + (lRX.bottom - lRX.top) - (lCX.bottom - lCX.top);

	uint64_t lDisplayWidth = static_cast<uint64_t>(GetSystemMetrics(SM_CXSCREEN));
	uint64_t lDisplayHeight = static_cast<uint64_t>(GetSystemMetrics(SM_CYSCREEN));

	windowRect.left = static_cast<LONG>((lDisplayWidth - lNewWidth) / 2);
	windowRect.top = static_cast<LONG>((lDisplayHeight - lNewHeight) / 2);
	windowRect.right = static_cast<LONG>(lNewWidth);
	windowRect.bottom = static_cast<LONG>(lNewHeight);

	windowHeight = static_cast<UINT>(height_);
	windowWidth = static_cast<UINT>(width_);

	SetWindowPos(hwnd, HWND_NOTOPMOST,
		windowRect.left,
		windowRect.top,
		windowRect.right,
		windowRect.bottom,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}

std::unique_ptr<IWindowsApp> CreateUniqueWindowsApp(const std::wstring& title_, uint32_t width_, uint32_t height_)
{
	std::unique_ptr<IWindowsApp> lWindowsApp = std::make_unique<WindowsApp>();
	lWindowsApp->CreatWindow(title_, width_, height_);
	return std::move(lWindowsApp);
}

std::shared_ptr<IWindowsApp> CreateSharedWindowsApp(const std::wstring& title_, uint32_t width_, uint32_t height_)
{
	std::shared_ptr<IWindowsApp> lWindowsApp = std::make_shared<WindowsApp>();
	lWindowsApp->CreatWindow(title_, width_, height_);
	return lWindowsApp;
}
