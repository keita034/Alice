#include "StripedPostEffect.h"

void StripedPostEffect::Initialize()
{
	if (needsInit)
	{
		needsInit = false;

		type = "STRIPEDPOSTEFFECT";


		cmdList = DirectX12Core::GetInstance()->GetCommandList().Get();

		width = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().width);
		height = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(DirectX12Core::GetCommandListSta().Get(), DirectX12Core::GetInstance()->GetSRVDescriptorHeap());

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = std::make_unique<Shader>();
		material->vertexShader->Create("Resources/Shaders/2D/PostEffect/StripedVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = std::make_unique<Shader>();
		material->pixelShader->Create("Resources/Shaders/2D/PostEffect/StripedPS.hlsl", "main", "ps_5_0");

		//頂点レイアウト設定
		material->inputLayouts =
		{
			// xyz座標
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			// uv座標
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			//カラー
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		material->depthFlag = false;

		material->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		material->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

		material->blenddesc.RenderTarget[1].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[1].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		material->blenddesc.RenderTarget[1].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

		material->renderTargetFormat.NumRenderTargets = 2;
		material->renderTargetFormat.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		material->renderTargetFormat.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		//ルートシグネチャ設定
		material->rootSignature = std::make_unique<RootSignature>();
		material->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
		material->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

		//生成
		material->Initialize();

		renderTargets.push_back(std::make_unique<RenderTarget>(DirectX12Core::GetInstance()->GetSRVDescriptorHeap(), cmdList));
		renderTargets.push_back(std::make_unique<RenderTarget>(DirectX12Core::GetInstance()->GetSRVDescriptorHeap(), cmdList));

		renderTargets[0]->Initialize(WindowsApp::GetInstance()->GetWindowSize().width, WindowsApp::GetInstance()->GetWindowSize().height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		renderTargets[1]->Initialize(WindowsApp::GetInstance()->GetWindowSize().width, WindowsApp::GetInstance()->GetWindowSize().height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		needsInit = false;
	}
}


void StripedPostEffect::PostUpdate(RenderTarget* mainRenderTarget)
{
	Draw(mainRenderTarget);

	MainRenderTargetDraw(mainRenderTarget);
}

const std::string& StripedPostEffect::GetType()
{
	return type;
}

void StripedPostEffect::Draw(RenderTarget* mainRenderTarget)
{
	for (size_t i = 0; i < 2; i++)
	{
		renderTargets[i]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[] =
	{
		renderTargets[0]->GetRenderTargetBuffer()->GetHandle(),
		renderTargets[1]->GetRenderTargetBuffer()->GetHandle(),
	};

	cmdList->OMSetRenderTargets(2, rtvHs, false, &renderTargets[0]->GetDepthStencilBuffer()->GetHandle());

	CD3DX12_VIEWPORT viewPorts[2]{};
	CD3DX12_RECT rects[2]{};

	for (size_t i = 0; i < 2; i++)
	{
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);

		rects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	}

	cmdList->RSSetViewports(2, viewPorts);
	cmdList->RSSetScissorRects(2, rects);

	for (size_t i = 0; i < 2; i++)
	{
		renderTargets[i]->Clear();
	}

	sprite->SetSize({ 1.0f,1.0f });

	sprite->Draw(material.get(), mainRenderTarget->GetGpuHandle());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	for (size_t i = 0; i < 2; i++)
	{
		renderTargets[i]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
}

void StripedPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget)
{
	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget->SetRenderTarget();

	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	cmdList->RSSetScissorRects(1, &rect);


	sprite->Draw(MaterialManager::GetMaterial("DefaultPostEffect"), renderTargets.back()->GetGpuHandle());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
