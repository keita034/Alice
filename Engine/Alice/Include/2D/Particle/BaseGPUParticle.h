#pragma once
#include<Adapter.h>

#include<ComputePipelineState.h>
#include<PipelineState.h>
#include<RootSignature.h>

#include<RWStructuredBuffer.h>
#include<StructuredBuffer.h>
#include<ConstantBuffer.h>
#include<DrawArgumentBuffer.h>

#include<AliceMathUtility.h>
#include<Color.h>
#include<Camera.h>
#include<DefaultMaterial.h>
#include<PadingType.h>
#include<CrossAdapterBuffer.h>

ALICE_SUPPRESS_WARNINGS_BEGIN
#include<string>
#include<unordered_map>
ALICE_SUPPRESS_WARNINGS_END

class BaseGPUParticle
{
protected:

	struct ViewProjection
	{
		AliceMathF::Matrix4 view;
		AliceMathF::Matrix4 projection;
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

	struct ParticleConstant
	{
		AliceMathF::Vector3 startPosition;
		int32_t emitCount;
		AliceMathF::Vector3 velocity;
		float lifeTime;
		AliceMathF::Vector3 accel;
		float pad;
		AliceMathF::Vector2 scale;
		AliceMathF::Vector2 rotation;
		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;
	};

	struct ParticleConstants
	{
		std::array<ParticleConstant,10> constants;
	};

protected:

	ICommandList* commandList = nullptr;
	IDevice* device = nullptr;

	ICommandList* computeCmmandList = nullptr;
	IDevice* computeDevice = nullptr;

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	std::unique_ptr<ICrossAdapterBuffer>particlePoolBuffer;
	std::unique_ptr<IRWStructuredBuffer>drawListBuffer;
	std::unique_ptr<IRWStructuredBuffer>freeListBuffer;

	std::unique_ptr<IConstantBuffer>particleConstantsBuffer;
	std::unique_ptr<IConstantBuffer>viewProjectionBuffer;
	std::unique_ptr<IConstantBuffer>timeConstantsBuffer;
	std::unique_ptr<IConstantBuffer>emitDataBuffer;

	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	ParticleConstants particleConstants;

	ViewProjection viewProjection;
	TimeConstants time;
	EmitData emitData;

	size_t particleMaxCount = 100000;
	size_t particleConstantMaxCount = 10;
	size_t particleEmitterMaxCount = 100;
	int32_t emitCount = 100;

	float timeBetweenEmit;
	float emitTimeCounter;

	Byte4 PADING;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(float deltaTime_) = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	virtual void Draw(Camera* camera_) = 0;

	/// <summary>
	/// アダプターを設定
	/// </summary>
	void SetAdapter(IAdapter* graphicAdapter_,IAdapter* computeAdapter_);

	BaseGPUParticle() = default;
	virtual ~BaseGPUParticle() = default;

private:

	virtual void PUpdateConstantBuffer(float deltaTime_) = 0;

	//コピーコンストラクタ・代入演算子削除
	BaseGPUParticle& operator=(const BaseGPUParticle&) = delete;
	BaseGPUParticle(const BaseGPUParticle&) = delete;
};

