#pragma once
#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<imgui.h>
#include<Windows.h>

#pragma warning(pop)


/// <summary>
/// ウィンドウクラス
/// </summary>
class WindowsApp
{
private:
	//ウィンドウクラス
	WNDCLASSEX wndclassex{};
	//ハンドル
	HWND hwnd;
	//メッセージ
	MSG massege{};
	//横幅
	UINT windowWidth;
	//縦幅
	UINT windowHeight;
	//ウインドウスタイル(サイズ変更できない・最大化できない)
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	//レンダーすべきか
	bool renderShould = false;
	//サイズが変更されたか
	bool isSizeChanged = false;
	char PADING[2];

	RECT windowRect;

public:
	
	//サイズ取得用構造体
	struct WindowsSize
	{
		//横幅
		UINT width;
		//縦幅
		UINT height;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static WindowsApp* GetInstance();

	static WindowsSize GetWindowsSize();

	/// <summary>
	/// ウィンドウ生成
	/// </summary>
	/// <param name="title">タイトル(ワイド文字)</param>
	/// <param name="width">ウィンドウ横幅</param>
	/// <param name="height">ウィンドウ横幅</param>
	void CreatWindow(const wchar_t* title = L"DirectXGame", UINT width = 1280, UINT height = 720);
	
	/// <summary>
	/// メッセージ処理
	/// </summary>
	bool MessageWindow();

	/// <summary>
	/// 後始末
	/// </summary>
	void Break();

	/// <summary>
	/// ウィンドウクラスを取得
	/// </summary>
	WNDCLASSEX* GetWndclassex();

	/// <summary>
	/// ハンドルを取得
	/// </summary>
	HWND* GetHwnd();

	/// <summary>
	/// ウィンドサイズを取得
	/// </summary>
	WindowsSize GetWindowSize();

	/// <summary>
	/// ボーダーレスのフルスクリーン
	/// </summary>
	void ShowFullScreen();

	/// <summary>
	/// ボーダーレスのフルスクリーン
	/// </summary>
	void ShowDefaultWindow(LONG h, LONG w);

	/// <summary>
	/// 描画するか(画面)
	/// </summary>
	const bool& RenderShould();

	/// <summary>
	/// フラグをfalseに
	/// </summary>
	void RenderShouldFalse();

	/// <summary>
	/// サイズが変更されたか
	/// </summary>
	const bool& IsSizeChanged();

	/// <summary>
	/// フラグをfalseに
	/// </summary>
	void IsSizeChangedFalse();

	/// <summary>
	/// 現在のウィンドウサイズ
	/// </summary>
	/// <returns></returns>
	WindowsSize GetNowWindowSize();

private:
	//コンストラクタ・デストラクタ
	WindowsApp() = default;
	~WindowsApp() = default;
};