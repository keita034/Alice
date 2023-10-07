#include "ComputeMaterial.h"

void ComputeMaterial::Initialize()
{
	pipelineState = CreateUniqueComputePipelineState();

	//コンピュートシェーダを設定
	if ( computeShader )
	{
		pipelineState->SetShader(*computeShader->GetShader());
	}

	pipelineState->SetCachedPSO(cachedPipeline);

	pipelineState->SetFlag(pipelineFlag);

	pipelineState->SetNodeMask(pipelineNodeMask);

	//ルートシグネチャを設定
	pipelineState->SetRootSignature(rootSignature.get());

	//生成
	pipelineState->Create();
}

ComputeMaterial::~ComputeMaterial()
{
}
