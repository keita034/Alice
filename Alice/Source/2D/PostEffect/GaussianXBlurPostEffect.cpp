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

		renderTarget = std::make_unique<RenderTarget>(DirectX12Core::GetInstance()->GetSRVDescriptorHeap(), cmdList);

		renderTarget->Initialize(WindowsApp::GetInstance()->GetWindowSize().width / 2, WindowsApp::GetInstance()->GetWindowSize().height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

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
	renderTarget->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);

	renderTarget->SetRenderTarget();

	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(0.0f, 0.0f, width, height);
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
	cmdList->RSSetScissorRects(1, &rect);

	//renderTarget->ClearRenderTarget();
	sprite->SetSize({ 1.0f,1.0f });
	sprite->Draw(material.get(), mainRenderTarget->GetGpuHandle());

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, weightBuff->GetAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	renderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
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
	sprite->Draw(MaterialManager::GetMaterial("DefaultPostEffect"), renderTarget->GetGpuHandle());

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);

	mainRenderTarget->Transition(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
