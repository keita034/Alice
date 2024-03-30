#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>

struct FireGPUParticleSetting
{
	//収束点(基準点からの相対)
	AliceMathF::Vector3 convergePointPosition;
	//半径
	float radius;
	//基準点
	AliceMathF::Vector3 position;
	//パーティクルが生きてる時間
	float lifeTime;
	//スタート時のカラー
	AliceMathF::Vector4 startColor;
	//終了時のカラー
	AliceMathF::Vector4 endColor;
	//このエミッタからでる最大数
	float maxParticles;
	//このエミッタから1回でるパーティクル数
	uint32_t emitCount;
	//パーティクルが発生する時間
	float timeBetweenEmit;
	//このエミッタが存在する時間
	float emitLifeTime;
	float size;
	float speed;
	bool isPlay = true;
private:
	Byte3 PADING;
};



class FireGPUParticle : public BaseGPUParticle
{

public://GPUで使う構造体

	struct ParticleConstantGPUData
	{
		AliceMathF::Vector3 convergePointPosition;
		float radius;

		AliceMathF::Vector3 position;
		float maxParticles;

		AliceMathF::Vector4 startColor;

		AliceMathF::Vector4 endColor;

		float lifeTime;
		float size;
		float speed;
		uint32_t emitCount;
	};

	struct FireGPUParticleGPUData
	{
		uint32_t maxParticles = 0;
		uint32_t emitDataIndex =0;

	};

	struct ParticleConstantGPUDatas
	{
		ParticleConstantGPUData particleConstantGPUDatas;
	};

public://内部で使う構造体

	struct ParticleEmit
	{
		AliceMathF::Vector3 position{};
		AliceMathF::Vector3 convergePointPosition{};
		float radius;
		float lifeTime;
		AliceMathF::Vector4 startColor{};
		AliceMathF::Vector4 endColor{};
		float maxParticles;
		uint32_t index;
		float emitTimeCounter;
		float timeBetweenEmit;
		float emitLifeTime;
		float emitMaxLifeTime;
		uint32_t emitCount;
		float size;
		float speed;
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

	ParticleEmit emitData;
	ParticleConstantGPUData particleConstant;

	size_t emitDataCount;

	TextureData* texture;


public:

	FireGPUParticle() = default;
	~FireGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting()override;

	void Create(uint32_t maxParticles_);
	void Emit(const FireGPUParticleSetting& setting_);
	void Move(const AliceMathF::Vector3& move_);
	void SetPos(const AliceMathF::Vector3& pos_ );
	void SetTex(uint32_t textureHandle_);
	void EmitPlay();
	void EmitStop();

	IFreeListBuffer* GetFreeListBuffer();

private:

	void PBufferCreate();

	void PUpdateConstantBuffer(float deltaTime_)override;

	bool PCanEmit(ParticleEmit& data_,float deltaTime_);

	//コピーコンストラクタ・代入演算子削除
	FireGPUParticle& operator=(const FireGPUParticle&) = delete;
	FireGPUParticle(const FireGPUParticle&) = delete;
};

