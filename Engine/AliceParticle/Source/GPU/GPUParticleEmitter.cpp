#include "GPUParticleEmitter.h"


void GPUParticleEmitter::Initialize()
{

	BasicGPUParticle::SetAdapter(multiAdapters->GetMainAdapter(),multiAdapters->GetSubAdapter());
	BasicGPUParticle::SetSwapChain(swapChain);

	BaseGPUParticle::ParticleBegin();

	scattering = std::make_unique<ScatteringGPUParticle>();
	scattering->Initialize();

	aggregating = std::make_unique<AggregatingGPUParticle>();
	aggregating->Initialize();

	BaseGPUParticle::ParticleEnd();

}

void GPUParticleEmitter::Update(float deltaTime_)
{
	BaseGPUParticle::ParticleBegin();

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

	for ( std::pair<const std::string,std::unique_ptr<AnimationModelSuctionGPUParticle>>& particle : animationModelSuction )
	{
		particle.second->Update(deltaTime_);
	}

	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
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

	for ( std::pair<const std::string,std::unique_ptr<AnimationModelSuctionGPUParticle>>& particle : animationModelSuction )
	{
		particle.second->Draw(worldMat_,billboardMat_);
	}
}

void GPUParticleEmitter::Finalize()
{
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

	for ( std::pair<const std::string,std::unique_ptr<ModelSuctionGPUParticle>>& particle : modelSuction )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<AnimationModelSuctionGPUParticle>>& particle : animationModelSuction )
	{
		particle.second->Finalize();
		particle.second.reset();
	}

	lasers.clear();
	animationMeshs.clear();
	meshs.clear();
	bloodGushs.clear();
	models.clear();
	animationModelSuction.clear();

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

void GPUParticleEmitter::FireParticleEmit(const std::string& name_,const FireGPUParticleSetting& setting_)
{
	fires[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::FireParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	fires[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::FireParticleEmitPlay(const std::string& name_)
{
	fires[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::FireParticleEmitStop(const std::string& name_)
{
	fires[ name_ ]->EmitStop();
}

void GPUParticleEmitter::FireParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_)
{
	fires[ name_ ]->SetPos(pos_);
}

FireGPUParticle* GPUParticleEmitter::GetFireParticle(const std::string& name_)
{
	return fires[ name_ ].get();
}

void GPUParticleEmitter::FireParticleMove(const std::string& name_,const AliceMathF::Vector3& move_)
{
	fires[ name_ ]->Move(move_);
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

void GPUParticleEmitter::ShockWaveParticleMove(const std::string& name_,const AliceMathF::Vector3& move_)
{
	shockWaves[ name_ ]->Move(move_);
}

void GPUParticleEmitter::ShockWaveParticleEmit(const std::string& name_,const ShockWaveGPUParticleSetting& setting_)
{
	return shockWaves[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::ShockWaveParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	shockWaves[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::ShockWaveParticleEmitPlay(const std::string& name_)
{
	shockWaves[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::ShockWaveParticleEmitStop(const std::string& name_)
{
	shockWaves[ name_ ]->EmitStop();
}

void GPUParticleEmitter::ShockWaveParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_)
{
	shockWaves[ name_ ]->SetPos(pos_);
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

void GPUParticleEmitter::LaserParticleMove(const std::string& name_,const AliceMathF::Vector3& move_)
{
	lasers[ name_ ]->Move(move_);
}

void GPUParticleEmitter::LaserParticleEmit(const std::string& name_,const LaserGPUParticleSetting& setting_)
{
	lasers[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::LaserParticleSetMainTex(const std::string& name_,uint32_t textureHandle_)
{
	lasers[ name_ ]->SetMainTex(textureHandle_);
}

void GPUParticleEmitter::LaserParticleSetSubTex(const std::string& name_,uint32_t textureHandle_)
{
	lasers[ name_ ]->SetSubTex(textureHandle_);
}

void GPUParticleEmitter::LaserParticleEmitPlay(const std::string& name_)
{
	lasers[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::LaserParticleEmitStop(const std::string& name_)
{
	lasers[ name_ ]->EmitStop();
}

void GPUParticleEmitter::LaserParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_)
{
	lasers[ name_ ]->SetPos(pos_);
}

void GPUParticleEmitter::LaserParticleSetVelocity(const std::string& name_,const AliceMathF::Vector3& velocity_)
{
	lasers[ name_ ]->SetVelocity(velocity_);
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

void GPUParticleEmitter::AnimationMeshGPUParticleEmit(const std::string& name_,const AnimationMeshGPUParticleSetting& setting_)
{
	animationMeshs[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_)
{
	animationMeshs[ name_ ]->SetMat(matWorld_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetDetermineTex(const std::string& name_,uint32_t textureHandle_)
{
	animationMeshs[ name_ ]->SetDetermineTex(textureHandle_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	animationMeshs[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::AnimationMeshGPUParticleEmitPlay(const std::string& name_)
{
	animationMeshs[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::AnimationMeshGPUParticleEmitStop(const std::string& name_)
{
	animationMeshs[ name_ ]->EmitStop();
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

void GPUParticleEmitter::MeshGPUParticleEmit(const std::string& name_, const MeshGPUParticleSetting& setting_)
{
	meshs[name_]->Emit(setting_);
}

void GPUParticleEmitter::MeshGPUParticleSetMat(const std::string& name_, const AliceMathF::Matrix4& matWorld_)
{
	meshs[name_]->SetMat(matWorld_);
}

void GPUParticleEmitter::MeshGPUParticleSetDetermineTex(const std::string& name_, uint32_t textureHandle_)
{
	meshs[name_]->SetDetermineTex(textureHandle_);
}

void GPUParticleEmitter::MeshGPUParticleSetTex(const std::string& name_, uint32_t textureHandle_)
{
	meshs[name_]->SetTex(textureHandle_);
}

void GPUParticleEmitter::MeshGPUParticleEmitPlay(const std::string& name_)
{
	meshs[name_]->EmitPlay();
}

void GPUParticleEmitter::MeshGPUParticleEmitStop(const std::string& name_)
{
	meshs[name_]->EmitStop();
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

void GPUParticleEmitter::AnimationMeshGPUParticleAggregating(const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	aggregating->SetVelocityMeshGPUParticle(animationMeshs[ name_ ].get());
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::MeshGPUParticleAggregating(const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	aggregating->SetVelocityMeshGPUParticle(meshs[ name_ ].get());
	BaseGPUParticle::ParticleEnd();
}

void GPUParticleEmitter::AggregatingSetSpeed(float speed_)
{
	aggregating->SetSpeed(speed_);
}

void GPUParticleEmitter::AggregatingSetLifeTime(float lifeTime_)
{
	aggregating->SetLifeTime(lifeTime_);
}

void GPUParticleEmitter::AggregatingSetCenterPos(const AliceMathF::Vector3& centerPos_)
{
	aggregating->SetCenterPos(centerPos_);
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

void GPUParticleEmitter::BloodGushGPUParticleEmit(const std::string& name_,const BloodGushGPUParticleSetting& setting_)
{
	bloodGushs[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::BloodGushGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	bloodGushs[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::BloodGushGPUParticleEmitPlay(const std::string& name_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& velocity_)
{
	bloodGushs[ name_ ]->EmitPlay(pos_,velocity_);
}

void GPUParticleEmitter::BloodGushGPUParticleEmitStop(const std::string& name_)
{
	bloodGushs[ name_ ]->EmitStop();
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

void GPUParticleEmitter::ModelSuctionGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<ModelSuctionGPUParticle> lParticle = std::make_unique<ModelSuctionGPUParticle>();
	lParticle->Create(maxParticles_);
	modelSuction[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

ModelSuctionGPUParticle* GPUParticleEmitter::GetModelSuctionGPUParticle(const std::string& name_)
{
	return modelSuction[ name_ ].get();
}

void GPUParticleEmitter::ModelSuctionGPUParticleEmit(const std::string& name_,const ModelSuctionGPUParticleSetting& setting_)
{
	modelSuction[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::ModelSuctionGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_)
{
	modelSuction[ name_ ]->SetMat(matWorld_);
}

void GPUParticleEmitter::ModelSuctionGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	modelSuction[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::ModelSuctionGPUParticleEmitPlay(const std::string& name_)
{
	modelSuction[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::ModelSuctionGPUParticleEmitStop(const std::string& name_)
{
	modelSuction[ name_ ]->EmitStop();
}

void GPUParticleEmitter::ModelSuctionGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	modelSuction[ name_ ]->SetModel(model_);
}


#pragma endregion

void GPUParticleEmitter::AnimationModelSuctionGPUParticleCreate(uint32_t maxParticles_,const std::string& name_)
{
	BaseGPUParticle::ParticleBegin();
	std::unique_ptr<AnimationModelSuctionGPUParticle> lParticle = std::make_unique<AnimationModelSuctionGPUParticle>();
	lParticle->Create(maxParticles_);
	animationModelSuction[ name_ ] = std::move(lParticle);
	BaseGPUParticle::ParticleEnd();
}

AnimationModelSuctionGPUParticle* GPUParticleEmitter::GetAnimationModelSuctionGPUParticle(const std::string& name_)
{
	return animationModelSuction[ name_ ].get();
}

void GPUParticleEmitter::AnimationModelSuctionGPUParticleEmit(const std::string& name_,const AnimationModelSuctionGPUParticleSetting& setting_)
{
	animationModelSuction[ name_ ]->Emit(setting_);
}

void GPUParticleEmitter::AnimationModelSuctionGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_)
{
	animationModelSuction[ name_ ]->SetMat(matWorld_);
}

void GPUParticleEmitter::AnimationModelSuctionGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_)
{
	animationModelSuction[ name_ ]->SetTex(textureHandle_);
}

void GPUParticleEmitter::AnimationModelSuctionGPUParticleEmitPlay(const std::string& name_)
{
	animationModelSuction[ name_ ]->EmitPlay();
}

void GPUParticleEmitter::AnimationModelSuctionGPUParticleEmitStop(const std::string& name_)
{
	animationModelSuction[ name_ ]->EmitStop();
}

void GPUParticleEmitter::AnimationModelSuctionGPUParticleSetModel(const std::string& name_,AliceModel* model_)
{
	animationModelSuction[ name_ ]->SetModel(model_);
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
