#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<string>

ALICE_SUPPRESS_WARNINGS_END

#include<Vector3.h>

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