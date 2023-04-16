#pragma once

#include<DirectX12Core.h>
#include<WindowsApp.h>

//ImGUIの管理
class ImGuiManager
{

private:

	DirectX12Core* directX12Core;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WindowsApp* windowsApp, DirectX12Core* core);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Bigin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	void Draw();
};

