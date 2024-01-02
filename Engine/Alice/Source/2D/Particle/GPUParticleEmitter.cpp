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

	for ( std::pair<const std::string,std::unique_ptr<LaserGPUParticle>>& particle : laserGPUParticles )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticle>>& particle : animationMeshGPUParticles )
	{
		particle.second->Update(deltaTime_);
	}

	for (std::pair<const std::string, std::unique_ptr<MeshGPUParticle>>& particle : meshGPUParticles)
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<BloodGushGPUParticle>>& particle : bloodGushGPUParticles )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ModelGPUParticle>>& particle : modelGPUParticles )
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

	for ( std::pair<const std::string,std::unique_ptr<LaserGPUParticle>>& particle : laserGPUParticles )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticle>>& particle : animationMeshGPUParticles )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for (std::pair<const std::string, std::unique_ptr<MeshGPUParticle>>& particle : meshGPUParticles)
	{
		particle.second->Draw(worldMat_, billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<BloodGushGPUParticle>>& particle : bloodGushGPUParticles )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ModelGPUParticle>>& particle : modelGPUParticles )
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

	for ( std::pair<const std::string,std::unique_ptr<LaserGPUParticle>>& particle : laserGPUParticles )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticle>>& particle : animationMeshGPUParticles )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<BloodGushGPUParticle>>& particle : bloodGushGPUParticles )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for (std::pair<const std::string, std::unique_ptr<MeshGPUParticle>>& particle : meshGPUParticles)
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<ModelGPUParticle>>& particle : modelGPUParticles )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	shockWaveParticles.clear();
	fireParticles.clear();
	laserGPUParticles.clear();
	animationMeshGPUParticles.clear();
	meshGPUParticles.clear();
	bloodGushGPUParticles.clear();
	basicGPUParticle.reset();

	BaseGPUParticle::BaseGPUParticleFinalize();

	multiAdapters = nullptr;
	swapChain = nullptr;

}

void GPUParticleEmitter::BasicGPUParticleEmit(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_)
{
	if ( !basicGPUParticle )
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

#pragma region レーザー

void GPUParticleEmitter::LaserParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<LaserGPUParticle> lParticle = std::make_unique<LaserGPUParticle>();
	lParticle->Create(maxParticles_);
	laserGPUParticles[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::LaserParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_)
{
	laserGPUParticles[ name_ ]->Move(move_,index_);
}

int32_t GPUParticleEmitter::LaserParticleEmit(const std::string& name_,const LaserGPUParticleSetting& setting_,int32_t index_)
{
	return laserGPUParticles[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::LaserParticleSetMainTex(const std::string& name_,uint32_t textureHandle_)
{
	laserGPUParticles[ name_ ]->SetMainTex(textureHandle_);
}

void GPUParticleEmitter::LaserParticleSetSubTex(const std::string& name_,uint32_t textureHandle_)
{
	laserGPUParticles[ name_ ]->SetSubTex(textureHandle_);
}

void GPUParticleEmitter::LaserParticleEmitPlay(const std::string& name_,int32_t index_)
{
	laserGPUParticles[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::LaserParticleEmitStop(const std::string& name_,int32_t index_)
{
	laserGPUParticles[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::LaserParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_)
{
	laserGPUParticles[ name_ ]->SetPos(pos_,index_);
}

void GPUParticleEmitter::LaserParticleSetVelocity(const std::string& name_,const AliceMathF::Vector3& velocity_,int32_t index_)
{
	laserGPUParticles[ name_ ]->SetVelocity(velocity_,index_);
}

LaserGPUParticle* GPUParticleEmitter::GetLaserParticle(const std::string& name_)
{
	return laserGPUParticles[ name_ ].get();
}

#pragma endregion

#pragma region メッシュ

void GPUParticleEmitter::AnimationMeshGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<AnimationMeshGPUParticle> lParticle = std::make_unique<AnimationMeshGPUParticle>();
	lParticle->Create(maxParticles_);
	animationMeshGPUParticles[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::AnimationMeshGPUParticleEmit(const std::string& name_,const AnimationMeshGPUParticleSetting& setting_,int32_t index_)
{
	return animationMeshGPUParticles[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_,int32_t index_)
{
	animationMeshGPUParticles[ name_ ]->SetMat(matWorld_,index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetDetermineTex(const std::string& name_,uint32_t textureHandle_)
{
	animationMeshGPUParticles[ name_ ]->SetDetermineTex(textureHandle_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	animationMeshGPUParticles[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleEmitPlay(const std::string& name_,int32_t index_)
{
	animationMeshGPUParticles[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleEmitStop(const std::string& name_,int32_t index_)
{
	animationMeshGPUParticles[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	animationMeshGPUParticles[ name_ ]->SetModel(model_);
}

AnimationMeshGPUParticle* GPUParticleEmitter::GetAnimationMeshGPUParticle(const std::string& name_)
{
	return animationMeshGPUParticles[ name_ ].get();
}


void GPUParticleEmitter::MeshGPUParticleCreate(uint32_t maxParticles_, const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<MeshGPUParticle> lParticle = std::make_unique<MeshGPUParticle>();
	lParticle->Create(maxParticles_);
	meshGPUParticles.insert(make_pair(name_,std::move(lParticle)));
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::MeshGPUParticleEmit(const std::string& name_, const MeshGPUParticleSetting& setting_, int32_t index_)
{
	return meshGPUParticles[name_]->Emit(setting_, index_);
}

void GPUParticleEmitter::MeshGPUParticleSetMat(const std::string& name_, const AliceMathF::Matrix4& matWorld_, int32_t index_)
{
	meshGPUParticles[name_]->SetMat(matWorld_, index_);
}

void GPUParticleEmitter::MeshGPUParticleSetDetermineTex(const std::string& name_, uint32_t textureHandle_)
{
	meshGPUParticles[name_]->SetDetermineTex(textureHandle_);
}

void GPUParticleEmitter::MeshGPUParticleSetTex(const std::string& name_, uint32_t textureHandle_)
{
	meshGPUParticles[name_]->SetTex(textureHandle_);
}

void GPUParticleEmitter::MeshGPUParticleEmitPlay(const std::string& name_, int32_t index_)
{
	meshGPUParticles[name_]->EmitPlay(index_);
}

void GPUParticleEmitter::MeshGPUParticleEmitStop(const std::string& name_, int32_t index_)
{
	meshGPUParticles[name_]->EmitStop(index_);
}

void GPUParticleEmitter::MeshGPUParticleSetModel(const std::string& name_, AliceModel* model_)
{
	meshGPUParticles[name_]->SetModel(model_);
}

MeshGPUParticle* GPUParticleEmitter::GetMeshGPUParticle(const std::string& name_)
{
	return meshGPUParticles[name_].get();
}
#pragma endregion

void GPUParticleEmitter::BloodGushGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<BloodGushGPUParticle> lParticle = std::make_unique<BloodGushGPUParticle>();
	lParticle->Create(maxParticles_);
	bloodGushGPUParticles[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::BloodGushGPUParticleEmit(const std::string& name_,const BloodGushGPUParticleSetting& setting_,int32_t index_)
{
	return bloodGushGPUParticles[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::BloodGushGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	bloodGushGPUParticles[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::BloodGushGPUParticleEmitPlay(const std::string& name_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& velocity_,int32_t index_)
{
	bloodGushGPUParticles[ name_ ]->EmitPlay(pos_,velocity_,index_);
}

void GPUParticleEmitter::BloodGushGPUParticleEmitStop(const std::string& name_,int32_t index_)
{
	bloodGushGPUParticles[ name_ ]->EmitStop(index_);
}

BloodGushGPUParticle* GPUParticleEmitter::GetBloodGushGPUParticle(const std::string& name_)
{
	return bloodGushGPUParticles[ name_ ].get();
}

void GPUParticleEmitter::AnimationModelGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<ModelGPUParticle> lParticle = std::make_unique<ModelGPUParticle>();
	lParticle->Create(maxParticles_);
	modelGPUParticles[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::AnimationModelGPUParticleEmit(const std::string& name_,const ModelGPUParticleSetting& setting_)
{
	modelGPUParticles[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::AnimationModelGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_)
{
	modelGPUParticles[ name_ ]->SetMat(matWorld_);
}

void GPUParticleEmitter::AnimationModelGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	modelGPUParticles[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::AnimationModelGPUParticleEmitPlay(const std::string& name_)
{
	modelGPUParticles[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::AnimationModelGPUParticleEmitStop(const std::string& name_)
{
	modelGPUParticles[ name_ ]->EmitStop();
}

void GPUParticleEmitter::AnimationModelGPUParticleDrawListRelease(const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();

	modelGPUParticles[ name_ ]->DrawListRelease();

	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::AnimationModelGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	modelGPUParticles[ name_ ]->SetModel(model_);
}

ModelGPUParticle* GPUParticleEmitter::GetAnimationModelGPUParticle(const std::string& name_)
{
	return modelGPUParticles[ name_ ].get();
}

void GPUParticleEmitter::SetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	multiAdapters = multiAdapters_;
}

void GPUParticleEmitter::SetSwapChain(ISwapChain* swapChain_)
{
	swapChain = swapChain_;
}
