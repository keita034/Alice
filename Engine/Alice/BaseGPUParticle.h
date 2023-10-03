#pragma once
#include<Adapter.h>
#include<RWStructuredBuffer.h>
#include<ComputePipelineState.h>
#include<RootSignature.h>

class BaseGPUParticle
{
private:

	ICommandList* commandList = nullptr;
	IDevice* device = nullptr;
	IComputePipelineState* pipelineState = nullptr;
	IRootSignature* rootSignature = nullptr;

	std::unique_ptr<IRWStructuredBuffer>inputBuffer;
	std::unique_ptr<IRWStructuredBuffer>drawBuffer;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	virtual void Dispatch(uint32_t x_,uint32_t y_,uint32_t z_) = 0;

	/// <summary>
	/// パイプライン、ルートシグネチャを設定
	/// </summary>
	void SetPipelineSignature(IComputePipelineState* pipelineState_,IRootSignature* rootSignature_);

	/// <summary>
	/// アダプターを設定
	/// </summary>
	void SetAdapter(IAdapter* adapter);

	BaseGPUParticle() = default;
	~BaseGPUParticle() = default;

};

