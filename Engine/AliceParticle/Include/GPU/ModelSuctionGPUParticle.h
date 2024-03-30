#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>
#include<MeshGPUParticleAliceModel.h>


struct ModelSuctionGPUParticleSetting
{
	AliceMathF::Matrix4 matWorld;

	AliceMathF::Vector4 startColor;
	AliceMathF::Vector4 endColor;

	float lifeTime;
	float timeBetweenEmit;
	float emitLifeTime;
	float size;

	uint32_t maxParticles;
	uint32_t emitSize;

	bool isPlay = true;
	bool isInfinityEmit = false;
private:
	Byte2 PADING;
};

class ModelSuctionGPUParticle : public BaseGPUParticle
{
public://GPUで使う構造体

	struct ParticleConstantGPUData
	{
		AliceMathF::Matrix4 matWorld;

		AliceMathF::Vector4 startColor;

		AliceMathF::Vector4 endColor;

		AliceMathF::Vector3 pos;
		float pad;

		float deltaTime;
		float lifeTime;
		float size;
		float speed;

		uint32_t vertexSize;
		uint32_t computeTime;
		uint32_t maxParticles;
		uint32_t emitSize;
	};

public://内部で使う構造体

	struct ParticleEmit
	{
		AliceMathF::Matrix4 matWorld;
		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;

		float lifeTime;
		float size;
		float emitTimeCounter;
		float timeBetweenEmit;
		float emitLifeTime;
		float emitMaxLifeTime;

		uint32_t maxParticles;
		uint32_t emitSize;
		uint32_t index;

		bool isPlay;
		bool isInfinityEmit;
	private:
		Byte6 PADING;
	};

private:

	TextureData* texture;
	TextureData* determineTexture;
	BoneMesh* boneMesh;

	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	std::unique_ptr<IConstantBuffer>particleConstantsBuffer;
	std::unique_ptr<IConstantBuffer>worldBillboardBuffer;
	std::unique_ptr <MeshGPUParticleAliceModel> modelData;

	WorldBillboardGPUData worldBillboardGPUData;

	ParticleEmit emitData;
	ParticleConstantGPUData particleConstant;

	bool boneMeshRoot = false;
	bool flag = false;
	Byte2 PADING;

public:

	ModelSuctionGPUParticle() = default;
	~ModelSuctionGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void Create(uint32_t maxParticles_);
	void Emit(const ModelSuctionGPUParticleSetting& setting_);
	void EmitStop();
	void EmitPlay(bool flag_ = true);

	float GetDeltaTime() const;
	void SetSetting()override {};

	void SetMat(const AliceMathF::Matrix4& matWorld_);
	void SetDetermineTex(uint32_t textureHandle_);
	void SetTex(uint32_t textureHandle_);
	void SetModel(AliceModel* model_);
	void SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_ = true);



private:

	void PBufferCreate();
	void PUpdateConstantBuffer(float deltaTime_)override;
	bool PCanEmit(ParticleEmit& data_,float deltaTime_);
	void PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_);

	//コピーコンストラクタ・代入演算子削除
	ModelSuctionGPUParticle& operator=(const ModelSuctionGPUParticle&) = delete;
	ModelSuctionGPUParticle(const ModelSuctionGPUParticle&) = delete;
};

