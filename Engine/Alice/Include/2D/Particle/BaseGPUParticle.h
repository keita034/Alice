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
protected:

	static ISwapChain* swapChain;

	static IAdapter* graphicAdapter;
	static IAdapter* computeAdapter;

	std::unique_ptr<IDrawListBuffer>drawListBuffer;
	std::unique_ptr<IFreeListBuffer>freeListBuffer;

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

	virtual void Draw(const AliceMathF::Matrix4& viewMat_,const AliceMathF::Matrix4& projectionMat_) = 0;

	virtual void SetSetting() =0;

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

