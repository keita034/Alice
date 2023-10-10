#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

struct BasicGPUParticleSetting
{
	AliceMathF::Vector4 startColor;
	AliceMathF::Vector4 endColor;
	AliceMathF::Vector3 velocity;
	float LifeTime = 0.0f;
	AliceMathF::Vector3 acceleration;
	int32_t emitCount = 0;
	uint32_t maxParticles = 0;
};

class BasicGPUParticle : public BaseGPUParticle
{
private:
	AliceMathF::Matrix4  worldMatrix;

	D3D12_DRAW_ARGUMENTS drawArguments;

public:

	BasicGPUParticle() = default;
	~BasicGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(Camera* camera_) override;

	void ADD(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_);

private:

	void PUpdateConstantBuffer(float deltaTime_)override;

	//コピーコンストラクタ・代入演算子削除
	BasicGPUParticle& operator=(const BasicGPUParticle&) = delete;
	BasicGPUParticle(const BasicGPUParticle&) = delete;
};

