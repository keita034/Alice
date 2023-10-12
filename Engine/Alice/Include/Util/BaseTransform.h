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

	//初期化用のビュー行列計算
	static AliceMathF::Matrix4 sDefaultViewMat;

	//初期化用のプロジェクション行列計算
	static AliceMathF::Matrix4 sDefaultProjectionMat;

	//定数バッファのマッピング用ポインタ
	worldViewpojCamera constBuffMap;

public:
	// ローカル座標
	AliceMathF::Vector3 translation = { 0.0f, 0.0f, 0.0f };
	// X,Y,Z軸回りのローカル回転角
	AliceMathF::Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	// ローカルスケール
	AliceMathF::Vector3 scale = { 1.0f, 1.0f, 1.0f };

	AliceMathF::Matrix4 matWorld;

	AliceMathF::Matrix4 rigidBodyMatWorld;

public:
	// 親となるワールド変換へのポインタ
	const BaseTransform* parent = nullptr;

public:

	BaseTransform() = default;
	virtual ~BaseTransform() = default;
};

namespace AliceMathF
{
	/// <summary>
	/// ワールド行列生成
	/// </summary>
	Matrix4 MakeWorldMatrix4(BaseTransform& transform_,const Quaternion& quaternion_);

		/// <summary>
	/// ワールド行列生成
	/// </summary>
	Matrix4 MakeWorldMatrix4(BaseTransform& transform_);

	/// <summary>
	/// ワールド行列生成
	/// </summary>
	Matrix4 MakeWorldMatrix4(BaseTransform& transform_,const AliceMathF::Matrix4& rotMat_);

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	Vector3 GetWorldPosition(BaseTransform& transform_);

	Matrix4 GetRotMatrix(BaseTransform& transform_);
}
