#include<VignettePostEffect.h>

VignettePostEffect* VignettePostEffect::GetInstance()
{
	static VignettePostEffect instance;
	return &instance;
}

void VignettePostEffect::Initialize()
{
	if (needsInit)
	{
		cmdList = DirectX12Core::GetInstance()->GetCommandList().Get();

		width = static_cast<float>(windowsApp->GetWindowSize().width);
		height = static_cast<float>(windowsApp->GetWindowSize().height);

		sprite = std::make_unique<PostEffectSprite>();
		sprite->Initialize(DirectX12Core::GetCommandListSta().Get(), DirectX12Core::GetInstance()->GetSRVDescriptorHeap());

		material = std::make_unique<Material>();

		//頂点シェーダの読み込み
		material->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/VignetteShaderVS.hlsl");

		//ピクセルシェーダの読み込み
		material->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/VignetteShaderPS.hlsl", "main", "ps_5_0");

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
		material->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
		material->rootSignature->AddStaticSampler(0);//s0
		material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

		//生成
		material->Initialize();

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

		vignetteDataBuff = CreateUniqueConstantBuffer(sizeof(VignetteData));

		data =
		{
			{1.0f,1.0f,1.0f},
			0,
			{0.5f,0.5f},
			0.8f,
			0,
			{static_cast<float>(windowsApp->GetNowWindowSize().width),static_cast<float>(windowsApp->GetNowWindowSize().height)},
			{1.0f, 1.0f},
		};

		vignetteDataBuff->Update(&data);

		cmdList = DirectX12Core::GetInstance()->GetCommandList().Get();

		type = "VIGNETTE";
	}
}

void VignettePostEffect::PostUpdate(RenderTarget* mainRenderTarget)
{
	Draw(mainRenderTarget);

	MainRenderTargetDraw(mainRenderTarget);
}

void VignettePostEffect::SetVignetteData(const AliceMathF::Vector3& color, const AliceMathF::Vector2& center, float power, const AliceMathF::Vector2& size)
{
	VignettePostEffect::GetInstance()->SetData(color, center, power, size);
}

void VignettePostEffect::SetColor(const AliceMathF::Vector3& color)
{
	VignettePostEffect::GetInstance()->SetCol(color);
}

void VignettePostEffect::SetCenter(const AliceMathF::Vector2& center)
{
	VignettePostEffect::GetInstance()->SetCent(center);
}

void VignettePostEffect::SetPower(float power)
{
	VignettePostEffect::GetInstance()->SetPow(power);
}

void VignettePostEffect::SetSize(const AliceMathF::Vector2& size)
{
	VignettePostEffect::GetInstance()->SetSi(size);
}

const std::string& VignettePostEffect::GetType()
{
	return type;
}

void VignettePostEffect::Draw(RenderTarget* mainRenderTarget)
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

	//renderTarget->ClearRenderTarget();
	sprite->SetSize({ width, height });

	sprite->Draw(material.get(), mainRenderTarget->GetGpuHandle());

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, vignetteDataBuff->GetAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	renderTargetBuffers[0]->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

void VignettePostEffect::MainRenderTargetDraw(RenderTarget* mainRenderTarget)
{
	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	mainRenderTarget->SetRenderTarget();

	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	cmdList->RSSetScissorRects(1, &rect);

	sprite->SetSize({ width, height });
	sprite->Draw(material.get(), handles[0]);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

void VignettePostEffect::SetData(const AliceMathF::Vector3& color, const AliceMathF::Vector2& center, float power, const AliceMathF::Vector2& size)
{
	AliceMathF::Vector3 col = { 1.0f,1.0f,1.0f };
	col = col - (color / 255.0f);

	data.color = col;
	data.center = center;
	data.power = power;
	data.winSize = { static_cast<float>(windowsApp->GetNowWindowSize().width),static_cast<float>(windowsApp->GetNowWindowSize().height) };
	data.size = size;
	vignetteDataBuff->Update(&data);
}

void VignettePostEffect::SetCol(const AliceMathF::Vector3& color)
{
	AliceMathF::Vector3 col = { 1.0f,1.0f,1.0f };
	col = col - (color / 255.0f);

	data.color = col;

	vignetteDataBuff->Update(&data);
}

void VignettePostEffect::SetCent(const AliceMathF::Vector2& center)
{
	data.center = center;

	vignetteDataBuff->Update(&data);
}

void VignettePostEffect::SetPow(float power)
{
	data.power = power;

	vignetteDataBuff->Update(&data);
}

void VignettePostEffect::SetSi(const AliceMathF::Vector2& size)
{
	data.size = size;

	vignetteDataBuff->Update(&data);
}
