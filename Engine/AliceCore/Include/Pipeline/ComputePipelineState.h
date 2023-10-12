#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<memory>

ALICE_SUPPRESS_WARNINGS_END

#include<BasePipelineState.h>

struct ID3D12Device;
struct ID3D12PipelineState;
struct D3D12_SHADER_BYTECODE;
struct D3D12_CACHED_PIPELINE_STATE;
class IRootSignature;

enum COMPUTE_PIPELINE_STATE_FLAGS
{
	COMPUTE_PIPELINE_STATE_FLAG_NONE = 0,
	COMPUTE_PIPELINE_STATE_FLAG_TOOL_DEBUG = 0x1,
	COMPUTE_PIPELINE_STATE_FLAG_DYNAMIC_DEPTH_BIAS = 0x4,
	COMPUTE_PIPELINE_STATE_FLAG_DYNAMIC_INDEX_BUFFER_STRIP_CUT = 0x8
};

/// <summary>
/// 計算シェーダ用パイプラインステート
/// </summary>
class IComputePipelineState : public BasePipelineState
{
public:

	IComputePipelineState() = default;
	virtual ~IComputePipelineState() = default;

	/// <summary>
	/// シェーダーをセット
	/// </summary>
	/// <param name="shader">シェーダー</param>
	virtual void SetShader(const D3D12_SHADER_BYTECODE& shader_) = 0;

	/// <summary>
	/// ルートシグネチャをセット
	/// </summary>
	/// <param name="rootSignature_">ルートシグネチャ</param>
	virtual void SetRootSignature(IRootSignature* rootSignature_) = 0;

	/// <summary>
	/// ノードマスクをセット
	/// </summary>
	/// <param name="nodeMask">ノードマスク</param>
	virtual void SetNodeMask(uint32_t nodeMas_) = 0;

	/// <summary>
	/// キャッシュパイプラインをセット
	/// </summary>
	/// <param name="cachedPSO">キャッシュパイプライン</param>
	virtual void SetCachedPSO(const D3D12_CACHED_PIPELINE_STATE& cachedPSO_) = 0;

	/// <summary>
	/// フラグをセット
	/// </summary>
	/// <param name="flag">フラグ</param>
	virtual void SetFlag(COMPUTE_PIPELINE_STATE_FLAGS flag_) = 0;

	/// <summary>
	/// 生成
	/// </summary>
	virtual void Create(IDevice* device_) = 0;

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	virtual ID3D12PipelineState* GetPipelineState()const = 0;

};

/// <summary>
/// 計算シェーダ用パイプラインステートの生成(ユニーク
/// </summary>
std::unique_ptr<IComputePipelineState> CreateUniqueComputePipelineState();

/// <summary>
/// 計算シェーダ用パイプラインステートの生成(シェアード)
/// </summary>
std::shared_ptr<IComputePipelineState> CreateSharedComputePipelineState();