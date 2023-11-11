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

	for ( std::pair<const std::string,std::unique_ptr<FireGPUParticle>>& particle : fireParticles )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ShockWaveGPUParticle>>& particle : shockWaveParticles )
	{
		particle.second->Update(deltaTime_);
	}

	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	if ( basicGPUParticle )
	{
		basicGPUParticle->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<FireGPUParticle>>& particle : fireParticles )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ShockWaveGPUParticle>>& particle : shockWaveParticles )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}
}

void GPUParticleEmitter::Finalize()
{
	for ( std::pair<const std::string,std::unique_ptr<FireGPUParticle>>& particle : fireParticles )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<ShockWaveGPUParticle>>& particle : shockWaveParticles )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	shockWaveParticles.clear();
	fireParticles.clear();
	basicGPUParticle.reset();

	BaseGPUParticle::BaseGPUParticleFinalize();

	multiAdapters = nullptr;
	swapChain = nullptr;

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

#pragma region 炎

void GPUParticleEmitter::FireParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<FireGPUParticle> lParticle = std::make_unique<FireGPUParticle>();
	lParticle->Create(maxParticles_);
	fireParticles[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::FireParticleEmit(const std::string& name_,const FireGPUParticleSetting& setting_,int32_t index_)
{
	return fireParticles[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::FireParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	fireParticles[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::FireParticleEmitPlay(const std::string& name_,int32_t index_)
{
	fireParticles[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::FireParticleEmitStop(const std::string& name_,int32_t index_)
{
	fireParticles[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::FireParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_)
{
	fireParticles[ name_ ]->SetPos(pos_,index_);
}

FireGPUParticle* GPUParticleEmitter::GetFireParticle(const std::string& name_)
{
	return fireParticles[ name_ ].get();
}

void GPUParticleEmitter::FireParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_)
{
	fireParticles[ name_ ]->Move(move_,index_);
}

#pragma endregion

#pragma region 衝撃波

void GPUParticleEmitter::ShockWaveParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<ShockWaveGPUParticle> lParticle = std::make_unique<ShockWaveGPUParticle>();
	lParticle->Create(maxParticles_);
	shockWaveParticles[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::ShockWaveParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_)
{
	shockWaveParticles[ name_ ]->Move(move_,index_);
}

int32_t GPUParticleEmitter::ShockWaveParticleEmit(const std::string& name_,const ShockWaveGPUParticleSetting& setting_,int32_t index_)
{
	return shockWaveParticles[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::ShockWaveParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	shockWaveParticles[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::ShockWaveParticleEmitPlay(const std::string& name_,int32_t index_)
{
	shockWaveParticles[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::ShockWaveParticleEmitStop(const std::string& name_,int32_t index_)
{
	shockWaveParticles[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::ShockWaveParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_)
{
	shockWaveParticles[ name_ ]->SetPos(pos_,index_);
}

ShockWaveGPUParticle* GPUParticleEmitter::GetShockWaveParticle(const std::string& name_)
{
	return shockWaveParticles[ name_ ].get();
}

#pragma endregion

void GPUParticleEmitter::SetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	multiAdapters = multiAdapters_;
}

void GPUParticleEmitter::SetSwapChain(ISwapChain* swapChain_)
{
	swapChain = swapChain_;
}
