#pragma once
#include"ErrorException.h"
#include"AliceMathUtility.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"ComputePipelineState.h"
#include"RootSignature.h"
#include"Shader.h"
#include"Structure.h"

struct TextureData
{
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	//デスクプリタヒープ
	ID3D12DescriptorHeap* srvHeap;

	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	//横幅
	size_t width = 0;
	//縦幅
	size_t height = 0;

	//カラー
	AliceMathF::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	std::string path;

	uint32_t textureHandle;

	~TextureData();

private:
	char PADING[4]{};
};


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
	std::unique_ptr<VertexBuffer> vertexBuffer;
	//頂点マップ
	PosColor* vertMap;
	//インデックスバッファ
	std::unique_ptr<IndexBuffer> indexBuffer;
	//インデックスマップ
	uint32_t* indexMap;

	Buff() = default;
	~Buff() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	Buff& operator=(const Buff&) = delete;
	Buff(const Buff&) = delete;

};

//ブレンドモード
enum class BlendMode
{
	AX_BLENDMODE_NOBLEND,//ノーブレンド（デフォルト）
	AX_BLENDMODE_ALPHA,//αブレンド
	AX_BLENDMODE_ADD,//加算ブレンド
	AX_BLENDMODE_SUB,//減算ブレンド
	AX_BLENDMODE_MULA,//乗算ブレンド
	AX_BLENDMODE_INVSRC,//反転ブレンド

	AX_BLENDMODE_MAX,//ブレンド種類数

	AX_BLENDMODE_CUSTOM = -1,
};

//コンピュートシェーダー関連(クラス共通)
struct ComputeRelation
{
	ComputeRelation();
	~ComputeRelation();

	//パイプラインステート
	std::shared_ptr<ComputePipelineState> computePipelineState;
	//ルートシグネチャ
	std::unique_ptr<RootSignature> rootSignature;

	std::unique_ptr<RootSignature> rootSignature1;
	//シェーダー
	std::unique_ptr<Shader> computeShader;

private:
	//コピーコンストラクタ・代入演算子削除
	ComputeRelation& operator=(const ComputeRelation&) = delete;
	ComputeRelation(const ComputeRelation&) = delete;
};