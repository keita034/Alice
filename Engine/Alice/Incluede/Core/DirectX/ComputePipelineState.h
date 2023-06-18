#pragma once
#include<RootSignature.h>

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)

#include<wrl.h>

#pragma warning(pop)

/// <summary>
/// 計算シェーダ用パイプラインステート
/// </summary>
class ComputePipelineState
{
private:
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	
	//シェーダーオブジェクト
	D3D12_SHADER_BYTECODE shaderByte;
	
	//ルートシグネチャ
	IRootSignature* rootSignaturePtr;

	//キャッシュパイプライン
	D3D12_CACHED_PIPELINE_STATE cachedPipeline;
	
	//フラグ
	D3D12_PIPELINE_STATE_FLAGS  pipelineFlag = D3D12_PIPELINE_STATE_FLAG_NONE;

	//ノードマスク
	uint32_t pipelineNodeMask = 0;

public:

	/// <summary>
	/// シェーダーをセット
	/// </summary>
	/// <param name="shader">シェーダー</param>
	void SetShader(const D3D12_SHADER_BYTECODE& shader_);

	/// <summary>
	/// ルートシグネチャをセット
	/// </summary>
	/// <param name="rootSignature_">ルートシグネチャ</param>
	void SetRootSignature(IRootSignature* rootSignature_);

	/// <summary>
	/// ノードマスクをセット
	/// </summary>
	/// <param name="nodeMask">ノードマスク</param>
	void SetNodeMask(uint32_t nodeMas_);

	/// <summary>
	/// キャッシュパイプラインをセット
	/// </summary>
	/// <param name="cachedPSO">キャッシュパイプライン</param>
	void SetCachedPSO(const D3D12_CACHED_PIPELINE_STATE& cachedPSO_);

	/// <summary>
	/// フラグをセット
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetFlag(D3D12_PIPELINE_STATE_FLAGS flag_);

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="device">デバイス</param>
	void Create(ID3D12Device* device_);

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	ID3D12PipelineState* GetPipelineState()const;

};

