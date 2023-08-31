#pragma once
#include<ModelMesh.h>
#include<StructuredBuffer.h>
#include<AliceMotionData.h>
#include<AliceBlendTree.h>

class AliceModelData
{
private:
	//フレンドクラス
	friend class AliceModel;
	friend class AliceFileStream;
	friend class AliceToonModel;

	std::string filePath;

	//モデル名
	std::string name;

	//メッシュ配列
	std::vector<std::unique_ptr<ModelMesh>> meshes;

	//姿勢行列
	std::unique_ptr<IConstantBuffer> postureMatBuff;

	//ノード配列
	std::vector<Node> nodes;

	AliceMathF::Matrix4 globalInverseTransform;

	uint32_t modelHandle;

	//アニメーション中か
	bool IsAnime = false;

	//頂点データ初期化
	bool vertexInitialize = false;

	//アニメーションできるか
	bool canAnimation = false;

	bool isToon = false;

	TextureData* rampTex = nullptr;

public:
	AliceModelData();
	~AliceModelData();

private:
	//コピーコンストラクタ・代入演算子削除
	AliceModelData& operator=(const AliceModelData&) = delete;
	AliceModelData(const AliceModelData&) = delete;
};


class AliceModel
{
protected:

	static ID3D12Device* sDevice;
	static ID3D12GraphicsCommandList* sCmdList;

	//ライト
	static Light* sLight;

	static std::vector<std::string>sFilePaths;

	static std::unordered_map<std::string, std::unique_ptr<AliceModelData>> sModelDatas;

	static uint32_t sModelCount;

	static const uint32_t sMAX_MODEL = 1024;

	const Material* modelMaterialData = nullptr;

	AliceModelData* modelData = nullptr;

	
public:

	struct AnimationTransform
	{
		AliceMathF::Vector3 scaling;
		AliceMathF::Quaternion rotation;
		AliceMathF::Vector3 translation;

		AliceMathF::Matrix4 boneMatrix;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AliceModel() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AliceModel() = default;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="transform">ワールド変換データ</param>
	/// <param name="material">マテリアル(パイプライン)</param>
	virtual void Draw(const Transform& transform_, const AliceMotionData* animation_,const Material* material_ = nullptr);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Transform& transform_,AliceBlendTree* blendTree_,const Material* material_ = nullptr);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Transform& transform_);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Transform& transform_,const Material* material_);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="animation">アニメーションデータ</param>
	void AnimationUpdate(const AliceMotionData* animation_);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="animation">アニメーションデータ</param>
	void AnimationUpdate(AliceBlendTree* blendTree_);

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="textureIndex">テクスチャの名前</param>
	/// <param name="textureData">テクスチャデータ</param>
	/// <returns>成功したかどうか</returns>
	bool TransTexture(const std::string& materialName_, const std::string& textureName_, TextureData* textureData_);

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="textureIndex">テクスチャのインデックス</param>
	/// <param name="textureData">テクスチャデータ</param>
	/// <returns>成功したかどうか</returns>
	bool TransTexture(const std::string& materialName_, size_t textureIndex_, TextureData* textureData_);

	/// <summary>
	/// UV座標を反転
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="inverseU">U座標</param>
	/// <param name="inverseV">V座標</param>
	/// <returns>成功したかどうか</returns>
	bool FlipUV(const std::string& materialName_, bool inverseU_ = false, bool inverseV_ = false);

	/// <summary>
	/// UV座標を回転
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="angle">角度</param>
	/// <returns>成功したかどうか</returns>
	bool rotationUV(const std::string& materialName_, float angle_);

	/// <summary>
	/// 頂点データを初期化
	/// </summary>
	void InitializeVertex();

	/// <summary>
	/// メッシュ配列を取得
	/// </summary>
	/// <returns></returns>
	const std::vector<std::unique_ptr<ModelMesh>>& GetMeshs();

	AliceMathF::Vector3 GetBonePosition(const std::string boneName_);

	AliceModel::AnimationTransform GetAnimationTransform(AliceBlendTree* blendTree_,const std::string boneName);

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="modelHandle">ハンドル</param>
	void SetModel(uint32_t modelHandle_);

	void SetAlpha(float alpha_);

	/// <summary>
	/// ライトをセット(共通)
	/// </summary>
	/// <param name="lightPtr">ライトのポインタ</param>
	static void SSetLight(Light* light_);

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <param name="filePath">ファイルディレクトリ</param>
	/// <returns>ハンドル</returns>
	static uint32_t SCreateModel(const std::string& fileDirectoryPath_);

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <param name="filePath">ファイルディレクトリ</param>
	/// <returns>ハンドル</returns>
	static uint32_t SCreateToonModel(const std::string& fileDirectoryPath_, const std::string& rampFilePath_ = "Resources/Default/Ramp.png");

	/// <summary>
	/// 共通初期化
	/// </summary>
	static void SCommonInitialize(DirectX12Core* directX12Core_);

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

protected:

	void PReadNodeHeirarchy(ModelMesh* mesh_,const AliceMotionData* pAnimation_,const Node* pNode_,const AliceMathF::Matrix4& mxParentTransform_);
	void PReadNodeHeirarchy(ModelMesh* mesh_,AliceBlendTree* blendTree_,const Node* pNode_,const AliceMathF::Matrix4& mxParentTransform_);
	void PReadNodeHeirarchy(ModelMesh* mesh_, AliceBlendTree* blendTree_,const Node* pNode_, const AliceMathF::Matrix4& mxParentTransform_,const std::string& boneName_,bool& end_,AliceModel::AnimationTransform& dust_);
	const ReturnMotionNode* PFindNodeAnim(const AliceMotionData* pAnimation_, const std::string& strNodeName_);
	void PModelDraw(const Transform& transform_);
	void PModelAnimationDraw(const Transform& transform_);

	//コピーコンストラクタ・代入演算子削除
	AliceModel& operator=(const AliceModel&) = delete;
	AliceModel(const AliceModel&) = delete;
};

