#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<vector>
#include<string>

ALICE_SUPPRESS_WARNINGS_END

#include<Vector3.h>
#include<Quaternion.h>


//Vector3のアニメーションキー
struct VectorKey
{
	//キーの時間
	float time;

	//キーの値
	AliceMathF::Vector3 value;

private:

	int32_t PADING;
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