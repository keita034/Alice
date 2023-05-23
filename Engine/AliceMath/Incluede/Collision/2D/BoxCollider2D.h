#pragma once
#include "Base2DCollider.h"
#include<AliceMathUtility.h>

class BoxCollider2D : public Base2DCollider
{
protected:

	//座標
	std::array<AliceMathF::Vector2, 3> position;

	//半径
	std::array<AliceMathF::Vector2, 2> radius;

public:

	enum PositionIndex
	{
		//中心
		CENTER,
		//左上
		LEFTTOP,
		//右下
		RIGHTBOTTOM
	};

	enum RadiusIndex
	{
		//左上
		LEFT2TOP,
		//右下
		RIGHT2BOTTOM,
	};

	/// <summary>
	///	中心座標をセット
	/// </summary>
	void SetCenter(const AliceMathF::Vector2& pos);

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	const AliceMathF::Vector2& GetCenter();

	/// <summary>
	/// 左上の半径をセット
	/// </summary>
	void SetLeftRadius(const AliceMathF::Vector2& rad);

	/// <summary>
	/// 左上の半径を取得
	/// </summary>
	const AliceMathF::Vector2& GetLeftRadius();

	/// <summary>
	/// 右下の半径をセット
	/// </summary>
	void SetRightRadius(const AliceMathF::Vector2& rad);

	/// <summary>
	/// 右下の半径を取得
	/// </summary>
	/// <returns></returns>
	const AliceMathF::Vector2& GetRightRadius();

	/// <summary>
	/// 左上座標を取得
	/// </summary>
	const AliceMathF::Vector2& GetLeftTop();

	/// <summary>
	/// 右下座標を取得
	/// </summary>
	const AliceMathF::Vector2& GetRightBottom();

	void Update();

	BoxCollider2D();
	virtual ~BoxCollider2D();
};

