#pragma once
#include"ModelMesh.h"
#include"fbxAnimation.h"
#include"StructuredBuffer.h"

//struct PosNormUvTangeCol
//{
//	AliceMathF::Vector3 position; // 位置座標
//	AliceMathF::Vector3 normal; // 法線
//	AliceMathF::Vector2 uv; // uv座標
//	AliceMathF::Vector3 tangent; // 接空間
//	AliceMathF::Vector4 color; // 頂点色
//};



class fbxModel:private Model
{

private:

	//モデル名
	std::string name;
	//ノード配列
	std::vector<ModelMesh> meshes;

	//フレンドクラス
	friend class FbxLoader;

	std::unique_ptr<ConstantBuffer> postureMatBuff;

	static std::shared_ptr<ComputeRelation> computeRelation;

	const aiScene* mScene = nullptr;

	std::vector<Node> nodes;

	float oldFrame = 0.0f;

	AliceMathF::Matrix4 globalInverseTransform;
	char PADING1[4]{};

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	Material* modelMaterialData;

	bool IsAnime = false;

	bool vertexInitialize = false;

	char PADING2[6]{};
public:

	fbxModel() = default;
	~fbxModel() = default;

	 void Initialize();

	 void Draw(Transform* transform, Material* material = nullptr);

	void AnimationUpdate(const fbxAnimation* animation, float frame);

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="filePath">オブジェクトまでのファイルパス</param>
	void Create(const char* filePath, bool smoothing, bool inverseU = false, bool inverseV = false, bool animeFlag = false);

	static void CommonInitialize();

	bool TransTexture(const std::string& materialName, const std::string& textureName, TextureData* textureData);

	bool TransTexture(const std::string& materialName, size_t textureIndex, TextureData* textureData);

	bool FlipUV(const std::string& materialName,bool inverseU = false, bool inverseV = false);

	bool rotationUV(const std::string& materialName, float angle);

	void InitializeVertex();

	const std::vector<ModelMesh>& GetMeshs();

private:

	void ReadNodeHeirarchy(ModelMesh* mesh, const aiAnimation* pAnimation, float AnimationTime, const aiNode* pNode, const AliceMathF::Matrix4& mxParentTransform);
	aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& strNodeName);
	void CalcInterpolatedScaling(AliceMathF::Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim);
	bool FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nScalingIndex);
	void CalcInterpolatedRotation(AliceMathF::Vector4& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim);
	bool FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nRotationIndex);
	void CalcInterpolatedPosition(AliceMathF::Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim);
	bool FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nPosIndex);

	//コピーコンストラクタ・代入演算子削除
	fbxModel& operator=(const fbxModel&) = delete;
	fbxModel(const fbxModel&) = delete;
};

