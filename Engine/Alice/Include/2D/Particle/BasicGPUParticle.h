#pragma once

#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>


struct BasicGPUParticleSetting
{
	//スタート時のカラー
	AliceMathF::Vector4 startColor;
	//終了時のカラー
	AliceMathF::Vector4 endColor;
	//方向ベクトル
	AliceMathF::Vector3 velocity;
	AliceMathF::Vector3 emitRadius;
	//生存時間
	float LifeTime = 0.0f;
	//加速度
	AliceMathF::Vector3 acceleration;
	//一回で生まれる数
	uint32_t emitCount = 0;
	//パーティクルの最大数
	uint32_t maxParticles = 0;
	AliceMathF::Vector2 scale;
	AliceMathF::Vector2 rotation;
	//エミッタ－の生存時間
	int64_t emitLifeTime;
	//パーティクルが生まれる時間
	float timeBetweenEmit;

private:
	Byte4 PADING;
};



class BasicGPUParticle : public BaseGPUParticle
{
public:

	struct ParticleConstant
	{
		AliceMathF::Vector3 startPosition;
		float pad1;
		AliceMathF::Vector3 velocity;
		float lifeTime;
		AliceMathF::Vector3 accel;
		float pad2;
		AliceMathF::Vector2 scale;
		AliceMathF::Vector2 rotation;
		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;
	};

	struct WorldBillboard
	{
		AliceMathF::Matrix4 worldMat;
		AliceMathF::Matrix4 billboardMat;
	};

	struct TimeConstants
	{
		float deltaTime = 0.0f;
		float totalTime = 0.0f;
	};

	struct EmitData
	{
		uint32_t maxParticles = 0;
		uint32_t particleDataCount = 0;
		uint32_t emitCount;
	};


	struct ParticleData
	{
		AliceMathF::Vector4 color;
		AliceMathF::Vector3 position;
		float age;
		AliceMathF::Vector3 velocity;
		float size;
		float alive;
		AliceMathF::Vector3 accel;
		uint32_t index;
	};

private:

	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;
	D3D12_DRAW_ARGUMENTS drawArguments;

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	std::unique_ptr<IConstantBuffer>particleConstantsBuffer;
	std::unique_ptr<IConstantBuffer>worldBillboardBuffer;
	std::unique_ptr<IConstantBuffer>timeConstantsBuffer;
	std::unique_ptr<IConstantBuffer>emitDataBuffer;

	std::unique_ptr<IRWStructuredBuffer>particlePoolBuffer;

	int64_t emitLifeTime;
	float timeBetweenEmit;
	float emitTimeCounter;

	ParticleConstant particleConstant;

	WorldBillboard worldBillboard;
	TimeConstants time;
	EmitData emitData;

private:

	Byte4 PADING;


public:

	BasicGPUParticle() = default;
	~BasicGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting()override;

	void ADD(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_);

private:

	void PBufferCreate();

	void PUpdateConstantBuffer(float deltaTime_)override;

	//コピーコンストラクタ・代入演算子削除
	BasicGPUParticle& operator=(const BasicGPUParticle&) = delete;
	BasicGPUParticle(const BasicGPUParticle&) = delete;
};

