#pragma once
#include<BasicGPUParticle.h>
#include<FireGPUParticle.h>
#include<Camera.h>
#include<MultiAdapters.h>

class GPUParticleEmitter
{
private:

	IMultiAdapters* multiAdapters = nullptr;
	ISwapChain* swapChain = nullptr;

	std::unique_ptr<BasicGPUParticle>basicGPUParticle;

	std::unordered_map<std::string,std::unique_ptr<FireGPUParticle>>fireParticles;

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

	void SetMultiAdapters(IMultiAdapters* multiAdapters_);
	void SetSwapChain(ISwapChain* swapChain_);

	GPUParticleEmitter() = default;
	~GPUParticleEmitter() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	GPUParticleEmitter& operator=(const GPUParticleEmitter&) = delete;
	GPUParticleEmitter(const GPUParticleEmitter&) = delete;

};