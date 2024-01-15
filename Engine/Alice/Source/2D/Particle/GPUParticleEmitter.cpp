#include "GPUParticleEmitter.h"


void GPUParticleEmitter::Initialize()
{

	BasicGPUParticle::SetAdapter(multiAdapters->GetMainAdapter(),multiAdapters->GetSubAdapter());
	BasicGPUParticle::SetSwapChain(swapChain);

	BaseGPUParticle::ParticleBegin();

	scattering = std::make_unique<ScatteringGPUParticle>();
	scattering->Initialize();

	BaseGPUParticle::ParticleEnd();

}

void GPUParticleEmitter::Update(float deltaTime_)
{
	BaseGPUParticle::ParticleBegin();

	for ( std::pair<const std::string,std::unique_ptr<FireGPUParticle>>& particle : fires )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ShockWaveGPUParticle>>& particle : shockWaves )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<LaserGPUParticle>>& particle : lasers )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticle>>& particle : animationMeshs )
	{
		particle.second->Update(deltaTime_);
	}

	for (std::pair<const std::string, std::unique_ptr<MeshGPUParticle>>& particle : meshs)
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<BloodGushGPUParticle>>& particle : bloodGushs )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationModelGPUParticle>>& particle : animationModels )
	{
		particle.second->Update(deltaTime_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ModelGPUParticle>>& particle : models )
	{
		particle.second->Update(deltaTime_);
	}

	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{

	for ( std::pair<const std::string,std::unique_ptr<FireGPUParticle>>& particle : fires )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ShockWaveGPUParticle>>& particle : shockWaves )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<LaserGPUParticle>>& particle : lasers )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticle>>& particle : animationMeshs )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for (std::pair<const std::string, std::unique_ptr<MeshGPUParticle>>& particle : meshs)
	{
		particle.second->Draw(worldMat_, billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<BloodGushGPUParticle>>& particle : bloodGushs )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationModelGPUParticle>>& particle : animationModels )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}

	for ( std::pair<const std::string,std::unique_ptr<ModelGPUParticle>>& particle : models )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}
}

void GPUParticleEmitter::Finalize()
{
	for ( std::pair<const std::string,std::unique_ptr<FireGPUParticle>>& particle : fires )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<ShockWaveGPUParticle>>& particle : shockWaves )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<LaserGPUParticle>>& particle : lasers )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticle>>& particle : animationMeshs )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<BloodGushGPUParticle>>& particle : bloodGushs )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for (std::pair<const std::string, std::unique_ptr<MeshGPUParticle>>& particle : meshs)
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationModelGPUParticle>>& particle : animationModels )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<ModelGPUParticle>>& particle : models )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	shockWaves.clear();
	fires.clear();
	lasers.clear();
	animationMeshs.clear();
	meshs.clear();
	bloodGushs.clear();
	models.clear();

	BaseGPUParticle::BaseGPUParticleFinalize();

	multiAdapters = nullptr;
	swapChain = nullptr;

}

#pragma region 炎

void GPUParticleEmitter::FireParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<FireGPUParticle> lParticle = std::make_unique<FireGPUParticle>();
	lParticle->Create(maxParticles_);
	fires[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::FireParticleEmit(const std::string& name_,const FireGPUParticleSetting& setting_,int32_t index_)
{
	return fires[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::FireParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	fires[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::FireParticleEmitPlay(const std::string& name_,int32_t index_)
{
	fires[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::FireParticleEmitStop(const std::string& name_,int32_t index_)
{
	fires[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::FireParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_)
{
	fires[ name_ ]->SetPos(pos_,index_);
}

FireGPUParticle* GPUParticleEmitter::GetFireParticle(const std::string& name_)
{
	return fires[ name_ ].get();
}

void GPUParticleEmitter::FireParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_)
{
	fires[ name_ ]->Move(move_,index_);
}

#pragma endregion

#pragma region 衝撃波

void GPUParticleEmitter::ShockWaveParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<ShockWaveGPUParticle> lParticle = std::make_unique<ShockWaveGPUParticle>();
	lParticle->Create(maxParticles_);
	shockWaves[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::ShockWaveParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_)
{
	shockWaves[ name_ ]->Move(move_,index_);
}

int32_t GPUParticleEmitter::ShockWaveParticleEmit(const std::string& name_,const ShockWaveGPUParticleSetting& setting_,int32_t index_)
{
	return shockWaves[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::ShockWaveParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	shockWaves[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::ShockWaveParticleEmitPlay(const std::string& name_,int32_t index_)
{
	shockWaves[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::ShockWaveParticleEmitStop(const std::string& name_,int32_t index_)
{
	shockWaves[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::ShockWaveParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_)
{
	shockWaves[ name_ ]->SetPos(pos_,index_);
}

ShockWaveGPUParticle* GPUParticleEmitter::GetShockWaveParticle(const std::string& name_)
{
	return shockWaves[ name_ ].get();
}

#pragma endregion

#pragma region レーザー

void GPUParticleEmitter::LaserParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<LaserGPUParticle> lParticle = std::make_unique<LaserGPUParticle>();
	lParticle->Create(maxParticles_);
	lasers[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::LaserParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_)
{
	lasers[ name_ ]->Move(move_,index_);
}

int32_t GPUParticleEmitter::LaserParticleEmit(const std::string& name_,const LaserGPUParticleSetting& setting_,int32_t index_)
{
	return lasers[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::LaserParticleSetMainTex(const std::string& name_,uint32_t textureHandle_)
{
	lasers[ name_ ]->SetMainTex(textureHandle_);
}

void GPUParticleEmitter::LaserParticleSetSubTex(const std::string& name_,uint32_t textureHandle_)
{
	lasers[ name_ ]->SetSubTex(textureHandle_);
}

void GPUParticleEmitter::LaserParticleEmitPlay(const std::string& name_,int32_t index_)
{
	lasers[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::LaserParticleEmitStop(const std::string& name_,int32_t index_)
{
	lasers[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::LaserParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_)
{
	lasers[ name_ ]->SetPos(pos_,index_);
}

void GPUParticleEmitter::LaserParticleSetVelocity(const std::string& name_,const AliceMathF::Vector3& velocity_,int32_t index_)
{
	lasers[ name_ ]->SetVelocity(velocity_,index_);
}

LaserGPUParticle* GPUParticleEmitter::GetLaserParticle(const std::string& name_)
{
	return lasers[ name_ ].get();
}

#pragma endregion

#pragma region メッシュ

#pragma region アニメーション

void GPUParticleEmitter::AnimationMeshGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<AnimationMeshGPUParticle> lParticle = std::make_unique<AnimationMeshGPUParticle>();
	lParticle->Create(maxParticles_);
	animationMeshs[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::AnimationMeshGPUParticleEmit(const std::string& name_,const AnimationMeshGPUParticleSetting& setting_,int32_t index_)
{
	return animationMeshs[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_,int32_t index_)
{
	animationMeshs[ name_ ]->SetMat(matWorld_,index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetDetermineTex(const std::string& name_,uint32_t textureHandle_)
{
	animationMeshs[ name_ ]->SetDetermineTex(textureHandle_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	animationMeshs[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleEmitPlay(const std::string& name_,int32_t index_)
{
	animationMeshs[ name_ ]->EmitPlay(index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleEmitStop(const std::string& name_,int32_t index_)
{
	animationMeshs[ name_ ]->EmitStop(index_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	animationMeshs[ name_ ]->SetModel(model_);
}

AnimationMeshGPUParticle* GPUParticleEmitter::GetAnimationMeshGPUParticle(const std::string& name_)
{
	return animationMeshs[ name_ ].get();
}

#pragma endregion

#pragma region ノンアニメーション

void GPUParticleEmitter::MeshGPUParticleCreate(uint32_t maxParticles_, const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<MeshGPUParticle> lParticle = std::make_unique<MeshGPUParticle>();
	lParticle->Create(maxParticles_);
	meshs.insert(make_pair(name_,std::move(lParticle)));
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::MeshGPUParticleEmit(const std::string& name_, const MeshGPUParticleSetting& setting_, int32_t index_)
{
	return meshs[name_]->Emit(setting_, index_);
}

void GPUParticleEmitter::MeshGPUParticleSetMat(const std::string& name_, const AliceMathF::Matrix4& matWorld_, int32_t index_)
{
	meshs[name_]->SetMat(matWorld_, index_);
}

void GPUParticleEmitter::MeshGPUParticleSetDetermineTex(const std::string& name_, uint32_t textureHandle_)
{
	meshs[name_]->SetDetermineTex(textureHandle_);
}

void GPUParticleEmitter::MeshGPUParticleSetTex(const std::string& name_, uint32_t textureHandle_)
{
	meshs[name_]->SetTex(textureHandle_);
}

void GPUParticleEmitter::MeshGPUParticleEmitPlay(const std::string& name_, int32_t index_)
{
	meshs[name_]->EmitPlay(index_);
}

void GPUParticleEmitter::MeshGPUParticleEmitStop(const std::string& name_, int32_t index_)
{
	meshs[name_]->EmitStop(index_);
}

void GPUParticleEmitter::MeshGPUParticleSetModel(const std::string& name_, AliceModel* model_)
{
	meshs[name_]->SetModel(model_);
}

MeshGPUParticle* GPUParticleEmitter::GetMeshGPUParticle(const std::string& name_)
{
	return meshs[name_].get();
}
#pragma endregion

#pragma endregion

void GPUParticleEmitter::AnimationMeshGPUParticleScattering(const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	scattering->SetVelocityMeshGPUParticle(animationMeshs[ name_ ].get());
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::MeshGPUParticleScattering(const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	scattering->SetVelocityMeshGPUParticle(meshs[ name_ ].get());
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::ScatteringSetSpeed(float speed_)
{
	scattering->SetSpeed(speed_);
}

void GPUParticleEmitter::ScatteringSetAccel(const AliceMathF::Vector3& accel_)
{
	scattering->SetAccel(accel_);
}

void GPUParticleEmitter::ScatteringSetLifeTime(float lifeTime_)
{
	scattering->SetLifeTime(lifeTime_);
}

void GPUParticleEmitter::ScatteringSetCenterPos(const AliceMathF::Vector3& centerPos_)
{
	scattering->SetCenterPos(centerPos_);
}

#pragma region 血しぶき

void GPUParticleEmitter::BloodGushGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<BloodGushGPUParticle> lParticle = std::make_unique<BloodGushGPUParticle>();
	lParticle->Create(maxParticles_);
	bloodGushs[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

int32_t GPUParticleEmitter::BloodGushGPUParticleEmit(const std::string& name_,const BloodGushGPUParticleSetting& setting_,int32_t index_)
{
	return bloodGushs[ name_ ]->Emit(setting_,index_);
}

void GPUParticleEmitter::BloodGushGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	bloodGushs[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::BloodGushGPUParticleEmitPlay(const std::string& name_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& velocity_,int32_t index_)
{
	bloodGushs[ name_ ]->EmitPlay(pos_,velocity_,index_);
}

void GPUParticleEmitter::BloodGushGPUParticleEmitStop(const std::string& name_,int32_t index_)
{
	bloodGushs[ name_ ]->EmitStop(index_);
}

BloodGushGPUParticle* GPUParticleEmitter::GetBloodGushGPUParticle(const std::string& name_)
{
	return bloodGushs[ name_ ].get();
}

void GPUParticleEmitter::AnimationModelGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<AnimationModelGPUParticle> lParticle = std::make_unique<AnimationModelGPUParticle>();
	lParticle->Create(maxParticles_);
	animationModels[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

#pragma endregion

#pragma region モデル

#pragma region アニメーション

void GPUParticleEmitter::AnimationModelGPUParticleEmit(const std::string& name_,const AnimationModelGPUParticleSetting& setting_)
{
	animationModels[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::AnimationModelGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_)
{
	animationModels[ name_ ]->SetMat(matWorld_);
}

void GPUParticleEmitter::AnimationModelGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	animationModels[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::AnimationModelGPUParticleEmitPlay(const std::string& name_)
{
	animationModels[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::AnimationModelGPUParticleEmitStop(const std::string& name_)
{
	animationModels[ name_ ]->EmitStop();
}

void GPUParticleEmitter::AnimationModelGPUParticleDrawListRelease(const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();

	animationModels[ name_ ]->DrawListRelease();

	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::AnimationModelGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	animationModels[ name_ ]->SetModel(model_);
}

AnimationModelGPUParticle* GPUParticleEmitter::GetAnimationModelGPUParticle(const std::string& name_)
{
	return animationModels[ name_ ].get();
}
#pragma endregion

#pragma region ノンアニメーション

void GPUParticleEmitter::ModelGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<ModelGPUParticle> lParticle = std::make_unique<ModelGPUParticle>();
	lParticle->Create(maxParticles_);
	models[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::ModelGPUParticleEmit(const std::string& name_,const ModelGPUParticleSetting& setting_)
{
	models[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::ModelGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_)
{
	models[ name_ ]->SetMat(matWorld_);
}

void GPUParticleEmitter::ModelGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	models[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::ModelGPUParticleEmitPlay(const std::string& name_)
{
	models[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::ModelGPUParticleEmitStop(const std::string& name_)
{
	models[ name_ ]->EmitStop();
}

void GPUParticleEmitter::ModelGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	models[ name_ ]->SetModel(model_);
}

ModelGPUParticle* GPUParticleEmitter::GetModelGPUParticle(const std::string& name_)
{
	return models[ name_ ].get();
}

#pragma endregion

#pragma endregion

void GPUParticleEmitter::SetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	multiAdapters = multiAdapters_;
}

void GPUParticleEmitter::SetSwapChain(ISwapChain* swapChain_)
{
	swapChain = swapChain_;
}
