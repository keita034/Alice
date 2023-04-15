#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cassert>

#pragma warning(pop)

#include "ComputePipelineState.h"

void ComputePipelineState::SetShader(D3D12_SHADER_BYTECODE& shader)
{
	shaderByte = shader;
}

void ComputePipelineState::SetRootSignature(RootSignature* rootSignature)
{
	rootSignaturePtr = rootSignature;
}

void ComputePipelineState::SetNodeMask(UINT nodeMask)
{
	pipelineNodeMask = nodeMask;
}

void ComputePipelineState::SetCachedPSO(D3D12_CACHED_PIPELINE_STATE& cachedPSO)
{
	cachedPipeline = cachedPSO;
}

void ComputePipelineState::SetFlag(D3D12_PIPELINE_STATE_FLAGS flag)
{
	pipelineFlag = flag;
}

void ComputePipelineState::Create(ID3D12Device* device)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};
	pipelineDesc.CS = shaderByte;
	pipelineDesc.pRootSignature = rootSignaturePtr->GetRootSignature();
	pipelineDesc.NodeMask = pipelineNodeMask;
	pipelineDesc.CachedPSO = cachedPipeline;
	pipelineDesc.Flags = pipelineFlag;

	HRESULT result =  device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(pipelineState.GetAddressOf()));

	if (FAILED(result))
	{
		assert(0);
	}
}

ID3D12PipelineState* ComputePipelineState::GetPipelineState()
{
	return pipelineState.Get();
}
