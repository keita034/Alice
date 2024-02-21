#pragma once
#include<Vector3.h>
#include<Matrix4.h>

/// <summary>
/// カメラ構造体
/// </summary>
struct WorldViewpojCamera
{
	//ワールド行列
	AliceMathF::Matrix4 world;
	//ワールド座標
	AliceMathF::Matrix4 matWorld;
	//カメラ座標(ワールド座標)
	AliceMathF::Vector3 cameraPos;
};