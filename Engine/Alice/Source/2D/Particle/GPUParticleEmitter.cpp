#include "GPUParticleEmitter.h"


void GPUParticleEmitter::Initialize()
{
	BasicGPUParticle::SetAdapter(multiAdapters->GetMainAdapter(),multiAdapters->GetSubAdapter());
	BasicGPUParticle::SetSwapChain(swapChain);
}

void GPUParticleEmitter::Update(float deltaTime_)
{
	BaseGPUParticle::ParticleBegin();

	if ( basicGPUParticle )
	{
		basicGPUParticle->Update(deltaTime_);
	}

	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::Draw(const AliceMathF::Matrix4& viewMat_,const AliceMathF::Matrix4& projectionMat_)
{
	if ( basicGPUParticle )
	{
		basicGPUParticle->Draw(viewMat_,projectionMat_);
	}
}

void GPUParticleEmitter::BasicGPUParticleEmit(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_)
{
	if (! basicGPUParticle )
	{
		BaseGPUParticle::ParticleBegin();

		basicGPUParticle = std::make_unique<BasicGPUParticle>();
		basicGPUParticle->ADD(pos_,setting_);
		basicGPUParticle->Initialize();

		BaseGPUParticle::ParticleEnd();
	}
	else
	{
		basicGPUParticle->ADD(pos_,setting_);
		basicGPUParticle->SetSetting();
	}
}

void GPUParticleEmitter::SetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	multiAdapters = multiAdapters_;
}

void GPUParticleEmitter::SetSwapChain(ISwapChain* swapChain_)
{
	swapChain = swapChain_;
}
