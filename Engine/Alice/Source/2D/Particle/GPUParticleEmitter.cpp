#include "GPUParticleEmitter.h"


void GPUParticleEmitter::Initialize()
{
}

void GPUParticleEmitter::Update(float deltaTime_)
{
	if ( basicGPUParticle )
	{
		basicGPUParticle->Update(deltaTime_);
	}
}

void GPUParticleEmitter::Draw(Camera* camera_)
{
	static_cast< void >( camera_ );
	if ( basicGPUParticle )
	{
		//basicGPUParticle->Draw(camera_);
	}
}

void GPUParticleEmitter::BasicGPUParticleEmit(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_)
{
	if (! basicGPUParticle )
	{
		basicGPUParticle = std::make_unique<BasicGPUParticle>();
		basicGPUParticle->SetAdapter(multiAdapters->GetMainAdapter(),multiAdapters->GetSubAdapter());
		basicGPUParticle->Initialize();
	}

	basicGPUParticle->ADD(pos_,setting_);
}

void GPUParticleEmitter::SetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	multiAdapters = multiAdapters_;
}
