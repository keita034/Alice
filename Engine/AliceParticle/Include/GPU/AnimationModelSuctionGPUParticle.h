#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>
#include<MeshGPUParticleAliceModel.h>


struct AnimationModelSuctionGPUParticleSetting
{
	AliceMathF::Matrix4 matWorld;

	AliceMathF::Vector4 startColor;

	AliceMathF::Vector4 endColor;

	AliceMathF::Vector3 centerPos;
	float distance;

	float lifeTime;
	float invisibleTime = 0.0f;

	float moveTime;
	float size;
	float timeBetweenEmit;
	float emitLifeTime;
	bool isPlay = true;
	bool isInfinityEmit = false;
private:
	Byte2 PADING;
};



class AnimationModelSuctionGPUParticle : public BaseGPUParticle
{
public://GPUで使う構造体

	struct ModelParticleGPUData
	{
		float memory[ 24 ];
	};

	struct ParticleConstantGPUData
	{
		AliceMathF::Matrix4 matWorld;

		AliceMathF::Vector4 startColor;

		AliceMathF::Vector4 endColor;

		AliceMathF::Vector3 centerPos;
		float distance;

		uint32_t computeTime;
		float deltaTime = 0.0f;
		float lifeTime;
		float invisibleTime = 0.0f;

		float moveTime;
		float size;
		uint32_t vertexSize;
		uint32_t maxParticles;

	};

public://内部で使う構造体

	struct ParticleEmit
	{
		AliceMathF::Matrix4 matWorld;
		AliceMathF::Vector3 velocity;
		uint32_t maxParticles;
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
	std::unique_ptr<IConstantBuffer>gpuParticleDataBuffer;
	std::unique_ptr <MeshGPUParticleAliceModel> modelData;

	WorldBillboardGPUData worldBillboardGPUData;

	ParticleEmit emitData;
	ParticleConstantGPUData particleConstant;

	float emitTime;

	size_t emitDataCount;
	size_t verticeSize;

	TextureData* texture;
	BoneMesh* boneMesh;

	bool boneMeshRoot = false;
	bool draw = true;
	bool init = false;
	Byte5 PADING;

public:

	AnimationModelSuctionGPUParticle() = default;
	~AnimationModelSuctionGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void Create(uint32_t maxParticles_);
	void Emit(const AnimationModelSuctionGPUParticleSetting& setting_);
	void EmitStop();
	void EmitPlay(bool flag_ = true);

	float GetDeltaTime() const;
	size_t GetVerticeSize()const;

	void SetSetting()override;
	void SetMat(const AliceMathF::Matrix4& matWorld_);
	void SetTex(uint32_t textureHandle_);
	void SetModel(AliceModel* model_);
	void SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	void SetCenterPos(const AliceMathF::Vector3& pos_);
	void DrawStop();
	void DrawStart();

	const Bone* GetBone(const std::string& meshName_,const std::string& boneName_)const;
	AliceMathF::Vector3 GetMeshBoneCenterPos(const std::string& meshName_,const std::string& boneName_)const;
private:

	void PBufferCreate();
	void PUpdateConstantBuffer(float deltaTime_)override;
	bool PCanEmit(ParticleEmit& data_,float deltaTime_);
	void PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_);

	//コピーコンストラクタ・代入演算子削除
	AnimationModelSuctionGPUParticle& operator=(const AnimationModelSuctionGPUParticle&) = delete;
	AnimationModelSuctionGPUParticle(const AnimationModelSuctionGPUParticle&) = delete;
};

