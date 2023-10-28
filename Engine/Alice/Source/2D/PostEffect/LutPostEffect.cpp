#include<LutPostEffect.h>
#include<DirectX12Core.h>
#include<TextureManager.h>
#include<WindowsApp.h>
#include<MaterialManager.h>

LutPostEffect* LutPostEffect::SGetInstance()
{
	static LutPostEffect lInstance;
	return &lInstance;
}

void LutPostEffect::Initialize()
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
		material->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/LutShaderVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/LutShaderPS.hlsl", "main", "ps_5_0");

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
		material->rootSignature->Add(IRootSignature::RangeType::SRV, 1);//t1
		material->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->AddStaticSampler(1);//s1
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

		uint32_t lTexHandle = TextureManager::SLoad("Resources/Lut/amoebae-66-multilut1.png");

		lutTexture = TextureManager::SGetTextureData(lTexHandle);

		lutSizeBuff = CreateUniqueConstantBuffer(sizeof(lutSize));

		size = { 1.0f / static_cast<float>(lutTexture->width),1.0f / static_cast<float>(lutTexture->height),1.13900006f,0.714969635f ,0.0489999987f,static_cast<float>(lutTexture->width / lutTexture->height) - 3.0f };

		lutSizeBuff->Update(&size);

		type = "LUT";
	}
}

void LutPostEffect::PostUpdate(RenderTarget* mainRenderTarget_)
{
	Draw(mainRenderTarget_);

	MainRenderTargetDraw(mainRenderTarget_);
}

void LutPostEffect::SetLutTexture(uint32_t handle_)
{
	lutTexture = TextureManager::SGetTextureData(handle_);

	size = { 1.0f / static_cast<float>(lutTexture->width),1.0f / static_cast<float>(lutTexture->height),1.139f,1.729f ,0.049f,static_cast<float>(lutTexture->width / lutTexture->height) - 3.0f };

	lutSizeBuff->Update(&size);
}

const std::string& LutPostEffect::GetType() 
{
	return type;
}

void LutPostEffect::Draw(RenderTarget* mainRenderTarget_)
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

	sprite->Draw(material.get(), mainRenderTarget_->GetGpuHandle());

	lCmdList->SetGraphicsRootDescriptorTable(1, lutTexture->gpuHandle);

	// 定数バッファビュー(CBV)の設定コマンド
	lCmdList->SetGraphicsRootConstantBufferView(2, lutSizeBuff->GetAddress());

	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	renderTargetBuffers[0]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

void LutPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget_->SetRenderTarget();

	CD3DX12_VIEWPORT lViewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	lCmdList->RSSetViewports(1, &lViewPort);

	CD3DX12_RECT lRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	lCmdList->RSSetScissorRects(1, &lRect);

	sprite->Draw(MaterialManager::SGetMaterial("DefaultPostEffect"), handles[0]);

	// 描画コマンド
	lCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget_->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
