#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<array>

ALICE_SUPPRESS_WARNINGS_END

#include<Vector2.h>
#include<Vector3.h>
#include<Vector4.h>

#pragma warning(push)
#pragma warning(disable: 5264)
//ボーンの最大数
constexpr size_t MAX_BONES = 128;
constexpr size_t MAX_BONE_INDICES = 4;
#pragma warning(pop)

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

	std::array<UINT,MAX_BONE_INDICES> boneIndex;	// ボーンの番号
	std::array<float,MAX_BONE_INDICES> boneWeight;	// ボーンの重み
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

	std::array<UINT,MAX_BONE_INDICES> boneIndex;	// ボーンの番号
	std::array<float,MAX_BONE_INDICES> boneWeight;	// ボーンの重み
};
