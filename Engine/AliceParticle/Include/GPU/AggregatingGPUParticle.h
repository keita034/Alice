#pragma once
#include<AliceMathF.h>
#include<MeshGPUParticle.h>
#include<AnimationMeshGPUParticle.h>

class AggregatingGPUParticle : public BaseGPUParticle
{
private:

	struct GPUData
	{
		AliceMathF::Vector3 centerPos;
		float speed;
		AliceMathF::Vector3 accel;
		float lifeTime;
		uint32_t maxParticles;
	};


	std::unique_ptr<IConstantBuffer>constantBuffer;
	AliceMathF::Vector3 centerPos;
	float lifeTime;
	AliceMathF::Vector3 accel;
	float speed;

	bool dirty = false;

	Byte7 PADING;

public:

	void Initialize();

	void SetVelocityMeshGPUParticle(MeshGPUParticle* meshGPUParticle_);
	void SetVelocityMeshGPUParticle(AnimationMeshGPUParticle* meshGPUParticle_);

	void SetSpeed(float speed_);
	void SetAccel(const AliceMathF::Vector3& accel_);
	void SetLifeTime(float lifeTime_);
	void SetCenterPos(const AliceMathF::Vector3& centerPos_);

	AggregatingGPUParticle() = default;
	~AggregatingGPUParticle() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	AggregatingGPUParticle& operator=(const AggregatingGPUParticle&) = delete;
	AggregatingGPUParticle(const AggregatingGPUParticle&) = delete;

	// BaseGPUParticle を介して継承されました
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting() override;
	void PUpdateConstantBuffer(float deltaTime_) override;
};

