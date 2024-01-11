#pragma once
#include<AliceMathF.h>
#include<MeshGPUParticle.h>

class ScatteringGPUParticle : public BaseGPUParticle
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

	void SetCenterPos(const AliceMathF::Vector3& centerPos_);

	void SetVelocityMeshGPUParticle(MeshGPUParticle* meshGPUParticle_);

	void SetSpeed(float speed_);

	void SetAccel(const AliceMathF::Vector3& accel_);

	void SetLifeTime(float lifeTime_);

	ScatteringGPUParticle() = default;
	~ScatteringGPUParticle() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	ScatteringGPUParticle& operator=(const ScatteringGPUParticle&) = delete;
	ScatteringGPUParticle(const ScatteringGPUParticle&) = delete;
};

