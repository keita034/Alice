#pragma once
#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)


#include<WindowsApp.h>
#include<DescriptorHeap.h>
#include<DirectX12Core.h>

//ImGUIの管理
class ImGuiManager
{

private:

	DirectX12Core* directX12Core;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IWindowsApp* windowsApp, DirectX12Core* core);

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

