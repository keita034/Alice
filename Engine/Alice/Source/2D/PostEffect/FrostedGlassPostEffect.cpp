#include "FrostedGlassPostEffect.h"
#include<MaterialManager.h>

FrostedGlassPostEffect* FrostedGlassPostEffect::SGetInstance()
{
	static FrostedGlassPostEffect lInstance;
	return &lInstance;
}

void FrostedGlassPostEffect::Initialize()
{
	if (needsInit)
	{
		ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();
		ID3D12Device* lDevice = sMainDevice->Get();

		width = static_cast<float>(sWindowsApp->GetWindowSize().width);
		height = static_cast<float>(sWindowsApp->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(lCmdList, sSrvHeap);

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/FrostedGlass/FrostedGlassVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/FrostedGlass/FrostedGlassPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

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

		//ルートシグネチャ設定
		material->rootSignature = CreateUniqueRootSignature();
		material->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->Add(IRootSignature::RootType::CBV, 0);//t0
		material->rootSignature->Create(lDevice);

		//生成
		material->Initialize();

		{
			//レンダーターゲットの生成
			std::unique_ptr<IRenderTargetBuffer> lRenderTargetBuffer = CreateUniqueRenderTargetBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

			{//SRV作成

				D3D12_SHADER_RESOURCE_VIEW_DESC lSrvDesc{};
				lSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				lSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				lSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				lSrvDesc.Texture2D.MipLevels = 1;
				D3D12_GPU_DESCRIPTOR_HANDLE handle{};
				handle.ptr = sSrvHeap->CreateSRV(lSrvDesc, lRenderTargetBuffer->GetTexture());
				handles.push_back(handle);
			}

			renderTargetBuffers.push_back(std::move(lRenderTargetBuffer));
		}

		{
			//デプスステンシルの生成
			std::unique_ptr<IDepthStencilBuffer> lDepthStencilBuffer = CreateUniqueDepthStencilBuffer(static_cast<uint32_t>(width), static_cast<uint32_t>(height), DXGI_FORMAT_D32_FLOAT);
			depthStencilBuffers.push_back(std::move(lDepthStencilBuffer));
		}

		needsInit = false;

		constantBuffer = CreateUniqueConstantBuffer(sizeof(ConstantBufferMap));

		constantBufferMap.intensity = { 500.0f,500.0f };
		constantBufferMap.radius = 5.0f;
		constantBuffer->Update(&constantBufferMap);

		type = "FROSTEDGLASS";
	}
}

void FrostedGlassPostEffect::PostUpdate(RenderTarget* mainRenderTarget_)
{
	Draw(mainRenderTarget_);
	MainRenderTargetDraw(mainRenderTarget_);
}

const std::string& FrostedGlassPostEffect::GetType()
{
	return type;
}

void FrostedGlassPostEffect::SetIntensity(const AliceMathF::Vector2& intensity_)
{
	SGetInstance()->PSetIntensity(intensity_);
}

void FrostedGlassPostEffect::SetRadius(float radius_)
{
	SGetInstance()->PSetRadius(radius_);
}

void FrostedGlassPostEffect::Draw(RenderTarget* mainRenderTarget_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	renderTargetBuffers[0]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	D3D12_CPU_DESCRIPTOR_HANDLE lRtvHs[] =
	{
		renderTargetBuffers[0]->GetHandle()
	};

	lCmdList->OMSetRenderTargets(1, lRtvHs, false, &depthStencilBuffers[0]->GetHandle());

	lCmdList->ClearRenderTargetView(renderTargetBuffers[0]->GetHandle(), clearColor.data(), 0, nullptr);
	lCmdList->ClearDepthStencilView(depthStencilBuffers[0]->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	CD3DX12_VIEWPORT lViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	lCmdList->RSSetViewports(1, &lViewPort);

	CD3DX12_RECT lRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	lCmdList->RSSetScissorRects(1, &lRect);

	sprite->SetSize({ 1.0f,1.0f });

	sprite->Draw(material.get(), mainRenderTarget_->GetGpuHandle());

	lCmdList->SetGraphicsRootConstantBufferView(1, constantBuffer->GetAddress());
	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	renderTargetBuffers[0]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void FrostedGlassPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget_->SetRenderTarget();

	CD3DX12_VIEWPORT lViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	lCmdList->RSSetViewports(1, &lViewPort);

	CD3DX12_RECT lRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	lCmdList->RSSetScissorRects(1, &lRect);
	sprite->SetSize({ 1.01f,1.01f });
	sprite->Draw(MaterialManager::SGetMaterial("DefaultPostEffect"), handles[0]);

	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void FrostedGlassPostEffect::PSetIntensity(const AliceMathF::Vector2& intensity_)
{
	constantBufferMap.intensity = intensity_;
	constantBuffer->Update(&constantBufferMap);
}

void FrostedGlassPostEffect::PSetRadius(float radius_)
{
	constantBufferMap.radius = radius_;
	constantBuffer->Update(&constantBufferMap);
}

FrostedGlassPostEffect::FrostedGlassPostEffect()
{
}