#pragma once

#include<WindowsApp.h>
#include<DirectX12Core.h>

/// <summary>
/// IMGUIマネージャー
/// </summary>
class IImGuiManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="windowsApp_">ウィンドウズ</param>
	/// <param name="directX12Core_">directX</param>
	virtual void Initialize(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_) =0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	virtual void Bigin() = 0;

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	virtual void End() = 0;

	/// <summary>
	/// 画面への描画
	/// </summary>
	virtual void Draw() = 0;

	IImGuiManager() = default;
	virtual ~IImGuiManager() = default;
};

/// <summary>
/// IMGUIマネージャーの生成(ユニーク)
/// </summary>
/// <param name="windowsApp_">ウィンドウズ</param>
/// <param name="directX12Core_">directX</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IImGuiManager> CreateUniqueImGuiManager(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_);

/// <summary>
/// IMGUIマネージャーの生成(シェアード)
/// </summary>
/// <param name="windowsApp_">ウィンドウズ</param>
/// <param name="directX12Core_">directX</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IImGuiManager> CreateSharedImGuiManager(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_);
