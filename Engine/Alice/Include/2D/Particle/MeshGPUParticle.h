#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>
#include<MeshGPUParticleAliceModel.h>


struct MeshGPUParticleSetting
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
	bool isInfinityEmit = false;
private:
	Byte2 PADING;
};



class MeshGPUParticle : public BaseGPUParticle
{
private:
	static constexpr size_t EMIT_DATA_MAX_COUNT = 100;

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
		uint32_t emitDataIndex =0;

	};

	struct ParticleConstantGPUDatas
	{
		std::array<ParticleConstantGPUData,EMIT_DATA_MAX_COUNT>particleConstantGPUDatas;
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
		bool isInfinityEmit;
	private:
		Byte2 PADING;
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

	WorldBillboardGPUData worldBillboardGPUData;
	TimeConstantGPUData timeGPUData;
	bool boneMeshRoot = false;
	Byte3 PADING;
	FireGPUParticleGPUData fireGPUParticleGPUData;

	std::vector<ParticleEmit>emitDatas;
	std::vector<ParticleConstantGPUData>particleConstants;

	size_t emitDataCount;

	TextureData* texture;
	TextureData* determineTexture;
	BoneMesh* boneMesh;

public:

	MeshGPUParticle() = default;
	~MeshGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void Create(uint32_t maxParticles_);
	int32_t Emit(const MeshGPUParticleSetting& setting_,int32_t index_ = -1);
	void EmitStop(int32_t index_);
	void EmitPlay(int32_t index_,bool flag_ = true);

	float GetDeltaTime() const;
	void SetSetting()override;

	void SetMat(const AliceMathF::Matrix4& matWorld_ ,int32_t index_);
	void SetDetermineTex(uint32_t textureHandle_);
	void SetTex(uint32_t textureHandle_);
	void SetModel(AliceModel* model_);
	void SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	


private:

	void PBufferCreate();
	void PUpdateConstantBuffer(float deltaTime_)override;
	bool PCanEmit(ParticleEmit& data_,float deltaTime_);
	void PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,size_t index_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_);

	//コピーコンストラクタ・代入演算子削除
	MeshGPUParticle& operator=(const MeshGPUParticle&) = delete;
	MeshGPUParticle(const MeshGPUParticle&) = delete;
};

