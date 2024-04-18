#pragma once
#include<AliceMathF.h>
#include<MeshGPUParticle.h>
#include<AnimationMeshGPUParticle.h>

class StopGPUParticle : public BaseGPUParticle
{
private:

	struct GPUData
	{
		AliceMathF::Vector3 centerPos;
		float radius;
		uint32_t maxParticles;
	};


	std::unique_ptr<IConstantBuffer>constantBuffer;
	AliceMathF::Vector3 centerPos;
	float radius;

	bool dirty = false;

	Byte7 PADING;

public:

	void Initialize();

	void SetVelocityMeshGPUParticle(MeshGPUParticle* meshGPUParticle_);
	void SetVelocityMeshGPUParticle(AnimationMeshGPUParticle* meshGPUParticle_);


	void SetRadius(float radius_);
	void SetCenterPos(const AliceMathF::Vector3& centerPos_);

	StopGPUParticle() = default;
	~StopGPUParticle() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	StopGPUParticle& operator=(const StopGPUParticle&) = delete;
	StopGPUParticle(const StopGPUParticle&) = delete;

	// BaseGPUParticle を介して継承されました
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting() override;
	void PUpdateConstantBuffer(float deltaTime_) override;
};

