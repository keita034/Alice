#pragma once
#include<BasicGPUParticle.h>
#include<FireGPUParticle.h>
#include<ShockWaveGPUParticle.h>
#include<LaserGPUParticle.h>
#include<AnimationMeshGPUParticle.h>
#include<BloodGushGPUParticle.h>
#include<Camera.h>
#include<MultiAdapters.h>

class GPUParticleEmitter
{
private:

	IMultiAdapters* multiAdapters = nullptr;
	ISwapChain* swapChain = nullptr;

	std::unique_ptr<BasicGPUParticle>basicGPUParticle;

	std::unordered_map<std::string,std::unique_ptr<FireGPUParticle>>fireParticles;
	std::unordered_map<std::string,std::unique_ptr<ShockWaveGPUParticle>>shockWaveParticles;
	std::unordered_map<std::string,std::unique_ptr<LaserGPUParticle>>laserGPUParticles;
	std::unordered_map<std::string,std::unique_ptr<AnimationMeshGPUParticle>>AnimationMeshGPUParticles;
	std::unordered_map<std::string,std::unique_ptr<BloodGushGPUParticle>>bloodGushGPUParticles;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void Update(float deltaTime_);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_);

	void Finalize();

	void BasicGPUParticleEmit(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_);

	void FireParticleCreate(uint32_t maxParticles_,const std::string& name_);
	void FireParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_);
	int32_t FireParticleEmit(const std::string& name_,const FireGPUParticleSetting& setting_,int32_t index_ = -1);
	void FireParticleSetTex(const std::string& name_,uint32_t textureHandle_);
	void FireParticleEmitPlay(const std::string& name_,int32_t index_);
	void FireParticleEmitStop(const std::string& name_,int32_t index_);
	void FireParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_);
	FireGPUParticle* GetFireParticle(const std::string& name_);

	void ShockWaveParticleCreate(uint32_t maxParticles_,const std::string& name_);
	void ShockWaveParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_);
	int32_t ShockWaveParticleEmit(const std::string& name_,const ShockWaveGPUParticleSetting& setting_,int32_t index_ = -1);
	void ShockWaveParticleSetTex(const std::string& name_,uint32_t textureHandle_);
	void ShockWaveParticleEmitPlay(const std::string& name_,int32_t index_);
	void ShockWaveParticleEmitStop(const std::string& name_,int32_t index_);
	void ShockWaveParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_);
	ShockWaveGPUParticle* GetShockWaveParticle(const std::string& name_);

	void LaserParticleCreate(uint32_t maxParticles_,const std::string& name_);
	void LaserParticleMove(const std::string& name_,const AliceMathF::Vector3& move_,int32_t index_);
	int32_t LaserParticleEmit(const std::string& name_,const LaserGPUParticleSetting& setting_,int32_t index_ = -1);
	void LaserParticleSetMainTex(const std::string& name_,uint32_t textureHandle_);
	void LaserParticleSetSubTex(const std::string& name_,uint32_t textureHandle_);
	void LaserParticleEmitPlay(const std::string& name_,int32_t index_);
	void LaserParticleEmitStop(const std::string& name_,int32_t index_);
	void LaserParticleSetPos(const std::string& name_,const AliceMathF::Vector3& pos_,int32_t index_);
	void LaserParticleSetVelocity(const std::string& name_,const AliceMathF::Vector3& velocity_,int32_t index_);
	LaserGPUParticle* GetLaserParticle(const std::string& name_);

	void AnimationMeshGPUParticleCreate(uint32_t maxParticles_,const std::string& name_);
	int32_t AnimationMeshGPUParticleEmit(const std::string& name_,const AnimationMeshGPUParticleSetting& setting_,int32_t index_ = -1);
	void AnimationMeshGPUParticleSetMat(const std::string& name_,const AliceMathF::Matrix4& matWorld_,int32_t index_);
	void AnimationMeshGPUParticleSetDetermineTex(const std::string& name_,uint32_t textureHandle_);
	void AnimationMeshGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_);
	void AnimationMeshGPUParticleEmitPlay(const std::string& name_,int32_t index_);
	void AnimationMeshGPUParticleEmitStop(const std::string& name_,int32_t index_);
	void AnimationMeshGPUParticleSetModel(const std::string& name_,AliceModel* model_);
	AnimationMeshGPUParticle* GetAnimationMeshGPUParticle(const std::string& name_);

	void BloodGushGPUParticleCreate(uint32_t maxParticles_,const std::string& name_);
	int32_t BloodGushGPUParticleEmit(const std::string& name_,const BloodGushGPUParticleSetting& setting_,int32_t index_ = -1);
	void BloodGushGPUParticleSetTex(const std::string& name_,uint32_t textureHandle_);
	void BloodGushGPUParticleEmitPlay(const std::string& name_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& velocity_,int32_t index_);
	void BloodGushGPUParticleEmitStop(const std::string& name_,int32_t index_);
	BloodGushGPUParticle* GetBloodGushGPUParticle(const std::string& name_);

	void SetMultiAdapters(IMultiAdapters* multiAdapters_);
	void SetSwapChain(ISwapChain* swapChain_);

	GPUParticleEmitter() = default;
	~GPUParticleEmitter() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	GPUParticleEmitter& operator=(const GPUParticleEmitter&) = delete;
	GPUParticleEmitter(const GPUParticleEmitter&) = delete;

};