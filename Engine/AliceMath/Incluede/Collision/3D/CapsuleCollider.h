#pragma once
#include "BaseCollider.h"

class CapsuleCollider : public BaseCollider
{
private:

	//上方向ベクトル
	AliceMathF::Vector3 upAxis;

	//縦幅
	float height;

	//半径
	float radius;

	//中心座標
	AliceMathF::Vector3 center;

public:

	/// <summary>
	/// 上方向ベクトルをセット
	/// </summary>
	/// <param name="upAxis"></param>
	void SetUpAxis(const AliceMathF::Vector3& upAxis_);

	/// <summary>
	/// 上方向ベクトルを取得
	/// </summary>
	const AliceMathF::Vector3& GetUpAxis()const;

	/// <summary>
	/// 縦幅(中心から)をセット
	/// </summary>
	/// <param name="height">縦幅(中心から)</param>
	void SetHeight(float height_);

	/// <summary>
	///  縦幅(中心から)を所得
	/// </summary>
	float GetHeight();

	/// <summary>
	/// 半径をセット
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius_);

	/// <summary>
	///  半径を所得
	/// </summary>
	float GetRadius();

	/// <summary>
	/// 中心座標をセット
	/// </summary>
	/// <param name="pos">座標</param>
	void SetCenter(const AliceMathF::Vector3& position_);

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	const AliceMathF::Vector3& GetCenter()const;

private:

	CapsuleCollider(const CapsuleCollider&) = delete;
	void operator = (const CapsuleCollider&) = delete;
};

