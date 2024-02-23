#pragma once
#include"AliceMathUtility.h"

#include<string>
#include<PosNormUvTangeColSkin.h>
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



#pragma region モデルアニメーション関係



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



#pragma endregion

