#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>

struct LaserGPUParticleSetting
{
	//収束点(基準点からの相対)
	AliceMathF::Vector3 position;
	//パーティクルが生きてる時間
	float lifeTime;
	AliceMathF::Vector3 velocity;
	//このエミッタからでる最大数
	float maxParticles;
	//スタート時のカラー
	AliceMathF::Vector4 startColor;
	//終了時のカラー
	AliceMathF::Vector4 endColor;
	//パーティクルが発生する時間
	float timeBetweenEmit;
	float maxSize;
	float maxSizeTime;
	float speed;
	bool isPlay = true;
private:
	Byte3 PADING;
};

class LaserGPUParticle : public BaseGPUParticle
{
private:
	static constexpr size_t EMIT_DATA_MAX_COUNT = 100;

public://GPUで使う構造体

	struct ParticleConstantGPUData
	{
		AliceMathF::Vector3 velocity;
		float lifeTime;

		AliceMathF::Vector3 position;
		float maxParticles;

		AliceMathF::Vector4 startColor;

		AliceMathF::Vector4 endColor;

		float speed;
		float maxSize;
		float maxSizeTime;
		float pad;
	};

	struct WorldBillboardGPUData
	{
		AliceMathF::Matrix4 worldMat;
		AliceMathF::Matrix4 billboardMat;
	};

	struct TimeConstantGPUData
	{
		float deltaTime = 0.0f;
		float totalTime = 0.0f;
		uint32_t computeTime;
	};

	struct FireGPUParticleGPUData
	{
		uint32_t maxParticles = 0;
		uint32_t emitDataIndex = 0;
	};

	struct ParticleGPUData
	{
		AliceMathF::Vector4 color;
		AliceMathF::Vector3 position;
		float age;
		AliceMathF::Vector3 velocity;
		float size;
		float alive;
		float threshold;
		float lifeTime;
		uint32_t index;
	};

	struct ParticleConstantGPUDatas
	{
		std::array<ParticleConstantGPUData,EMIT_DATA_MAX_COUNT>particleConstantGPUDatas;
	};

public://内部で使う構造体

	struct ParticleEmit
	{
		AliceMathF::Vector3 position;
		float emitTimeCounter;
		AliceMathF::Vector3 velocity;
		float lifeTime;
		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;
		float maxParticles;
		uint32_t index;

		float timeBetweenEmit;
		float maxSize;
		float maxSizeTime;
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

	std::unique_ptr<ICrossAdapterBuffer>particlePoolBuffer;

	WorldBillboardGPUData worldBillboardGPUData;
	TimeConstantGPUData timeGPUData;
	Byte4 PADING;
	FireGPUParticleGPUData fireGPUParticleGPUData;

	std::vector<ParticleEmit>emitDatas;
	std::vector<ParticleConstantGPUData>particleConstants;

	size_t emitDataCount;
	size_t maxParticles;

	TextureData* mainTexture;
	TextureData* subTexture;

public:

	LaserGPUParticle() = default;
	~LaserGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting()override;

	void Create(uint32_t maxParticles_);
	int32_t Emit(const LaserGPUParticleSetting& setting_,int32_t index_ = -1);
	void Move(const AliceMathF::Vector3& move_,int32_t index_);
	void SetPos(const AliceMathF::Vector3& pos_,int32_t index_);
	void SetVelocity(const AliceMathF::Vector3& velocity_,int32_t index_);
	void EmitPlay(int32_t index_);
	void EmitStop(int32_t index_);
	float GetDeltaTime();
	void SetMainTex(uint32_t textureHandle_);
	void SetSubTex(uint32_t textureHandle_);

private:

	void PBufferCreate();

	void PUpdateConstantBuffer(float deltaTime_)override;

	bool PCanEmit(ParticleEmit& data_,float deltaTime_);

	//コピーコンストラクタ・代入演算子削除
	LaserGPUParticle& operator=(const LaserGPUParticle&) = delete;
	LaserGPUParticle(const LaserGPUParticle&) = delete;
};

