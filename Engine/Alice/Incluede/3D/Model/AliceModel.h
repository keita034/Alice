#pragma once
#include<ModelMesh.h>
#include<StructuredBuffer.h>
#include<AliceMotionData.h>

class AliceModelData
{
private:
	//フレンドクラス
	friend class AliceModel;
	friend class AliceFileStream;
	friend class AliceToonModel;

	std::string filePath;

	uint32_t modelHandle;
	char PADING[4]{};

	//モデル名
	std::string name;

	//メッシュ配列
	std::vector<std::unique_ptr<ModelMesh>> meshes;

	//姿勢行列
	std::unique_ptr<IConstantBuffer> postureMatBuff;

	//ノード配列
	std::vector<Node> nodes;

	AliceMathF::Matrix4 globalInverseTransform;

	//アニメーション中か
	bool IsAnime = false;

	//頂点データ初期化
	bool vertexInitialize = false;

	//アニメーションできるか
	bool canAnimation = false;

	bool isToon = false;

	char PADING3[4]{};

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
	virtual void Draw(const Transform& transform_, const AliceMotionData* animation_ = nullptr, float frame_ = 0.0f, const Material* material_ = nullptr);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="animation">アニメーションデータ</param>
	/// <param name="frame">フレーム</param>
	void AnimationUpdate(const AliceMotionData* animation_, float frame_);

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

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="modelHandle">ハンドル</param>
	void SetModel(uint32_t modelHandle_);

	/// <summary>
	/// ライトをセット(共通)
	/// </summary>
	/// <param name="lightPtr">ライトのポインタ</param>
	static void SetLight(Light* light_);

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

protected:

	void PReadNodeHeirarchy(ModelMesh* mesh_, const AliceMotionData* pAnimation_, float AnimationTime_, const Node* pNode_, const AliceMathF::Matrix4& mxParentTransform_);
	const MotionNode* PFindNodeAnim(const AliceMotionData* pAnimation_, const std::string& strNodeName_);
	void PCalcInterpolatedScaling(AliceMathF::Vector3& mxOut_, float animationTime_, const MotionNode* pNodeAnim_);
	bool PFindScaling(float AnimationTime_, const MotionNode* pNodeAnim_, uint32_t& nScalingIndex_);
	void PCalcInterpolatedRotation(AliceMathF::Quaternion& mxOut_, float animationTime_, const MotionNode* pNodeAnim_);
	bool PFindRotation(float AnimationTime_, const MotionNode* pNodeAnim_, uint32_t& nRotationIndex_);
	void PCalcInterpolatedPosition(AliceMathF::Vector3& mxOut_, float animationTime_, const MotionNode* pNodeAnim_);
	bool PFindPosition(float AnimationTime_, const MotionNode* pNodeAnim_, uint32_t& nPosIndex_);
	void PModelDraw(const Transform& transform_);
	void PModelAnimationDraw(const Transform& transform_);

	//コピーコンストラクタ・代入演算子削除
	AliceModel& operator=(const AliceModel&) = delete;
	AliceModel(const AliceModel&) = delete;
};

