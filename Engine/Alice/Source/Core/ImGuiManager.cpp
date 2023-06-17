#include<ImGuiManager.h>

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

#include<DescriptorHeap.h>

//ImGUIの管理
class ImGuiManager : public IImGuiManager
{

private:

	DirectX12Core* directX12Core =nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_);

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

	ImGuiManager() = default;
	~ImGuiManager() = default;
};

void ImGuiManager::Initialize(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_)
{
	directX12Core = directX12Core_;

	//ImGuiのコンテキストを生成
	ImGui::CreateContext();

	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(windowsApp_->GetHwnd());

	IDescriptorHeap::DescriptorHeapViewHandle lHandl = directX12Core->GetSRVDescriptorHeap()->AddSRV();

	ImGui_ImplDX12_Init(
		directX12Core->GetDevice(),
		static_cast<int>(directX12Core->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, directX12Core->GetSRVDescriptorHeap()->GetHeap(),
		lHandl.cpuHandle,
		lHandl.gpuHandle
	);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Resources/Fonts/NotoSansJP-Bold.otf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

}

void ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Bigin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void ImGuiManager::End()
{
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* lCommandlist = directX12Core->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* lDescriptorHeaps[] = { directX12Core->GetSRVDescriptorHeap()->GetHeap()};
	lCommandlist->SetDescriptorHeaps(_countof(lDescriptorHeaps), lDescriptorHeaps);

	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), lCommandlist);
}

std::unique_ptr<IImGuiManager> CreateUniqueImGuiManager(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_)
{
	std::unique_ptr<IImGuiManager>lImGuiManager = std::make_unique<ImGuiManager>();
	lImGuiManager->Initialize(windowsApp_, directX12Core_);
	return std::move(lImGuiManager);
}

std::shared_ptr<IImGuiManager> CreateSharedImGuiManager(IWindowsApp* windowsApp_, DirectX12Core* directX12Core_)
{
	std::shared_ptr<IImGuiManager> lImGuiManager = std::make_unique<ImGuiManager>();
	lImGuiManager->Initialize(windowsApp_, directX12Core_);
	return lImGuiManager;
}
