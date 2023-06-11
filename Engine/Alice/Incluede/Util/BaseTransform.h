#pragma once
#include<AliceMathUtility.h>
#include<Camera.h>
#include<ConstantBuffer.h>
#include<AliceUtility.h>

class BaseTransform
{
protected:
	// 定数バッファ
	std::shared_ptr<IConstantBuffer> constBuff;
	//定数バッファのマッピング用ポインタ
	worldViewpojCamera constBuffMap;
	char PADING1[4];

	//初期化用のビュー行列計算
	static AliceMathF::Matrix4 defaultViewMat;

	//初期化用のプロジェクション行列計算
	static AliceMathF::Matrix4 defaultProjectionMat;

public:
	// ローカル座標
	AliceMathF::Vector3 translation = { 0.0f, 0.0f, 0.0f };
	// X,Y,Z軸回りのローカル回転角
	AliceMathF::Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	// ローカルスケール
	AliceMathF::Vector3 scale = { 1.0f, 1.0f, 1.0f };

	AliceMathF::Matrix4 matWorld;
private:
	char PADING2[4];
public:

	// 親となるワールド変換へのポインタ
	const BaseTransform* parent = nullptr;
};

namespace AliceMathF
{
	/// <summary>
	/// ワールド行列生成
	/// </summary>
	Matrix4 MakeWorldMatrix4(BaseTransform& transform);

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	Vector3 GetWorldPosition(BaseTransform& transform);
}
