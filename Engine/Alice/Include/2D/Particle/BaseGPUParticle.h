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


	struct ParticleData
	{
		AliceMathF::Vector3 position;
		float time;
		AliceMathF::Vector3 velocity;
		float size;
		float alive;
		AliceUtility::Color color;
		int32_t constantsIndex;
		float padding;
	};

	struct ParticleConstants
	{
		AliceMathF::Vector4 startColor;
		AliceMathF::Vector4 endColor;
		AliceMathF::Vector3 velocity;
		float LifeTime = 0.0f;
		AliceMathF::Vector3 acceleration;
		float pad;
		int32_t emitCount = 0;
		int32_t maxParticles = 0;
		int32_t gridSize = 0;
	};

	struct PipelineRootSignature
	{
		IComputePipelineState* pipelineState = nullptr;
		IRootSignature* rootSignature = nullptr;
	};

protected:

	ICommandList* commandList = nullptr;
	IDevice* device = nullptr;

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	std::unique_ptr<IRWStructuredBuffer>particlePoolBuffer;
	std::unique_ptr<IRWStructuredBuffer>drawListBuffer;
	std::unique_ptr<IRWStructuredBuffer>freeListBuffer;

	std::unique_ptr<IStructuredBuffer>particleConstantsBuffer;
	std::unique_ptr<IConstantBuffer>viewProjectionBuffer;
	std::unique_ptr<IConstantBuffer>timeConstantsBuffer;
	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	std::unordered_map<std::string,std::unique_ptr<PipelineRootSignature>> pipelineSet;

	std::vector<ParticleConstants>particleConstants;

	PipelineState* pipelineState = nullptr;
	IRootSignature* rootSignature = nullptr;

	ViewProjection viewProjection;
	TimeConstants time;

	size_t particleMaxCount;
	size_t particleConstantMaxCount;
	size_t emitCount = 0;
	size_t particleEmitterMaxCount;

	float timeBetweenEmit;
	float emitTimeCounter;

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

	virtual void Draw() = 0;

	/// <summary>
	/// アダプターを設定
	/// </summary>
	void SetAdapter(IAdapter* adapter);

	BaseGPUParticle() = default;
	virtual ~BaseGPUParticle() = default;

private:

	virtual void PUpdateConstantBuffer(float deltaTime_) = 0;

	//コピーコンストラクタ・代入演算子削除
	BaseGPUParticle& operator=(const BaseGPUParticle&) = delete;
	BaseGPUParticle(const BaseGPUParticle&) = delete;
};

