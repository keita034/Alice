#include "StripedPostEffect.h"

StripedPostEffect* StripedPostEffect::SGetInstance()
{
	static StripedPostEffect lInstance;
	return &lInstance;
}

void StripedPostEffect::Initialize()
{
	if (needsInit)
	{
		needsInit = false;

		type = "STRIPED";

		width = static_cast<float>(sWindowsApp->GetWindowSize().width);
		height = static_cast<float>(sWindowsApp->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(sCmdList, sSrvHeap);

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/Striped/StripedVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/Striped/StripedPS.hlsl", "main", "ps_5_0");

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
		material->rootSignature->Create(sDevice);

		//生成
		material->Initialize();

		for (size_t i = 0; i < 2; i++)
		{
			//レンダーターゲットの生成
			std::unique_ptr<IRenderTargetBuffer> lRenderTargetBuffer = CreateUniqueRenderTargetBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, { {0.1f, 0.25f, 0.5f, 1.0f} });
			
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


		material2= std::make_unique<Material>();

		//頂点シェーダの読み込み
		material2->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/StripedDraw/StripedDrawVS.hlsl");

		//ピクセルシェーダの読み込み
		material2->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/StripedDraw/StripedDrawPS.hlsl", "main", "ps_5_0");

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
		material2->rootSignature->Create(sDevice);

		//生成
		material2->Initialize();

		constantBuffer = CreateUniqueConstantBuffer(sizeof(ConstantBufferMap));


		ConstantBufferMap tmp;
		tmp.blurLevel = 1;
		tmp.uForRange = 6;
		tmp.vForRange = 6;
		tmp.windowSize = { static_cast<float>(sWindowsApp->GetWindowSize().width),static_cast<float>(sWindowsApp->GetWindowSize().height) };
		constantBuffer->Update(&tmp);

			clearColor = { {0.1f, 0.25f, 0.5f, 1.0f} };

	}
}


void StripedPostEffect::PostUpdate(RenderTarget* mainRenderTarget_)
{
	Draw(mainRenderTarget_);

	MainRenderTargetDraw(mainRenderTarget_);
}

const std::string& StripedPostEffect::GetType()
{
	return type;
}

void StripedPostEffect::Draw(RenderTarget* mainRenderTarget_)
{
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

	sCmdList->RSSetViewports(2, lViewPorts.data());
	sCmdList->RSSetScissorRects(2, lRects.data());

	D3D12_CPU_DESCRIPTOR_HANDLE lRtvHs[] =
	{
		renderTargetBuffers[0]->GetHandle(),
		renderTargetBuffers[1]->GetHandle(),
	};

	sCmdList->OMSetRenderTargets(2, lRtvHs, false, &depthStencilBuffers[0]->GetHandle());


	for (size_t i = 0; i < 2; i++)
	{
		sCmdList->ClearRenderTargetView(renderTargetBuffers[i]->GetHandle(), clearColor.data(), 0, nullptr);
	}

	sCmdList->ClearDepthStencilView(depthStencilBuffers[0]->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	sprite->SetSize({ 1.0f,1.0f });

	sprite->Draw(material.get(), mainRenderTarget_->GetGpuHandle());

	sCmdList->SetGraphicsRootConstantBufferView(1, constantBuffer->GetAddress());

	// 描画コマンド
	sCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	for (size_t i = 0; i < 2; i++)
	{
		renderTargetBuffers[i]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
}

void StripedPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget_)
{
	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget_->SetRenderTarget();

	CD3DX12_VIEWPORT lViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	sCmdList->RSSetViewports(1, &lViewPort);

	CD3DX12_RECT lRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	sCmdList->RSSetScissorRects(1, &lRect);


	sprite->Draw(material2.get(), handles[0]);
	sCmdList->SetGraphicsRootDescriptorTable(1, handles[1]);
	// 描画コマンド
	sCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

StripedPostEffect::StripedPostEffect()
{

}