#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<vector>

ALICE_SUPPRESS_WARNINGS_END

#include<Shader.h>
#include<ComputePipelineState.h>
#include<RootSignature.h>
#include<BaseMaterial.h>

class ComputeMaterial : public BaseMaterial
{
public:
	std::string name;

	//パイプラインステート
	std::unique_ptr<IComputePipelineState> pipelineState;

	//ルートシグネチャ
	std::unique_ptr<IRootSignature> rootSignature;

	//コンピュートシェーダ
	std::unique_ptr<IShader> computeShader;

	//キャッシュパイプライン
	D3D12_CACHED_PIPELINE_STATE cachedPipeline;

	//フラグ
	COMPUTE_PIPELINE_STATE_FLAGS  pipelineFlag = COMPUTE_PIPELINE_STATE_FLAGS::COMPUTE_PIPELINE_STATE_FLAG_NONE;

	//ノードマスク
	uint32_t pipelineNodeMask = 0;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IDevice* device_);

	ComputeMaterial() = default;
	~ComputeMaterial();

private:
	//コピーコンストラクタ・代入演算子削除
	ComputeMaterial(const ComputeMaterial&) = delete;
	ComputeMaterial& operator=(const ComputeMaterial&) = delete;
};

