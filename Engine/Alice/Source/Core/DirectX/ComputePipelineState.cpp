#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cassert>

#pragma warning(pop)

#include<ComputePipelineState.h>

void ComputePipelineState::SetShader(const D3D12_SHADER_BYTECODE& shader_)
{
	shaderByte = shader_;
}

void ComputePipelineState::SetRootSignature(IRootSignature* rootSignature_)
{
	rootSignaturePtr = rootSignature_;
}

void ComputePipelineState::SetNodeMask(uint32_t nodeMas_)
{
	pipelineNodeMask = nodeMas_;
}

void ComputePipelineState::SetCachedPSO(const D3D12_CACHED_PIPELINE_STATE& cachedPSO)
{
	cachedPipeline = cachedPSO;
}

void ComputePipelineState::SetFlag(D3D12_PIPELINE_STATE_FLAGS flag_)
{
	pipelineFlag = flag_;
}

void ComputePipelineState::Create(ID3D12Device* device_)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC lPipelineDesc{};
	lPipelineDesc.CS = shaderByte;
	lPipelineDesc.pRootSignature = rootSignaturePtr->GetRootSignature();
	lPipelineDesc.NodeMask = pipelineNodeMask;
	lPipelineDesc.CachedPSO = cachedPipeline;
	lPipelineDesc.Flags = pipelineFlag;

	HRESULT lResult = device_->CreateComputePipelineState(&lPipelineDesc, IID_PPV_ARGS(pipelineState.GetAddressOf()));

	if (FAILED(lResult))
	{
		assert(0);
	}
}

ID3D12PipelineState* ComputePipelineState::GetPipelineState() const
{
	return pipelineState.Get();
}
