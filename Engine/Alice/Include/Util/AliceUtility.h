#pragma once
#include<AliceMathUtility.h>
#include<Structure.h>
#include<Shader.h>
#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<ComputePipelineState.h>
#include<RootSignature.h>

typedef struct
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//デスクプリタヒープ
	ID3D12DescriptorHeap* srvHeap;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	//スプライト数
	UINT nextIndex;
}ModelShareVaria;

//パイプライン・ルートシグネチャセット
struct PipelineSet
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

//バッファ関連
struct Buff
{
	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;
	//頂点マップ
	PosColor* vertMap;
	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;
	//インデックスマップ
	uint32_t* indexMap;

	Buff() = default;
	~Buff() = default;

	//コピーコンストラクタ・代入演算子削除
	Buff& operator=(const Buff&) = delete;
	Buff(const Buff&) = delete;

};

//コンピュートシェーダー関連(クラス共通)
struct ComputeRelation
{
	ComputeRelation();
	~ComputeRelation();

	//パイプラインステート
	std::shared_ptr<IComputePipelineState> computePipelineState;
	//ルートシグネチャ
	std::unique_ptr<IRootSignature> rootSignature;

	std::unique_ptr<IRootSignature> rootSignature1;
	//シェーダー
	std::unique_ptr<IShader> computeShader;

	//コピーコンストラクタ・代入演算子削除
	ComputeRelation& operator=(const ComputeRelation&) = delete;
	ComputeRelation(const ComputeRelation&) = delete;
};