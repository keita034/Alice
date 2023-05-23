#include<GaussianXBlurPostEffect.h>

void GaussianXBlurPostEffect::Initialize()
{
	if (needsInit)
	{
		cmdList = DirectX12Core::GetInstance()->GetCommandList().Get();

		width = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().width / 2);
		height = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(DirectX12Core::GetCommandListSta().Get(), DirectX12Core::GetInstance()->GetSRVDescriptorHeap());

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = std::make_unique<Shader>();
		material->vertexShader->Create("Resources/Shaders/2D/PostEffect/GaussianXBlurVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = std::make_unique<Shader>();
		material->pixelShader->Create("Resources/Shaders/2D/PostEffect/GaussianBlurPS.hlsl", "main", "ps_5_0");

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
		material->rootSignature = std::make_unique<RootSignature>();
		material->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
		material->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

		//生成
		material->Initialize();

		{
			//レンダーターゲットの生成
			std::unique_ptr<RenderTargetBuffer> buff = std::make_unique<RenderTargetBuffer>();
			buff->Create(static_cast<UINT>(width), static_cast<UINT>(height), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

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
			std::unique_ptr<DepthStencilBuffer> buff = std::make_unique<DepthStencilBuffer>();
			buff->Create(static_cast<UINT>(width), static_cast<UINT>(height), DXGI_FORMAT_D32_FLOAT);
			depthStencilBuffers.push_back(std::move(buff));
		}

		weightBuff = std::make_unique<ConstantBuffer>();
		weightBuff->Create(sizeof(weight));
		weightBuff->Update(weight.data());

		needsInit = false;
	}
}

void GaussianXBlurPostEffect::PostUpdate(RenderTarget* mainRenderTarget)
{
	Draw(mainRenderTarget);

	MainRenderTargetDraw(mainRenderTarget);
}

const std::string& GaussianXBlurPostEffect::GetType()
{
	return type;
}

void GaussianXBlurPostEffect::SetWeight(std::array<float, 8>& weightPtr)
{
	weight = weightPtr;

	weightBuff->Update(weight.data());
}

void GaussianXBlurPostEffect::Draw(RenderTarget* mainRenderTarget)
{
	renderTargetBuffers[0]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[] =
	{
		renderTargetBuffers[0]->GetHandle()
	};

	cmdList->OMSetRenderTargets(1, rtvHs, false, &depthStencilBuffers[0]->GetHandle());

	cmdList->ClearRenderTargetView(renderTargetBuffers[0]->GetHandle(), clearColor.data(), 0, nullptr);
	cmdList->ClearDepthStencilView(depthStencilBuffers[0]->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	cmdList->RSSetScissorRects(1, &rect);

	sprite->SetSize({ 1.0f,1.0f });
	sprite->Draw(material.get(), mainRenderTarget->GetGpuHandle());

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, weightBuff->GetAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	renderTargetBuffers[0]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void GaussianXBlurPostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget)
{
	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget->SetRenderTarget();

	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width * 2, height);
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(width * 2), static_cast<LONG>(height));
	cmdList->RSSetScissorRects(1, &rect);

	sprite->SetSize({ 1.0f,1.0f });
	sprite->Draw(MaterialManager::GetMaterial("DefaultPostEffect"), handles[0]);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
