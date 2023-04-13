#pragma once
#include"ErrorException.h"
#include"AliceUtility.h"
#include"AliceMathUtility.h"
#include"Camera.h"
#include"DirectX12Core.h"
#include"Light.h"
#include"Transform.h"
#include"Material.h"
#include"ConstantBuffer.h"
#include"VertexBuffer.h"
#include"ComputeVertexBuffer.h"
#include"IndexBuffer.h"
#include"StructuredBuffer.h"

class BaseCollider;

enum class ModelShape
{
	Cube,//立方体
	Sphere,//球
	Capsule,//カプセル
	Cylinder,//円柱
	Cone,//円錐
};

enum ShaderType
{
	Default,
	Flat,
	Gouraud,
	Lambert,
	Phong,

};

struct ModelData
{
	ModelData() = default;
	~ModelData();

	std::string filePath;

	uint32_t modelHandle;
private:
	char PADING1[4] = {};
public:

	TextureData* textureData;
	//頂点バッファ
	std::unique_ptr<ComputeVertexBuffer> vertexBuffer;
	//インデックスバッファ
	std::unique_ptr<IndexBuffer> indexBuffer;
private:
	char PADING2[4] = {};
public:

	//インデックスの数
	UINT maxIndex = 0u;;
private:
	char PADING3[4] = {};
public:

	//頂点の数
	UINT maxVert = 0u;
	//ワールド行列
	AliceMathF::Matrix4 matWorld;
	//頂点データ
	std::vector<PosNormalUv>vertices;
	//頂点インデックス
	std::vector<uint32_t> indices;
	//頂点法線スムージング用データ
	std::unordered_map<uint32_t, std::vector<uint32_t>>smoothData;
	//マテリアル
	ModelMaterial modelMaterial{};
	//定数バッファマテリアル
	std::unique_ptr<ConstantBuffer> constBuffMaterial;
	//ポリゴン爆散
	std::unique_ptr<ConstantBuffer> constBuffVelocity;

	//コンピュートシェーダ入力
	std::unique_ptr<StructuredBuffer> csInputVer;
	//コンピュートシェーダ変換
	std::unique_ptr<StructuredBuffer> csInputBlendVer;
	//コンピュートシェーダタイム
	std::unique_ptr<ConstantBuffer> csTime;

	Material* modelMaterialData;

private:

	//コピーコンストラクタ・代入演算子削除
	ModelData& operator=(const ModelData&) = delete;
	ModelData(const ModelData&) = delete;
};

class Model
{
protected:

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;

	//ライト
	static Light* light;

	static std::vector<std::string>filePaths;

	static std::unordered_map<std::string, std::unique_ptr<ModelData>> modelDatas;

	static uint32_t modelCount;

	static const uint32_t maxModel = 1024;

	static ComputeRelation computeRelation;

	ModelData* modelData;

	std::vector<ModelData*> blendModels;

public:

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	AliceMathF::Matrix4& GetMatWorld();

	/// <summary>
	/// 頂点座標を取得
	/// </summary>
	/// <returns>頂点座標配列</returns>
	const std::vector<PosNormalUv>GetVertices();

	/// <summary>
	/// インデックスを取得
	/// </summary>
	/// <returns>インデックス座標配列</returns>
	const std::vector<uint32_t>GetIndices();

	/// <summary>
	/// ライトのセット
	/// </summary>
	/// <param name="light">ライト</param>
	static void SetLight(Light* light_);

	void SetModel(uint32_t modelHandle);

	void SetBlendModel(const std::vector<uint32_t>& models);

	void ClearBlendModel();

	Model() = default;
	~Model() = default;

	void Draw(Transform& transform , Material* material = nullptr);

	void BlendShapeUpdate(float& t);

	void EffectDraw(Transform* transform, float velocity, Material* material = nullptr);

	bool RotationUV(float angle);

	void SetTexture(uint32_t handle);

	void FlipUV(bool inverseU = false, bool inverseV = false);

	static uint32_t CreatePrimitiveModel(ModelShape type);

	static uint32_t CreateObjModel(const std::string& filePath, bool smoothing = false);
	
	static void CommonInitialize();

private:

	//コピーコンストラクタ・代入演算子削除
	Model& operator=(const Model&) = delete;
	Model(const Model&) = delete;
};