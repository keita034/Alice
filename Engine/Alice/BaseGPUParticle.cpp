#include "BaseGPUParticle.h"

void BaseGPUParticle::SetPipelineSignature(IComputePipelineState* pipelineState_,IRootSignature* rootSignature_)
{
	pipelineState = pipelineState_;
	rootSignature = rootSignature_;
}

void BaseGPUParticle::SetAdapter(IAdapter* adapter_)
{
	commandList = adapter_->GetCommandList();
	device = adapter_->GetDevice();
}
