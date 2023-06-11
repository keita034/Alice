#include "StripedPostEffect.h"

StripedPostEffect* StripedPostEffect::GetInstance()
{
	static StripedPostEffect instance;
	return &instance;
}

void StripedPostEffect::Initialize()
{
	if (needsInit)
	{
		needsInit = false;

		type = "STRIPEDPOSTEFFECT";

		cmdList = DirectX12Core::GetInstance()->GetCommandList().Get();

		width = static_cast<float>(windowsApp->GetWindowSize().width);
		height = static_cast<float>(windowsApp->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(DirectX12Core::GetCommandListSta().Get(), DirectX12Core::GetInstance()->GetSRVDescriptorHeap());

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/StripedVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/StripedPS.hlsl", "main", "ps_5_0");

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
		material->rootSignature = CreateUniqueRootSignature();
		material->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
		material->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

		//生成
		material->Initialize();

		for (size_t i = 0; i < 2; i++)
		{
			//レンダーターゲットの生成
			std::unique_ptr<IRenderTargetBuffer> buff = CreateUniqueRenderTargetBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			
			{//SRV作成

				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
				srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;
				D3D12_GPU_DESCRIPTOR_HANDLE handle{};
				handle.ptr = srvHeap->CreateSRV(srvDesc, buff->GetTexture());
				handles.push_back(handle);
			}

			renderTargetBuffers.push_back(std::move(buff));
		}

		{
			//デプスステンシルの生成
			std::unique_ptr<IDepthStencilBuffer> buff = CreateUniqueDepthStencilBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), DXGI_FORMAT_D32_FLOAT);
			depthStencilBuffers.push_back(std::move(buff));
		}


		needsInit = false;


		material2= std::make_unique<Material>();

		//頂点シェーダの読み込み
		material2->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/StripedDrawVS.hlsl");

		//ピクセルシェーダの読み込み
		material2->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/StripedDrawPS.hlsl", "main", "ps_5_0");

		//頂点レイアウト設定
		material2->inputLayouts =
		{
			// xyz座標
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			// uv座標
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			//カラー
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		};

		material2->depthFlag = false;

		material2->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material2->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		material2->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

		material2->renderTargetFormat.NumRenderTargets = 1;
		material2->renderTargetFormat.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		//ルートシグネチャ設定
		material2->rootSignature = CreateUniqueRootSignature();
		material2->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
		material2->rootSignature->AddStaticSampler(0);//s0
		material2->rootSignature->Add(IRootSignature::RangeType::SRV, 1);//t0
		material2->rootSignature->AddStaticSampler(1);//s0
		material2->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

		//生成
		material2->Initialize();

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
		renderTargetBuffers[i]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	CD3DX12_VIEWPORT viewPorts[2]{};
	CD3DX12_RECT rects[2]{};

	for (size_t i = 0; i < 2; i++)
	{
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);

		rects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	}

	cmdList->RSSetViewports(2, viewPorts);
	cmdList->RSSetScissorRects(2, rects);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[] =
	{
		renderTargetBuffers[0]->GetHandle(),
		renderTargetBuffers[1]->GetHandle(),
	};

	cmdList->OMSetRenderTargets(2, rtvHs, false, &depthStencilBuffers[0]->GetHandle());


	for (size_t i = 0; i < 2; i++)
	{
		cmdList->ClearRenderTargetView(renderTargetBuffers[i]->GetHandle(), clearColor.data(), 0, nullptr);
	}

	cmdList->ClearDepthStencilView(depthStencilBuffers[0]->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	sprite->SetSize({ 1.0f,1.0f });

	sprite->Draw(material.get(), mainRenderTarget->GetGpuHandle());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	for (size_t i = 0; i < 2; i++)
	{
		renderTargetBuffers[i]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
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


	sprite->Draw(material2.get(), handles[0]);
	cmdList->SetGraphicsRootDescriptorTable(1, handles[1]);
	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

StripedPostEffect::StripedPostEffect()
{

}