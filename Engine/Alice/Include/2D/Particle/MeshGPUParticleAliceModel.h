#pragma once

#include<AliceModel.h>
#include<BufferType.h>
#include<CrossAdapterBuffer.h>
#include<DrawArgumentBuffer.h>

struct BoneMesh
{
	Bone* bone = nullptr;
	std::string boneName;
	//頂点データの配列
	std::vector<PosNormUvTangeColSkin> vertices;
	//インデックスの配列
	std::vector<uint32_t> indices;
	//インデックスバッファ
	std::unique_ptr<IIndexBuffer> indexBuffer;
	//頂点バッファ
	std::unique_ptr<IVertexBuffer> vertexBuffer;
	//ドローアギュメントバッファ
	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	std::unique_ptr<ICrossAdapterBuffer>particlePoolBuffer;
	std::vector<BoneMesh*> parent;

	AliceMathF::Vector3 centerPos{0,0,0};

	bool isVisible = true;

	Byte3 PADING;

	BoneMesh() = default;
	~BoneMesh() = default;

private:

//コピーコンストラクタ・代入演算子削除
	BoneMesh& operator=(const BoneMesh&) = delete;
	BoneMesh(const BoneMesh&) = delete;

};

class MeshGPUParticleModelMesh
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

	std::unique_ptr<IConstantBuffer> postureMatBuff;
	AliceMathF::Matrix4* postureMat;

	std::unique_ptr<ICrossAdapterBuffer>particlePoolBuffer;
	std::unique_ptr<IDrawArgumentBuffer>drawArgumentBuffer;

	std::vector<std::unique_ptr<BoneMesh>>boneMeshs;

	bool boneMesh = false;

	Byte7 PADING;
public:

	/// <summary>
	/// 頂点座標を取得
	/// </summary>
	/// <returns>頂点座標配列</returns>
	const std::vector<PosNormUvTangeColSkin>& GetVertices() const;

	D3D12_GPU_DESCRIPTOR_HANDLE GetVertexSRVAddress() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetIndicesSRVAddress() const;

	IConstantBuffer* GetBoneBuffer() const;

	IConstantBuffer* GetPostureMatBuffer() const;

	IDrawArgumentBuffer* GetDrawArgumentBuffer()const;
	ICrossAdapterBuffer* GetCrossAdapterBuffer()const;

	void InvisibleBoneMesh(std::unordered_map<std::string,bool>& boneMeshIsVisibles_,const std::string& boneName_,bool root_);

	void VisibleBoneMesh(std::unordered_map<std::string,bool>& boneMeshIsVisibles_,const std::string& boneName_,bool root_);

	BoneMesh* GetBoneMesh(const std::string& boneName_);

	void CreateBoneMeshIsVisibles(std::unordered_map<std::string,bool>& boneMeshIsVisibles_);

private:

//コピーコンストラクタ・代入演算子削除

	MeshGPUParticleModelMesh& operator=(const MeshGPUParticleModelMesh&) = delete;
	MeshGPUParticleModelMesh(const MeshGPUParticleModelMesh&) = delete;

public:

	MeshGPUParticleModelMesh() = default;
	~MeshGPUParticleModelMesh() = default;
};

class MeshGPUParticleAliceModelData
{
private:
	//フレンドクラス
	friend class MeshGPUParticleAliceModel;
	friend class AliceFileStream;

	std::string filePath;

	//モデル名
	std::string name;

	//メッシュ配列
	std::vector<std::unique_ptr<MeshGPUParticleModelMesh>> meshes;

	//姿勢行列
	IConstantBuffer* postureMatBuff;

	size_t verticeSize=0;

public:
	MeshGPUParticleAliceModelData() = default;
	~MeshGPUParticleAliceModelData() = default;

private:
	//コピーコンストラクタ・代入演算子削除
	MeshGPUParticleAliceModelData& operator=(const MeshGPUParticleAliceModelData&) = delete;
	MeshGPUParticleAliceModelData(const MeshGPUParticleAliceModelData&) = delete;


};

class MeshGPUParticleAliceModel
{
protected:

	static IMultiAdapters* sMultiAdapters;

	static std::unordered_map<std::string,std::unique_ptr<MeshGPUParticleAliceModelData>> sModelDatas;

	MeshGPUParticleAliceModelData* modelData = nullptr;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MeshGPUParticleAliceModel() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MeshGPUParticleAliceModel() = default;

	/// <summary>
	/// メッシュ配列を取得
	/// </summary>
	/// <returns></returns>
	const std::vector<std::unique_ptr<MeshGPUParticleModelMesh>>& GetMeshs();

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="modelHandle">ハンドル</param>
	void SetModel(AliceModel* model_,BufferType type,bool bufferCreat_ = false);

	/// <summary>
	/// 共通初期化
	/// </summary>
	static void SSetMultiAdapters(IMultiAdapters* multiAdapters_);

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	void CreateBoneMeshIsVisibles(std::unordered_map<std::string,std::unordered_map<std::string,bool>>& boneMeshIsVisibles_);

	void InvisibleBoneMesh(std::unordered_map<std::string,std::unordered_map<std::string,bool>>& boneMeshIsVisibles_,const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	void VisibleBoneMesh(std::unordered_map<std::string,std::unordered_map<std::string,bool>>& boneMeshIsVisibles_,const std::string& meshName_,const std::string& boneName_,bool root_ = true);
	BoneMesh* GetBoneMesh(const std::string& meshName_,const std::string& boneName_);

	size_t GetVerticeSize();

protected:

	const ReturnMotionNode* PFindNodeAnim(const AliceMotionData* pAnimation_,const std::string& strNodeName_);
	
	void AddBoneMesh(std::vector<std::unique_ptr<BoneMesh>>& boneMeshs_,uint32_t indice_,const PosNormUvTangeColSkin& ver_,const std::vector<Bone>& bone_);

	uint32_t GetBoneIndex(const PosNormUvTangeColSkin& ver_);

	void PReadChildren(const std::vector<Node>&nodes_,const std::vector<std::unique_ptr<BoneMesh>>&boneMeshs_,BoneMesh* boneMesh_);

	//コピーコンストラクタ・代入演算子削除
	MeshGPUParticleAliceModel& operator=(const MeshGPUParticleAliceModel&) = delete;
	MeshGPUParticleAliceModel(const MeshGPUParticleAliceModel&) = delete;
};
