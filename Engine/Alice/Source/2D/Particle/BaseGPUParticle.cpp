#include "BaseGPUParticle.h"

void BaseGPUParticle::SetAdapter(IAdapter* graphicAdapter_, IAdapter* computeAdapter_)
{
	commandList = graphicAdapter_->GetCommandList();
	device = graphicAdapter_->GetDevice();

	 computeCmmandList = computeAdapter_->GetCommandList();
	 computeDevice = computeAdapter_->GetDevice();

}
