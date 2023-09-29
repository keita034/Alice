#pragma once
#include "ErrorException.h"
#include"AliceMathUtility.h"
#include"fbxModel.h"



class FbxLoader
{

private:

	//D3D12デバイス
	ID3D12Device* device;

	std::string directoryPath;

	bool anicmatrixion = false;

	bool inverseU = false;

	bool inverseV = false;
	char PADING[5] = {};

	const size_t MAX_BONE_INDICES = 4;

	//読み込みのデフォルトフラグ
	const UINT ASSIMP_LOAD_FLAG_DEFAULT =
		aiProcess_Triangulate| //三角面化
		aiProcess_CalcTangentSpace| //接線ベクトル生成
		aiProcess_GenSmoothNormals| //スムージングベクトル生成
		aiProcess_GenUVCoords| //非マッピングを適切なUV座標に変換
		aiProcess_RemoveRedundantMaterials| //冗長なマテリアルを削除
		aiProcess_OptimizeMeshes| //メッシュ数を最適化
		aiProcess_MakeLeftHanded| //ノードを左手座標系に
		aiProcess_JoinIdenticalVertices|//インデックスを生成
		aiProcess_LimitBoneWeights;//各頂点が影響を受けるボーンを4に制限

	char PAGING[4]{};

	//const UINT ASSIMP_LOAD_FLAG_DEFAULT =
	//	aiProcess_Triangulate | //三角面化
	//	aiProcess_GenSmoothNormals | //スムージングベクトル生成
	//	aiProcess_GenBoundingBoxes | //AABBを生成
	//	aiProcess_FlipUVs|
	//	aiProcess_ConvertToLeftHanded|
	//	aiProcess_JoinIdenticalVertices |//インデックスを生成
	//	aiProcess_LimitBoneWeights;//各頂点が影響を受けるボーンを4に制限

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	void LoadModel(fbxModel* model, const std::string& modelDirectory, bool inverseU_, bool inverseV_, bool animeFlag);

	void LoadAnimation(fbxAnimation* animation, const std::string& modelDirectory, const std::string& name);

private:

	void PraseNodeRecurive(fbxModel* model, aiNode* fbxNode, Node* parent = nullptr);

	void ParseMesh(fbxModel* model, aiMesh* fbxMesh);

	void ParseMeshVertices(fbxModel* model, aiMesh* mesh);

	void ParseMeshFaces(fbxModel* model, aiMesh* mesh);

	void ParseMaterial(fbxModel* model, aiMesh* mesh, aiMaterial* material);

	void GetNodeNum(const aiNode* node, UINT32& num);

	void ParseSkin(fbxModel* model, aiMesh* mesh);

	//テクスチャ解析
	std::vector<TextureData*> LoadMatrixerialTextures(aiMaterial* cmatrix, aiTextureType type, std::string typeName, const aiScene* scene_);

	void PrintModelData(fbxModel* model);

	// ディレクトリを含んだファイルパスからファイル名を抽出する。
	std::string ExtractFileName(const std::string& path);

	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
};