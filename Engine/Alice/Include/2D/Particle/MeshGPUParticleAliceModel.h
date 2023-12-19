#pragma once

#include<AliceModel.h>
#include<BufferType.h>

class AnimationMeshGPUParticleModelMesh
{
public:

	//名前
	std::string name;

	//頂点データの配列
	std::vector<PosNormUvTangeColSkin>* vertices;
	//インデックスの配列
	std::vector<uint32_t>* indices;

	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;

	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;
	//ボーン
	std::unique_ptr<IConstantBuffer> constBoneBuffer;
	BoneData* bonedata;

public:

	/// <summary>
	/// 頂点座標を取得
	/// </summary>
	/// <returns>頂点座標配列</returns>
	const std::vector<PosNormUvTangeColSkin>& GetVertices();

	D3D12_GPU_DESCRIPTOR_HANDLE GetVertexSRVAddress() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetIndicesSRVAddress() const;

	IConstantBuffer* GetBoneBuffer();



private:

//コピーコンストラクタ・代入演算子削除
	AnimationMeshGPUParticleModelMesh& operator=(const AnimationMeshGPUParticleModelMesh&) = delete;
	AnimationMeshGPUParticleModelMesh(const AnimationMeshGPUParticleModelMesh&) = delete;

public:

	AnimationMeshGPUParticleModelMesh() = default;
	~AnimationMeshGPUParticleModelMesh() = default;
};

class AnimationMeshGPUParticleAliceModelData
{
private:
	//フレンドクラス
	friend class AnimationMeshGPUParticleAliceModel;
	friend class AliceFileStream;

	std::string filePath;

	//モデル名
	std::string name;

	//メッシュ配列
	std::vector<std::unique_ptr<AnimationMeshGPUParticleModelMesh>> meshes;

	//姿勢行列
	IConstantBuffer* postureMatBuff;

public:
	AnimationMeshGPUParticleAliceModelData() = default;
	~AnimationMeshGPUParticleAliceModelData() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	AnimationMeshGPUParticleAliceModelData& operator=(const AnimationMeshGPUParticleAliceModelData&) = delete;
	AnimationMeshGPUParticleAliceModelData(const AnimationMeshGPUParticleAliceModelData&) = delete;
};

class AnimationMeshGPUParticleAliceModel
{
protected:

	static IMultiAdapters* sMultiAdapters;

	static std::unordered_map<std::string,std::unique_ptr<AnimationMeshGPUParticleAliceModelData>> sModelDatas;

	AnimationMeshGPUParticleAliceModelData* modelData = nullptr;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnimationMeshGPUParticleAliceModel() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AnimationMeshGPUParticleAliceModel() = default;

	/// <summary>
	/// メッシュ配列を取得
	/// </summary>
	/// <returns></returns>
	const std::vector<std::unique_ptr<AnimationMeshGPUParticleModelMesh>>& GetMeshs();

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="modelHandle">ハンドル</param>
	void SetModel(AliceModel* model_,BufferType type);

	/// <summary>
	/// 共通初期化
	/// </summary>
	static void SSetMultiAdapters(IMultiAdapters* multiAdapters_);

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

protected:

	void PReadNodeHeirarchy(AnimationMeshGPUParticleModelMesh* mesh_,const AliceMotionData* pAnimation_,const Node* pNode_,const AliceMathF::Matrix4& mxParentTransform_);
	void PReadNodeHeirarchy(AnimationMeshGPUParticleModelMesh* mesh_,AliceBlendTree* blendTree_,const Node* pNode_,const AliceMathF::Matrix4& mxParentTransform_);
	const ReturnMotionNode* PFindNodeAnim(const AliceMotionData* pAnimation_,const std::string& strNodeName_);

	//コピーコンストラクタ・代入演算子削除
	AnimationMeshGPUParticleAliceModel& operator=(const AnimationMeshGPUParticleAliceModel&) = delete;
	AnimationMeshGPUParticleAliceModel(const AnimationMeshGPUParticleAliceModel&) = delete;
};
