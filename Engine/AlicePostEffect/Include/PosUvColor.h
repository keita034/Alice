#pragma once
#include<Vector2.h>
#include<Vector3.h>
#include<Vector4.h>

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