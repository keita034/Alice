#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<string>

ALICE_SUPPRESS_WARNINGS_END

#include<Matrix4.h>

/// <summary>
/// 骨
/// </summary>
class Bone
{
public:
	//名前
	std::string name;

	//計算後行列
	AliceMathF::Matrix4 matrix;
	//オフセット行列
	AliceMathF::Matrix4 offsetMatirx;

	//番号
	uint32_t index;

private:
	int32_t PADING;

};

//ボーン構造体
struct BoneData
{
	//ボーンデータ
	std::array<AliceMathF::Matrix4,MAX_BONES> boneMat;
};
