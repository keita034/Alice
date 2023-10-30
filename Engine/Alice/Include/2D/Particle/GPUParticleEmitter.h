#pragma once
#include<BasicGPUParticle.h>
#include<Camera.h>
#include<MultiAdapters.h>

class GPUParticleEmitter
{
private:

	IMultiAdapters* multiAdapters = nullptr;
	ISwapChain* swapChain = nullptr;

	std::unique_ptr<BasicGPUParticle>basicGPUParticle;


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

	void BasicGPUParticleEmit(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_);

	void SetMultiAdapters(IMultiAdapters* multiAdapters_);
	void SetSwapChain(ISwapChain* swapChain_);

	GPUParticleEmitter() = default;
	~GPUParticleEmitter() = default;

private:

	//コピーコンストラクタ・代入演算子削除
	GPUParticleEmitter& operator=(const GPUParticleEmitter&) = delete;
	GPUParticleEmitter(const GPUParticleEmitter&) = delete;

};