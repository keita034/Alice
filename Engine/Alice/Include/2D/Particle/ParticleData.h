#pragma once

#include<AliceMathUtility.h>

struct VerPosColScaRot
{
	AliceMathF::Vector3 pos;//座標
	AliceMathF::Vector4 color;//カラー
	float scale;//スケール
	float rotation;//回転角
};

struct ParticleConstBuffData
{
	AliceMathF::Matrix4 mat;
	AliceMathF::Matrix4 matBillboard;
};

//パーティクルデータ
struct ParticleData
{
	//座標
	AliceMathF::Vector3 position = {};
	//速度
	AliceMathF::Vector3 velocity = {};
	//加速度
	AliceMathF::Vector3 accel = {};

	//現在フレーム
	uint32_t frame = 0;
	//終了フレーム
	uint32_t numFrame = 0;

	//スケール
	float scale = 1.0f;
	//初期値
	float sScale = 1.0f;
	//最終値
	float eScale = 0.0f;

	//回転角
	float rotation = 0.0f;
	//初期値
	float sRotation = 0.0f;
	//最終値
	float eRotation = 0.0f;

	//カラー
	AliceMathF::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	//初期値
	AliceMathF::Vector4 sColor = { 1.0f,1.0f,1.0f,1.0f };
	//最終値
	AliceMathF::Vector4 eColor = { 1.0f,1.0f,1.0f,1.0f };
};