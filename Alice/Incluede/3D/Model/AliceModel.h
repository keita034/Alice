#pragma once
#include<ModelMesh.h>
#include<StructuredBuffer.h>
#include<AliceMotionData.h>

class AliceModelData
{
public:
	AliceModelData();
	~AliceModelData();

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
	std::vector<ModelMesh> meshes;

	//姿勢行列
	std::unique_ptr<ConstantBuffer> postureMatBuff;

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



	//コピーコンストラクタ・代入演算子削除
	AliceModelData& operator=(const AliceModelData&) = delete;
	AliceModelData(const AliceModelData&) = delete;
};


class AliceModel
{
protected:

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;

	//ライト
	static Light* light;

	static std::vector<std::string>filePaths;

	static std::unordered_map<std::string, std::unique_ptr<AliceModelData>> modelDatas;

	static uint32_t modelCount;

	static const uint32_t maxModel = 1024;

	Material* modelMaterialData = nullptr;

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
	virtual void Draw(Transform& transform, const AliceMotionData* animation = nullptr, float frame = 0.0f, Material* material = nullptr);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="animation">アニメーションデータ</param>
	/// <param name="frame">フレーム</param>
	void AnimationUpdate(const AliceMotionData* animation, float frame);

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="textureIndex">テクスチャの名前</param>
	/// <param name="textureData">テクスチャデータ</param>
	/// <returns>成功したかどうか</returns>
	bool TransTexture(const std::string& materialName, const std::string& textureName, TextureData* textureData);

	/// <summary>
	/// テクスチャの変更
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="textureIndex">テクスチャのインデックス</param>
	/// <param name="textureData">テクスチャデータ</param>
	/// <returns>成功したかどうか</returns>
	bool TransTexture(const std::string& materialName, size_t textureIndex, TextureData* textureData);

	/// <summary>
	/// UV座標を反転
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="inverseU">U座標</param>
	/// <param name="inverseV">V座標</param>
	/// <returns>成功したかどうか</returns>
	bool FlipUV(const std::string& materialName, bool inverseU = false, bool inverseV = false);

	/// <summary>
	/// UV座標を回転
	/// </summary>
	/// <param name="materialName">マテリアルの名前</param>
	/// <param name="angle">角度</param>
	/// <returns>成功したかどうか</returns>
	bool rotationUV(const std::string& materialName, float angle);

	/// <summary>
	/// 頂点データを初期化
	/// </summary>
	void InitializeVertex();

	/// <summary>
	/// メッシュ配列を取得
	/// </summary>
	/// <returns></returns>
	const std::vector<ModelMesh>& GetMeshs();

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="modelHandle">ハンドル</param>
	void SetModel(uint32_t modelHandle);

	/// <summary>
	/// ライトをセット(共通)
	/// </summary>
	/// <param name="lightPtr">ライトのポインタ</param>
	static void SetLight(Light* lightPtr);

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <param name="filePath">ファイルディレクトリ</param>
	/// <returns>ハンドル</returns>
	static uint32_t CreateModel(const std::string& fileDirectoryPath);

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <param name="filePath">ファイルディレクトリ</param>
	/// <returns>ハンドル</returns>
	static uint32_t CreateToonModel(const std::string& fileDirectoryPath, const std::string& rampFilePath = "Resources/Default/defaultRamp.png");

	/// <summary>
	/// 共通初期化
	/// </summary>
	static void CommonInitialize();

protected:

	void ReadNodeHeirarchy(ModelMesh* mesh, const AliceMotionData* pAnimation, float AnimationTime, const Node* pNode, const AliceMathF::Matrix4& mxParentTransform);
	const MotionNode* FindNodeAnim(const AliceMotionData* pAnimation, const std::string& strNodeName);
	void CalcInterpolatedScaling(AliceMathF::Vector3& mxOut, float AnimationTime, const MotionNode* pNodeAnim);
	bool FindScaling(float AnimationTime, const MotionNode* pNodeAnim, UINT& nScalingIndex);
	void CalcInterpolatedRotation(AliceMathF::Quaternion& mxOut, float AnimationTime, const MotionNode* pNodeAnim);
	bool FindRotation(float AnimationTime, const MotionNode* pNodeAnim, UINT& nRotationIndex);
	void CalcInterpolatedPosition(AliceMathF::Vector3& mxOut, float AnimationTime, const MotionNode* pNodeAnim);
	bool FindPosition(float AnimationTime, const MotionNode* pNodeAnim, UINT& nPosIndex);
	void ModelDraw(Transform& transform);
	void ModelAnimationDraw(Transform& transform);

	//コピーコンストラクタ・代入演算子削除
	AliceModel& operator=(const AliceModel&) = delete;
	AliceModel(const AliceModel&) = delete;
};

