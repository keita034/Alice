#include "BaseGPUParticle.h"

void BaseGPUParticle::SetAdapter(IAdapter* adapter_)
{
	commandList = adapter_->GetCommandList();
	device = adapter_->GetDevice();
}