#include<ComputePipelineState.h>
#include<RootSignature.h>
#include<AliceAssert.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<wrl.h>
#include<directx/d3d12.h>

ALICE_SUPPRESS_WARNINGS_END

/// <summary>
/// 計算シェーダ用パイプラインステート
/// </summary>
class ComputePipelineState : public IComputePipelineState
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
	void SetShader(const D3D12_SHADER_BYTECODE& shader_)override;

	/// <summary>
	/// ルートシグネチャをセット
	/// </summary>
	/// <param name="rootSignature_">ルートシグネチャ</param>
	void SetRootSignature(IRootSignature* rootSignature_)override;

	/// <summary>
	/// ノードマスクをセット
	/// </summary>
	/// <param name="nodeMask">ノードマスク</param>
	void SetNodeMask(uint32_t nodeMas_)override;

	/// <summary>
	/// キャッシュパイプラインをセット
	/// </summary>
	/// <param name="cachedPSO">キャッシュパイプライン</param>
	void SetCachedPSO(const D3D12_CACHED_PIPELINE_STATE& cachedPSO_)override;

	/// <summary>
	/// フラグをセット
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetFlag(COMPUTE_PIPELINE_STATE_FLAGS flag_)override;

	/// <summary>
	/// 生成
	/// </summary>
	void Create(IDevice* device_)override;

	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	ID3D12PipelineState* GetPipelineState()const override;

};

void ComputePipelineState::SetShader(const D3D12_SHADER_BYTECODE& shader_)
{
	shaderByte = shader_;
}

void ComputePipelineState::SetRootSignature(IRootSignature* rootSignature_)
{
	rootSignaturePtr = rootSignature_;
}

void ComputePipelineState::SetNodeMask(uint32_t nodeMas_)
{
	pipelineNodeMask = nodeMas_;
}

void ComputePipelineState::SetCachedPSO(const D3D12_CACHED_PIPELINE_STATE& cachedPSO)
{
	cachedPipeline = cachedPSO;
}

void ComputePipelineState::SetFlag(COMPUTE_PIPELINE_STATE_FLAGS flag_)
{
	pipelineFlag = static_cast< D3D12_PIPELINE_STATE_FLAGS > ( flag_ );
}

void ComputePipelineState::Create(IDevice* device_)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC lPipelineDesc{};
	lPipelineDesc.CS = shaderByte;
	lPipelineDesc.pRootSignature = rootSignaturePtr->GetRootSignature();
	lPipelineDesc.NodeMask = pipelineNodeMask;
	lPipelineDesc.CachedPSO = cachedPipeline;
	lPipelineDesc.Flags = pipelineFlag;

	HRESULT lResult = device_->Get()->CreateComputePipelineState(&lPipelineDesc,IID_PPV_ARGS(pipelineState.GetAddressOf()));

	if (FAILED(lResult))
	{

		AliceAssert(0,"コンピュートパイプラインの生成に失敗しました");
	}
}

ID3D12PipelineState* ComputePipelineState::GetPipelineState() const
{
	return pipelineState.Get();
}

std::unique_ptr<IComputePipelineState> CreateUniqueComputePipelineState()
{
	std::unique_ptr<IComputePipelineState> lComputePipelineState = std::make_unique<ComputePipelineState>();
	return std::move(lComputePipelineState);
}

std::shared_ptr<IComputePipelineState> CreateSharedComputePipelineState()
{
	std::shared_ptr<IComputePipelineState> lComputePipelineState = std::make_shared<ComputePipelineState>();
	return std::move(lComputePipelineState);
}
