#include<Material.h>

#include<cassert>

void Material::Initialize()
{
	pipelineState = std::make_unique<PipelineState>();

	//頂点シェーダを設定
	if (vertexShader)
	{
		pipelineState->SetVertexShader(vertexShader->GetShader());
	}
	
	//ピクセルシェーダを設定
	if (pixelShader)
	{
		pipelineState->SetPixelShader(pixelShader->GetShader());
	}

	//ジオメトリシェーダを設定
	if (geometryShader)
	{
		pipelineState->SetGeometryShader(geometryShader->GetShader());
	}

	//ハルシェーダを設定
	if (hullShader)
	{
		pipelineState->SetHullShader(hullShader->GetShader());
	}

	//ドメインシェーダを設定
	if (domainShader)
	{
		pipelineState->SetDomainShader(domainShader->GetShader());
	}

	//頂点レイアウトを設定
	pipelineState->SetInputLayout(inputLayouts.data(), inputLayouts.size());

	//ブレンドを設定
	pipelineState->SetBlend(blenddesc);

	//深度フラグを設定
	pipelineState->SetDepthFlag(depthFlag);

	//ルートシグネチャを設定
	pipelineState->SetRootSignature(rootSignature->GetRootSignature());

	//プリミティブ形状を設定
	pipelineState->SetPrimitiveType(primitiveType);

	//カウリングのモードを設定
	pipelineState->SetCullMode(cullMode);

	pipelineState->SetRenderTargetFormat(renderTargetFormat);

	pipelineState->SetDepthWriteMask(depthWriteMask);

	//生成
	if (!pipelineState->Create())
	{
		printf("Pipeline state could not be generated");
		assert(0);
	}
}

Material::~Material()
{
}
