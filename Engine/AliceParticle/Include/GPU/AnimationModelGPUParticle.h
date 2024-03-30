#pragma once
#include<BaseGPUParticle.h>
#include<Color.h>

#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<TextureManager.h>
#include<MeshGPUParticleAliceModel.h>


struct AnimationModelGPUParticleSetting
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



class AnimationModelGPUParticle : public BaseGPUParticle
{
public://GPUで使う構造体

	struct ParticleConstantGPUData
	{
		AliceMathF::Matrix4 matWorld;

		AliceMathF::Vector4 color;

		AliceMathF::Vector4 setColor;

		float deltaTime;
		float totalTime;
		uint32_t computeTime;
		float size;

		uint32_t vertexSize;
		uint32_t meshIndex =0;
		AliceMathF::Vector2 PAD;

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

	struct BoneMeshData
	{
		uint32_t sVisibles;
		AliceMathF::Vector3 PAD;
	};

	struct BoneMeshDatas
	{
		std::array<BoneMeshData,128>boneMeshisVisibles;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	std::unique_ptr<IConstantBuffer>particleConstantsBuffer;
	std::unique_ptr<IConstantBuffer>boneMeshDatasBuffer;
	std::unique_ptr<IConstantBuffer>worldBillboardBuffer;
	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	std::unique_ptr <MeshGPUParticleAliceModel> modelData;
	std::unordered_map <std::string, std::unordered_map<std::string,uint32_t>>boneMeshIsVisibles;

	TextureData* texture;

	WorldBillboardGPUData worldBillboardGPUData;
	ParticleConstantGPUData particleConstants;
	ParticleEmit emitDatas;
	BoneMeshDatas boneMeshDatas;

	uint32_t particleCount =0;

	bool isEmit = false;
	bool animationStop = false;
	Byte6 PADING;
public:

	AnimationModelGPUParticle() = default;
	~AnimationModelGPUParticle() = default;

	void Initialize() override;
	void Update(float deltaTime_) override;
	void Finalize() override;
	void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) override;
	void SetSetting()override;

	void Create(uint32_t maxParticles_);
	void Emit(const AnimationModelGPUParticleSetting& setting_);
	void SetMat(const AliceMathF::Matrix4& matWorld_);
	void SetTex(uint32_t textureHandle_);
	void EmitPlay();
	void EmitStop();
	void SetModel(AliceModel* model_);
	float GetDeltaTime() const;
	void DrawListRelease();
	void InvisibleBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	void VisibleBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	void ChangeColor(const AliceMathF::Vector4& color_,const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	void ReturnColor(const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	void StopAnimation();
	void PlayAnimation();
private:

	void PBufferCreate();

	void PUpdateConstantBuffer(float deltaTime_)override;

	bool PCanEmit(ParticleEmit& data_,float deltaTime_);

	//コピーコンストラクタ・代入演算子削除
	AnimationModelGPUParticle& operator=(const AnimationModelGPUParticle&) = delete;
	AnimationModelGPUParticle(const AnimationModelGPUParticle&) = delete;
};
