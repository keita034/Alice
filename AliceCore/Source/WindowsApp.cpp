#include "WindowsApp.h"

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
	switch (msg) {

	//描画して欲しい
	case WM_PAINT:
		//GetInstance()->renderShould = true;
		return 0;

	case WM_SIZE:
		/*GetInstance()->isSizeChanged = true;*/
		return 0;

	//ウィンドウが破棄された
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

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

WindowsApp* WindowsApp::GetInstance()
{
	//シングルトン
	static	WindowsApp windowsApp_;
	return &windowsApp_;
}

WindowsApp::WindowsSize WindowsApp::GetWindowsSize()
{
	return GetInstance()->GetWindowSize();
}

void WindowsApp::CreatWindow(const wchar_t* title, UINT width, UINT height)
{
	windowHeight = height;
	windowWidth = width;

	//ウインドウクラスの設定
	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロシージャを設定
	wndclassex.lpszClassName = title;//ウインドウクラス名
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
		title,				//タイトルバーの文字
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

	DEVMODE devMode = {};
	devMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

	windowRect = {
		devMode.dmPosition.x,
		devMode.dmPosition.y,
		devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
		devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)
	};

	SetWindowPos(
		hwnd,
		HWND_TOPMOST,
		windowRect.left,
		windowRect.top,
		windowRect.right,
		windowRect.bottom,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_NORMAL);
}

void WindowsApp::ShowDefaultWindow(LONG h, LONG w)
{
	SetWindowLong(hwnd, GWL_STYLE, static_cast<LONG>(windowStyle));
	
	RECT rx; //ウィンドウ領域
	RECT cx; //クライアント領域
	GetWindowRect(hwnd, &rx);
	GetClientRect(hwnd, &cx);

	 LONG newWidth = w + (rx.right - rx.left) - (cx.right - cx.left);
	 LONG newHeight = h + (rx.bottom - rx.top) - (cx.bottom - cx.top);

	 LONG displayWidth = GetSystemMetrics(SM_CXSCREEN);
	 LONG displayHeight = GetSystemMetrics(SM_CYSCREEN);

	windowRect.left = (displayWidth - newWidth) / 2;
	windowRect.top = (displayHeight - newHeight) / 2;
	windowRect.right = newWidth;
	windowRect.bottom = newHeight;

	LONG left, top;
	left = windowRect.left;
	top = windowRect.top;

	//自動でサイズを補正する
	//AdjustWindowRect(&windowRect, windowStyle, false);

	windowHeight = static_cast<UINT>(h);
	windowWidth = static_cast<UINT>(w);

	SetWindowPos(hwnd, HWND_NOTOPMOST,
		windowRect.left,
		windowRect.top,
		windowRect.right,
		windowRect.bottom,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}
