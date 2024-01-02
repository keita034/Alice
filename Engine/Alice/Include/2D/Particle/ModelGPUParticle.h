#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>
#include<MeshGPUParticleAliceModel.h>


struct ModelGPUParticleSetting
{
	AliceMathF::Matrix4 matWorld;

	AliceMathF::Vector3 velocity;
	//スタート時のカラー
	AliceMathF::Vector4 startColor;
	//終了時のカラー
	AliceMathF::Vector4 endColor;
	//パーティクルが生きてる時間
	float lifeTime;
	//このエミッタからでる最大数
	float maxParticles;
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



class ModelGPUParticle : public BaseGPUParticle
{
public://GPUで使う構造体

	struct ParticleConstantGPUData
	{
		AliceMathF::Matrix4 matWorld;

		AliceMathF::Vector3 velocity;
		float maxParticles;

		AliceMathF::Vector4 startColor;

		AliceMathF::Vector4 endColor;

		float lifeTime;
		float size;
		float speed;
		uint32_t vertexSize;
	};

	struct FireGPUParticleGPUData
	{
		uint32_t maxParticles = 0;
		uint32_t emitDataIndex = 0;
	};

	struct ParticleConstantGPUDatas
	{
		ParticleConstantGPUData particleConstantGPUData;
	};

public://内部で使う構造体

	struct ParticleEmit
	{
		AliceMathF::Matrix4 matWorld;
		AliceMathF::Vector3 velocity;
		float maxParticles;
		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;
		float lifeTime;
		float size;
		float speed;
		uint32_t index;
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
	std::unique_ptr<IConstantBuffer>animationConstantsBuffer;
	std::unique_ptr<IConstantBuffer>worldBillboardBuffer;
	std::unique_ptr<IConstantBuffer>timeConstantsBuffer;
	std::unique_ptr<IConstantBuffer>gpuParticleDataBuffer;

	std::unique_ptr <MeshGPUParticleAliceModel> modelData;

	TextureData* texture;

	std::vector<std::unique_ptr<IDrawListBuffer>>drawListBuffers;

	WorldBillboardGPUData worldBillboardGPUData;
	TimeConstantGPUData timeGPUData;
	ParticleConstantGPUData particleConstants;
	FireGPUParticleGPUData fireGPUParticleGPUData;
	ParticleEmit emitDatas;

	size_t emitDataCount;
	size_t maxParticles;

	bool isEmit = false;

	Byte7 PADING;
public:

	ModelGPUParticle() = default;
	~ModelGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting()override;

	void Create(uint32_t maxParticles_);
	void Emit(const ModelGPUParticleSetting& setting_);
	void SetMat(const AliceMathF::Matrix4& matWorld_);
	void SetTex(uint32_t textureHandle_);
	void EmitPlay();
	void EmitStop();
	void SetModel(AliceModel* model_);
	float GetDeltaTime() const;
	void DrawListRelease();

private:

	void PBufferCreate();

	void PUpdateConstantBuffer(float deltaTime_)override;

	bool PCanEmit(ParticleEmit& data_,float deltaTime_);

	//コピーコンストラクタ・代入演算子削除
	ModelGPUParticle& operator=(const ModelGPUParticle&) = delete;
	ModelGPUParticle(const ModelGPUParticle&) = delete;
};
