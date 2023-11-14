#include "BaseGPUParticle.h"

IAdapter* BaseGPUParticle::graphicAdapter;
IAdapter* BaseGPUParticle::computeAdapter;

ISwapChain* BaseGPUParticle::swapChain = nullptr;

void BaseGPUParticle::BaseGPUParticleFinalize()
{
	swapChain = nullptr;

	graphicAdapter = nullptr;
	computeAdapter = nullptr;
}

void BaseGPUParticle::SetAdapter(IAdapter* graphicAdapter_,IAdapter* computeAdapter_)
{
	graphicAdapter = graphicAdapter_;
	computeAdapter = computeAdapter_;
}

void BaseGPUParticle::ParticleBegin()
{
	size_t bbIndex = static_cast< size_t >( swapChain->GetCurrentBackBufferIndex() );
	graphicAdapter->ComputeCommandListReset(bbIndex);
}

void BaseGPUParticle::SetSwapChain(ISwapChain* swapChain_)
{
	swapChain = swapChain_;
}

void BaseGPUParticle::ParticleEnd()
{
	graphicAdapter->ComputeCommandListExecute();
	graphicAdapter->ComputeWaitPreviousFrame();

}