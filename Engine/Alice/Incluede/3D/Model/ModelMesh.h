﻿#pragma once
#include<AliceUtility.h>
#include<VertexBuffer.h>
#include<IndexBuffer.h>
#include<DefaultMaterial.h>
#include<ConstantBuffer.h>
#include<RootSignature.h>
#include<ComputePipelineState.h>
#include<RWStructuredBuffer.h>
#include<StructuredBuffer.h>
#include<Shader.h>
#include<ComputeVertexBuffer.h>
#include<Transform.h>
#include<Light.h>

class ModelMesh
{
public:

	//名前
	std::string name;

	//名前
	std::string nodeName;

	//頂点データの配列
	std::vector<PosNormUvTangeColSkin> vertices;

	//インデックスの配列
	std::vector<uint32_t> indices;

	//テクスチャ
	std::vector <TextureData*> textures;

	//テクスチャ
	std::vector <TextureData*> texturesNormal;

	std::unordered_map<std::string, Bone*> bones;

	std::vector<Bone> vecBones;

	std::vector< AliceMathF::Matrix4> deformationMat;

	// マテリアル
	ModelMaterial material;

	Node* node = nullptr;

	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;
	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;
	//マテリアルバッファ
	std::unique_ptr<IConstantBuffer> materialBuffer;
	//ボーン
	std::unique_ptr<IConstantBuffer> constBoneBuffer;

	BoneData bonedata;

	bool dirtyFlag;
	//有効
	bool enable = false;
	int16_t PADING;

	// カラー
	std::array<float, 4> col = { {1.0f,1.0f,1.0f,1.0f} };
	int32_t PADING2;

	ModelMesh() = default;
	~ModelMesh();
public:

	/// <summary>
	/// 各種バッファを生成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, const Light* light_);
	
	/// <summary>
	/// アニメーション描画
	/// </summary>
	void AnimDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, const Light* light_);

	/// <summary>
	/// 描画
	/// </summary>
	void ToonDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, D3D12_GPU_DESCRIPTOR_HANDLE rampHandle_, const Light* light_);

	void OutLineDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_);

	/// <summary>
	/// アニメーション描画
	/// </summary>
	void AnimToonDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_, D3D12_GPU_DESCRIPTOR_HANDLE rampHandle_, const Light* light_);

	void AnimOutLineDraw(ID3D12GraphicsCommandList* cmdList_, const Transform& transform_);

	void Update();

	void InitializeVertex();

	/// <summary>
	/// 頂点座標を取得
	/// </summary>
	/// <returns>頂点座標配列</returns>
	const std::vector<PosNormUvTangeColSkin>& GetVertices();

	/// <summary>
	/// インデックスを取得
	/// </summary>
	/// <returns>インデックス座標配列</returns>
	const std::vector<uint32_t>& GetIndices();

private:

	//コピーコンストラクタ・代入演算子削除
	ModelMesh& operator=(const ModelMesh&) = delete;
	ModelMesh(const ModelMesh&) = delete;
};