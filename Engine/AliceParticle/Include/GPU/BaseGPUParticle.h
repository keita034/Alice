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
#include<MaterialManager.h>
#include<PadingType.h>
#include<CrossAdapterBuffer.h>
#include<FreeListBuffer.h>
#include<DrawListBuffer.h>

ALICE_SUPPRESS_WARNINGS_BEGIN
#include<string>
#include<unordered_map>
ALICE_SUPPRESS_WARNINGS_END

class BaseGPUParticle
{

public:
	struct ParticleGPUData
	{
		AliceMathF::Vector4 color;

		AliceMathF::Vector3 position;
		float age;

		AliceMathF::Vector3 velocity;
		float alive;

		AliceMathF::Vector2 size;
		float threshold;
		float lifeTime;

		uint32_t index;
		AliceMathF::Vector3 accel;
	};
protected:

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

	static ISwapChain* swapChain;

	static IAdapter* graphicAdapter;
	static IAdapter* computeAdapter;

	std::unique_ptr<IDrawListBuffer>drawListBuffer;
	std::unique_ptr<IFreeListBuffer>freeListBuffer;
	std::unique_ptr<ICrossAdapterBuffer>particlePoolBuffer;

	size_t maxParticles;

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

	static void BaseGPUParticleFinalize();

	virtual void Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_) = 0;

	virtual void SetSetting() =0;

	virtual ICrossAdapterBuffer* GetParticlePoolBuffer();

	virtual IFreeListBuffer* GetFreeListBuffer();

	virtual IDrawListBuffer* GetDrawListBuffer();

	size_t GetMaxParticles();

	/// <summary>
	/// アダプターを設定
	/// </summary>
	static void SetAdapter(IAdapter* graphicAdapter_,IAdapter* computeAdapter_);

	static void SetSwapChain(ISwapChain* swapChain_);

	static void ParticleBegin();

	static void ParticleEnd();

	BaseGPUParticle() = default;
	virtual ~BaseGPUParticle() = default;

private:

	virtual void PUpdateConstantBuffer(float deltaTime_) = 0;

	//コピーコンストラクタ・代入演算子削除
	BaseGPUParticle& operator=(const BaseGPUParticle&) = delete;
	BaseGPUParticle(const BaseGPUParticle&) = delete;
};

