#pragma once
#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)
#pragma warning(disable: 4514)

#include<Windows.h>
#include<widemath.h>
#include<string>
#include<memory>

#pragma warning(pop)

/// <summary>
/// ウィンドウクラス
/// </summary>
class IWindowsApp
{
public:

	//サイズ取得用構造体
	struct WindowsSize
	{
		//横幅
		uint32_t width;
		//縦幅
		uint32_t height;
	};

	/// <summary>
	/// ウィンドウ生成
	/// </summary>
	/// <param name="title">タイトル(ワイド文字)</param>
	/// <param name="width">ウィンドウ横幅</param>
	/// <param name="height">ウィンドウ横幅</param>
	virtual void CreatWindow(const std::wstring& title_, uint32_t width_, uint32_t height_) = 0;

	/// <summary>
	/// メッセージ処理
	/// </summary>
	virtual bool MessageWindow() = 0;

	/// <summary>
	/// 後始末
	/// </summary>
	virtual void Break() = 0;

	/// <summary>
	/// ウィンドウクラスを取得
	/// </summary>
	virtual WNDCLASSEX* GetWndclassex() = 0;

	/// <summary>
	/// ハンドルを取得
	/// </summary>
	virtual HWND* GetHwnd() = 0;

	/// <summary>
	/// ウィンドサイズを取得
	/// </summary>
	virtual WindowsSize GetWindowSize() = 0;

	/// <summary>
	/// ボーダーレスのフルスクリーン
	/// </summary>
	virtual void ShowFullScreen()= 0;

	/// <summary>
	/// ボーダーレスのフルスクリーン
	/// </summary>
	virtual void ShowDefaultWindow(uint64_t width_, uint64_t height_) = 0;

	/// <summary>
	/// 描画するか(画面)
	/// </summary>
	virtual const bool& RenderShould() = 0;

	/// <summary>
	/// フラグをfalseに
	/// </summary>
	virtual void RenderShouldFalse() = 0;

	/// <summary>
	/// サイズが変更されたか
	/// </summary>
	virtual const bool& IsSizeChanged() = 0;

	/// <summary>
	/// フラグをfalseに
	/// </summary>
	virtual void IsSizeChangedFalse() = 0;

	/// <summary>
	/// 現在のウィンドウサイズ
	/// </summary>
	/// <returns></returns>
	virtual WindowsSize GetNowWindowSize() = 0;

	//コンストラクタ・デストラクタ
	IWindowsApp() = default;
	virtual ~IWindowsApp() = default;
};

/// <summary>
/// ウィンドウクラスの生成(ユニーク)
/// </summary>
/// <param name="title">タイトル(ワイド文字)</param>
/// <param name="width">ウィンドウ横幅</param>
/// <param name="height">ウィンドウ横幅</param>
std::unique_ptr<IWindowsApp> CreateUniqueWindowsApp(const std::wstring& title_ = L"DirectXGame", uint32_t width_ = 1280, uint32_t height_ = 720);

/// <summary>
///  ウィンドウクラスの生成(シェアード)
/// </summary>
/// <param name="title">タイトル(ワイド文字)</param>
/// <param name="width">ウィンドウ横幅</param>
/// <param name="height">ウィンドウ横幅</param>
std::shared_ptr<IWindowsApp> CreateSharedWindowsApp(const std::wstring& title_ = L"DirectXGame", uint32_t width_ = 1280, uint32_t height_ = 720);