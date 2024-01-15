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

ICrossAdapterBuffer* BaseGPUParticle::GetParticlePoolBuffer()
{
	return particlePoolBuffer.get();
}

IFreeListBuffer* BaseGPUParticle::GetFreeListBuffer()
{
	return freeListBuffer.get();
}

IDrawListBuffer* BaseGPUParticle::GetDrawListBuffer()
{
	return drawListBuffer.get();
}

size_t BaseGPUParticle::GetMaxParticles()
{
	return maxParticles;
}

void BaseGPUParticle::SetAdapter(IAdapter* graphicAdapter_,IAdapter* computeAdapter_)
{
	graphicAdapter = graphicAdapter_;
	computeAdapter = computeAdapter_;
}

void BaseGPUParticle::ParticleBegin()
{
	size_t bbIndex = static_cast< size_t >( swapChain->GetCurrentBackBufferIndex() );
	computeAdapter->GraphicCommandListReset(bbIndex);
	computeAdapter->ComputeCommandListReset(bbIndex);
}

void BaseGPUParticle::SetSwapChain(ISwapChain* swapChain_)
{
	swapChain = swapChain_;
}

void BaseGPUParticle::ParticleEnd()
{
	computeAdapter->ComputeCommandListExecute();
	computeAdapter->ComputeWaitPreviousFrame();

	computeAdapter->GraphicCommandListExecute();
	computeAdapter->GraphicWaitPreviousFrame();
}