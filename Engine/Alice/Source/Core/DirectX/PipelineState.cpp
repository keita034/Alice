#include<PipelineState.h>

bool PipelineState::Create()
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Core::GetDeviceSta();

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	if (vsByte)
	{
		pipelineDesc.VS = *vsByte;
	}
	if (psByte)
	{
		pipelineDesc.PS = *psByte;
	}
	if (dsByte)
	{
		pipelineDesc.DS = *dsByte;
	}
	if (hsByte)
	{
		pipelineDesc.HS = *hsByte;
	}
	if (gsByte)
	{
		pipelineDesc.GS = *gsByte;
	}

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = cullMode; // カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	//ブレンドデスク
	pipelineDesc.BlendState = blendDesc;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayoutLength);
	pipelineDesc.InputLayout.pInputElementDescs = inputLayoutData;

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = primitiveType;
	//デプスステンシルステートの設定
	//深度テストを行うか
	pipelineDesc.DepthStencilState.DepthEnable = depthFlag;
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	
	// その他の設定
	
	//描画対象数
	pipelineDesc.NumRenderTargets = renderTargetFormat.NumRenderTargets;
	
	//描画対象のフォーマット
	std::memcpy(pipelineDesc.RTVFormats, renderTargetFormat.RTVFormats.data(), sizeof(DXGI_FORMAT) * 8);

	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
	if (result != S_OK)
	{
		return false;
	}

	return true;
}

void PipelineState::SetHullShader(D3D12_SHADER_BYTECODE* pHsByte)
{
	hsByte = pHsByte;
}

void PipelineState::SetDomainShader(D3D12_SHADER_BYTECODE* pDsByte)
{
	dsByte = pDsByte;
}

void PipelineState::SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type)
{
	primitiveType = type;
}

void PipelineState::SetCullMode(D3D12_CULL_MODE model)
{
	cullMode = model;
}

void PipelineState::SetRenderTargetFormat(const RenderTargetFormat& format)
{
	std::memcpy(&renderTargetFormat, &format, sizeof(RenderTargetFormat));
}

ID3D12PipelineState* PipelineState::GetPipelineState()
{
	return pipelineState.Get();
}

void PipelineState::SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc, size_t length)
{
	inputLayoutData = desc;
	inputLayoutLength = length;
}

void PipelineState::SetRootSignature(ID3D12RootSignature* pRootSignature)
{
	rootSignature = pRootSignature;
}

void PipelineState::SetBlend(D3D12_BLEND_DESC& desc)
{
	blendDesc = desc;
}

void PipelineState::SetDepthFlag(bool flag)
{
	depthFlag = flag;
}

void PipelineState::SetVertexShader(D3D12_SHADER_BYTECODE* pVsByte)
{
	vsByte = pVsByte;
}

void PipelineState::SetPixelShader(D3D12_SHADER_BYTECODE* pPsByte)
{
	psByte = pPsByte;
}

void PipelineState::SetGeometryShader(D3D12_SHADER_BYTECODE* pGsByte)
{
	gsByte = pGsByte;
}

RenderTargetFormat::RenderTargetFormat()
{
	//描画対象は1つ
	NumRenderTargets = 1;
	//0～255指定のRGBA
	RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}
