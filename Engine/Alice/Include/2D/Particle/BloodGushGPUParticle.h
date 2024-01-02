#pragma once
#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>

struct BloodGushGPUParticleSetting
{
	//加速度
	AliceMathF::Vector3 accel;
	float lifeTime;

	AliceMathF::Vector3 blurWidth;
	uint32_t index;

	AliceMathF::Vector4 startColor;

	AliceMathF::Vector4 endColor;

	AliceMathF::Vector2 size;
	float speed;
	float amount;

	float emitLifeTime;
	float emitMaxLifeTime;
	float timeBetweenEmit;
	bool isPlay = true;
private:
	Byte3 PADING;
};



class BloodGushGPUParticle : public BaseGPUParticle
{
private:
	static constexpr size_t EMIT_DATA_MAX_COUNT = 100;
	static constexpr size_t EMIT_COUNT = 20;

public://GPUで使う構造体

	struct ParticleConstantGPUData
	{
		AliceMathF::Vector3 position;
		float lifeTime;

		AliceMathF::Vector3 velocity;
		float maxParticles;

		AliceMathF::Vector3 accel;
		float speed;

		AliceMathF::Vector4 startColor;

		AliceMathF::Vector4 endColor;

		uint32_t index;
		uint32_t emitCount;
		float emitLifeTime;
		float amount;

		AliceMathF::Vector2 size;
		float pad;
		float pad2;
	};

	struct FireGPUParticleGPUData
	{
		uint32_t maxParticles = 0;
		uint32_t emitDataIndex = 0;

	};

	struct ParticleConstantGPUDatas
	{
		std::array<ParticleConstantGPUData,EMIT_DATA_MAX_COUNT>particleConstantGPUDatas;
	};

public://内部で使う構造体

	struct ParticleEmit
	{
		AliceMathF::Vector3 position;
		float lifeTime;

		AliceMathF::Vector3 velocity;
		float maxParticles;

		AliceMathF::Vector3 accel;
		uint32_t index;

		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;

		AliceMathF::Vector2 size;
		uint32_t emitCount;
		float speed;
		float amount;

		float emitTimeCounter;
		float timeBetweenEmit;
		float emitLifeTime;
		float emitMaxLifeTime;

		bool isPlay;
	private:
		Byte3 PADING;
	};

private:

	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	std::unique_ptr<IConstantBuffer>particleConstantsBuffer;
	std::unique_ptr<IConstantBuffer>worldBillboardBuffer;
	std::unique_ptr<IConstantBuffer>timeConstantsBuffer;
	std::unique_ptr<IConstantBuffer>fireGPUParticleDataBuffer;

	WorldBillboardGPUData worldBillboardGPUData;
	TimeConstantGPUData timeGPUData;
	Byte4 PADING;
	FireGPUParticleGPUData fireGPUParticleGPUData;

	std::vector<ParticleEmit>emitDatas;
	std::vector<ParticleConstantGPUData>particleConstants;

	size_t emitDataCount;
	size_t maxParticles;

	TextureData* texture;


public:

	BloodGushGPUParticle() = default;
	~BloodGushGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting()override;

	void Create(uint32_t maxParticles_);
	int32_t Emit(const BloodGushGPUParticleSetting& setting_,int32_t index_ = -1);
	void SetTex(uint32_t textureHandle_);
	void EmitPlay(const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& velocity_,int32_t index_);
	void EmitStop(int32_t index_);

private:

	void PBufferCreate();

	void PUpdateConstantBuffer(float deltaTime_)override;

	bool PCanEmit(ParticleEmit& data_,float deltaTime_);

	//コピーコンストラクタ・代入演算子削除
	BloodGushGPUParticle& operator=(const BloodGushGPUParticle&) = delete;
	BloodGushGPUParticle(const BloodGushGPUParticle&) = delete;
};


