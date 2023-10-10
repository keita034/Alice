#include "BloomPostEffect.h"

BloomPostEffect* BloomPostEffect::SGetInstance()
{
	static BloomPostEffect lInstance;
    return &lInstance;
}

void BloomPostEffect::Initialize()
{
	if (needsInit)
	{
		ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();
		ID3D12Device* lDevice = sMainDevice->Get();

		needsInit = false;

		type = "BLOOM";

		width = static_cast<float>(sWindowsApp->GetWindowSize().width);
		height = static_cast<float>(sWindowsApp->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(lCmdList, sSrvHeap);

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/Bloom/BloomVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/Bloom/BloomPS.hlsl", "main", "ps_5_0");

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
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->Create(lDevice);

		//生成
		material->Initialize();

		for (size_t i = 0; i < 2; i++)
		{
			//レンダーターゲットの生成
			std::unique_ptr<IRenderTargetBuffer> lRenderTargetBuffer = CreateUniqueRenderTargetBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			{//SRV作成

				D3D12_SHADER_RESOURCE_VIEW_DESC lSrvDesc{};
				lSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				lSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				lSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				lSrvDesc.Texture2D.MipLevels = 1;
				D3D12_GPU_DESCRIPTOR_HANDLE lHandle{};
				lHandle.ptr = sSrvHeap->CreateSRV(lSrvDesc, lRenderTargetBuffer->GetTexture());
				handles.push_back(lHandle);
			}

			renderTargetBuffers.push_back(std::move(lRenderTargetBuffer));
		}

		{
			//デプスステンシルの生成
			std::unique_ptr<IDepthStencilBuffer> lDepthStencilBuffer = CreateUniqueDepthStencilBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), DXGI_FORMAT_D32_FLOAT);
			depthStencilBuffers.push_back(std::move(lDepthStencilBuffer));
		}


		needsInit = false;


		material2 = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material2->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/Bloom/BloomDrawVS.hlsl");

		//ピクセルシェーダの読み込み
		material2->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/Bloom/BloomDrawPS.hlsl", "main", "ps_5_0");

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
		material2->rootSignature->Create(lDevice);

		//生成
		material2->Initialize();

	}
}

void BloomPostEffect::PostUpdate(RenderTarget* mainRenderTarget_)
{
	Draw(mainRenderTarget_);

	MainRenderTargetDraw(mainRenderTarget_);
}

const std::string& BloomPostEffect::GetType()
{
	return type;
}

void BloomPostEffect::Draw(RenderTarget* mainRenderTarget_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	for (size_t i = 0; i < 2; i++)
	{
		renderTargetBuffers[i]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	std::array<CD3DX12_VIEWPORT, 2> lViewPorts;
	std::array<CD3DX12_RECT, 2> lRects;

	for (size_t i = 0; i < 2; i++)
	{
		lViewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);

		lRects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	}

	lCmdList->RSSetViewports(2, lViewPorts.data());
	lCmdList->RSSetScissorRects(2, lRects.data());

	D3D12_CPU_DESCRIPTOR_HANDLE lRtvHs[] =
	{
		renderTargetBuffers[0]->GetHandle(),
		renderTargetBuffers[1]->GetHandle(),
	};

	lCmdList->OMSetRenderTargets(2, lRtvHs, false, &depthStencilBuffers[0]->GetHandle());


	for (size_t i = 0; i < 2; i++)
	{
		lCmdList->ClearRenderTargetView(renderTargetBuffers[i]->GetHandle(), clearColor.data(), 0, nullptr);
	}

	lCmdList->ClearDepthStencilView(depthStencilBuffers[0]->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	sprite->SetSize({ 1.0f,1.0f });

	sprite->Draw(material.get(), mainRenderTarget_->GetGpuHandle());

	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	for (size_t i = 0; i < 2; i++)
	{
		renderTargetBuffers[i]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
}

void BloomPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget_->SetRenderTarget();

	CD3DX12_VIEWPORT lViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	lCmdList->RSSetViewports(1, &lViewPort);

	CD3DX12_RECT lRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	lCmdList->RSSetScissorRects(1, &lRect);


	sprite->Draw(material2.get(), handles[0]);
	lCmdList->SetGraphicsRootDescriptorTable(1, handles[1]);
	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

BloomPostEffect::BloomPostEffect()
{
}
