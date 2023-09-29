#include<PipelineState.h>

#include<AliceAssert.h>

IDevice* PipelineState::sDevice = nullptr;

bool PipelineState::Create()
{
	HRESULT lResult;

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC lPipelineDesc{};
	// シェーダーの設定
	if (vsByte)
	{
		lPipelineDesc.VS = *vsByte;
	}
	if (psByte)
	{
		lPipelineDesc.PS = *psByte;
	}
	if (dsByte)
	{
		lPipelineDesc.DS = *dsByte;
	}
	if (hsByte)
	{
		lPipelineDesc.HS = *hsByte;
	}
	if (gsByte)
	{
		lPipelineDesc.GS = *gsByte;
	}

	// サンプルマスクの設定
	lPipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	lPipelineDesc.RasterizerState.CullMode = cullMode; // カリングしない
	lPipelineDesc.RasterizerState.FillMode = fillMode; // ポリゴン内塗りつぶし
	lPipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	//ブレンドデスク
	lPipelineDesc.BlendState = blendDesc;

	// 頂点レイアウトの設定
	lPipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayoutlength_);
	lPipelineDesc.InputLayout.pInputElementDescs = inputLayoutData;

	// 図形の形状設定
	lPipelineDesc.PrimitiveTopologyType = primitiveType;
	//デプスステンシルステートの設定
	//深度テストを行うか
	lPipelineDesc.DepthStencilState.DepthEnable = depthFlag;
	lPipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//書き込み許可
	lPipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//小さければ合格
	lPipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	
	// その他の設定
	
	//描画対象数
	lPipelineDesc.NumRenderTargets = renderTargetFormat.NumRenderTargets;
	
	//描画対象のフォーマット
	std::memcpy(lPipelineDesc.RTVFormats, renderTargetFormat.RTVFormats.data(), sizeof(DXGI_FORMAT) * 8);

	lPipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// パイプラインにルートシグネチャをセット
	lPipelineDesc.pRootSignature = rootSignature;

	// パイプランステートの生成
	lResult = sDevice->Get()->CreateGraphicsPipelineState(&lPipelineDesc,IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
	AliceAssert(SUCCEEDED(lResult),"パイプラインの生成に失敗しました。");

	if (lResult != S_OK)
	{
		return false;
	}

	return true;
}

void PipelineState::SetHullShader(D3D12_SHADER_BYTECODE* pHsByte_)
{
	hsByte = pHsByte_;
}

void PipelineState::SetDomainShader(D3D12_SHADER_BYTECODE* pDsByte_)
{
	dsByte = pDsByte_;
}

void PipelineState::SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type_)
{
	primitiveType = type_;
}

void PipelineState::SetCullMode(D3D12_CULL_MODE mode_)
{
	cullMode = mode_;
}

void PipelineState::SetFillMode(D3D12_FILL_MODE mode_)
{
	fillMode = mode_;
}

void PipelineState::SetRenderTargetFormat(const RenderTargetFormat& format_)
{
	std::memcpy(&renderTargetFormat, &format_, sizeof(RenderTargetFormat));
}

ID3D12PipelineState* PipelineState::GetPipelineState()const
{
	return pipelineState.Get();
}

void PipelineState::SSetDevice(IDevice* device_)
{
	sDevice = device_;
}

void PipelineState::SetInputLayout(D3D12_INPUT_ELEMENT_DESC* desc_, size_t length_)
{
	inputLayoutData = desc_;
	inputLayoutlength_ = length_;
}

void PipelineState::SetRootSignature(ID3D12RootSignature* pRootSignature_)
{
	rootSignature = pRootSignature_;
}

void PipelineState::SetBlend(const D3D12_BLEND_DESC& desc_)
{
	blendDesc = desc_;
}

void PipelineState::SetDepthFlag(bool flag_)
{
	depthFlag = flag_;
}

void PipelineState::SetVertexShader(D3D12_SHADER_BYTECODE* pVsByte_)
{
	vsByte = pVsByte_;
}

void PipelineState::SetPixelShader(D3D12_SHADER_BYTECODE* pPsByte_)
{
	psByte = pPsByte_;
}

void PipelineState::SetGeometryShader(D3D12_SHADER_BYTECODE* pGsByte_)
{
	gsByte = pGsByte_;
}

RenderTargetFormat::RenderTargetFormat()
{
	//描画対象は1つ
	NumRenderTargets = 1;
	//0～255指定のRGBA
	RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
}
