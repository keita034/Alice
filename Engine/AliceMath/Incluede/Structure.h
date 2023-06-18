#pragma once
#include"AliceMathUtility.h"

#include<string>

#pragma warning(push)
#pragma warning(disable: 5264)
//ボーンの最大数
constexpr size_t MAX_BONES = 128;
constexpr size_t MAX_BONE_INDICES = 4;
#pragma warning(pop)

#pragma region 頂点データ

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・カラー</para>
/// </summary>
struct PosColor
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector4 color;//カラー
};

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・法線ベクトル</para>
/// <para>・uv座標</para>
/// </summary>
struct PosNormalUv
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector3 normal;//法線ベクトル
	AliceMathF::Vector2 uv;//uv座標
};

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・uv座標</para>
/// <para>・カラー</para>
/// </summary>
struct PosUvColor
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector2 uv;//uv座標
	AliceMathF::Vector4 color;//カラー
};

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・法線</para>
/// <para>・接空間</para>
/// <para>・カラー</para>
/// </summary>
struct PosNormUvTangeCol
{
	AliceMathF::Vector4 position = { 0.0f,0.f,0.0f,1.0f }; // 座標
	AliceMathF::Vector3 normal; // 法線
	AliceMathF::Vector2 uv; // uv座標
	AliceMathF::Vector3 tangent; // 接空間
	AliceMathF::Vector4 color; // カラー
};

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・法線</para>
/// <para>・接空間</para>
/// <para>・カラー</para>
/// <para>・ボーンの番号</para>
/// <para>・ボーンの重み</para>
/// </summary>
struct PosNormUvTangeColSkin
{
	AliceMathF::Vector4 position = { 0.0f,0.f,0.0f,1.0f }; // 座標
	AliceMathF::Vector3 normal; // 法線
	AliceMathF::Vector2 uv; // uv座標
	AliceMathF::Vector3 tangent; // 接空間
	AliceMathF::Vector4 color; // カラー

	std::array<UINT, MAX_BONE_INDICES> boneIndex;	// ボーンの番号
	std::array<float, MAX_BONE_INDICES> boneWeight;	// ボーンの重み
};

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・法線</para>
/// <para>・ボーンの番号</para>
/// <para>・ボーンの重み</para>
/// </summary>
struct PosNormSkin
{
	AliceMathF::Vector4 position; // 座標
	AliceMathF::Vector3 normal; // 法線

	std::array<UINT, MAX_BONE_INDICES> boneIndex;	// ボーンの番号
	std::array<float, MAX_BONE_INDICES> boneWeight;	// ボーンの重み
};

/// <summary>
/// <para>頂点データ</para>
/// <para>・座標</para>
/// <para>・法線</para>
/// </summary>
struct PosNorm
{
	AliceMathF::Vector4 position; // 位置座標
	AliceMathF::Vector3 normal; // 法線
};

#pragma endregion

//座標変換行列
struct ConstBufferDataTransform
{
	AliceMathF::Matrix4 mat;//3D変換行列
};

/// <summary>
/// カメラ構造体
/// </summary>
struct worldViewpojCamera
{
	//ワールド行列
	AliceMathF::Matrix4 world;
	//ワールド座標
	AliceMathF::Matrix4 matWorld;
	//カメラ座標(ワールド座標)
	AliceMathF::Vector3 cameraPos;
};

/// <summary>
/// カメラ構造体(ビュープロジェクション付き)
/// </summary>
struct worldViewpojCameraWVP
{
	//ワールド行列
	AliceMathF::Matrix4 world;
	//ワールド座標
	AliceMathF::Matrix4 matWorld;
	//カメラ座標(ワールド座標)
	AliceMathF::Vector3 cameraPos;
	//ビュープロジェクション
	AliceMathF::Matrix4 WVP;

};

//マテリアル用定数バッファ構造体
struct ConstBuffDataMaterial
{
	//アンビエント係数
	AliceMathF::Vector3 ambient;
	//パディング
	float pad1;
	//ディフューズ係数
	AliceMathF::Vector3 diffuse;
	//パディング
	float pad2;
	//スペキュラー係数
	AliceMathF::Vector3 specular;
	//パディング
	float pad3;
	//エミッション
	AliceMathF::Vector3 emission;
	//パディング
	float pad4;
	//光沢
	float shininess;
	//アルファ
	float alpha;
};

//モデルマテリアル
struct ModelMaterial
{
	//マテリアル名
	std::string name;
	//アンビエント影響度
	AliceMathF::Vector3 ambient = { 0.06f, 0.06f, 0.06f };
	//ディフューズ影響度
	AliceMathF::Vector3 diffuse = { 0.0f,0.0f,0.0f };
	//スペキュラー影響度
	AliceMathF::Vector3 specular = { 0.0f,0.0f,0.0f };
	//エミッション影響度
	AliceMathF::Vector3 emission = { 0.0f,0.0f,0.0f };
	//光沢
	float shininess = 1.0f;
	//アルファ
	float alpha = 1.0f;
	//テクスチャファイル名
	std::string textureFiename;

	ConstBuffDataMaterial GetConstBuffMaterial();
};

#pragma region モデルアニメーション関係

/// <summary>
/// 骨
/// </summary>
struct Bone
{
	//名前
	std::string name;

	//計算後行列
	AliceMathF::Matrix4 matrix;
	//オフセット行列
	AliceMathF::Matrix4 offsetMatirx;

	//番号
	UINT index;

private:
	char PADING[4] = {};

};

/// <summary>
/// ノード
/// </summary>
struct Node
{
	//名前
	std::string name;
	//ローカル変形行列
	AliceMathF::Matrix4 transform;
	//グローバル変形行列
	AliceMathF::Matrix4 globalTransform;
	//親ノード
	Node* parent = nullptr;
	//子ノード
	std::vector<Node*>childrens;

};

//定数バッファ用データ構造体(スキニング)
struct SkinData
{
	//ボーンマトリックス
	std::vector<AliceMathF::Matrix4> bones;
};

//コンピュートシェーダ入力用
struct SkinComputeInput
{
	//頂点データ
	std::vector<PosNormSkin> vertices;
	//ボーンデータ
	SkinData bone;
};

//ボーン構造体
struct BoneData
{
	//ボーンデータ
	std::array<AliceMathF::Matrix4, MAX_BONES> boneMat;
};

//Vector3のアニメーションキー
struct VectorKey
{
	//キーの時間
	float time;

	//キーの値
	AliceMathF::Vector3 value;

private:

	char PADING[4]{};
};

//Quaternionのアニメーションキー
struct QuaternionKey
{
	//キーの時間
	float time;

	//キーの値
	AliceMathF::Quaternion value;
};

//アニメーションノード
struct MotionNode
{
	//名前
	std::string name;

	//スケーリング
	std::vector<VectorKey>scalingKeys;

	//回転角
	std::vector<QuaternionKey>rotationKeys;

	//位置
	std::vector<VectorKey>positionKeys;
};

#pragma endregion

