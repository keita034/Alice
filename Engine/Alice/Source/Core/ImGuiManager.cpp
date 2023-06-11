

#include<ImGuiManager.h>


void ImGuiManager::Initialize(IWindowsApp* windowsApp, DirectX12Core* core)
{
	directX12Core = core;

	//ImGuiのコンテキストを生成
	ImGui::CreateContext();

	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(windowsApp->GetHwnd());

	IDescriptorHeap::DescriptorHeapViewHandle handl = directX12Core->GetSRVDescriptorHeap()->AddSRV();

	ImGui_ImplDX12_Init(
		directX12Core->GetDevice().Get(),
		static_cast<int>(directX12Core->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, directX12Core->GetSRVDescriptorHeap()->GetHeap(),
		handl.cpuHandle,
		handl.gpuHandle
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
	ID3D12GraphicsCommandList* commandlist = directX12Core->GetCommandList().Get();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { directX12Core->GetSRVDescriptorHeap()->GetHeap()};
	commandlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandlist);
}
