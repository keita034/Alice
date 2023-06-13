#pragma once
#include<Base2DCollider.h>
#include<Segment2D.h>

class CapsuleCllider2D : public Base2DCollider
{
private:

	//半径
	float radius;
	//縦幅
	float height;
	//中心座標
	Point2D center;
	//軸の方向
	AliceMathF::Vector2 axis;
	//カプセルの中心線分
	Segment2D segment;
	//長さ
	float length_;

	char PADING2[4]{};

public:

	CapsuleCllider2D();
	virtual ~CapsuleCllider2D();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 半径をセット
	/// </summary>
	void SetRadius(float radius_);

	/// <summary>
	/// 半径を取得
	/// </summary>
	float GetRadius()const;

	/// <summary>
	/// 縦幅をセット
	/// </summary>
	void SetHeight(float height_);

	/// <summary>
	/// 縦幅を取得
	/// </summary>
	float GetHeight()const;

	/// <summary>
	/// 中心座標をセット
	/// </summary>
	void SetCenter(const Point2D& position_);

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	const AliceMathF::Vector2& GetCenter()const;

	/// <summary>
	/// 中心座標を動かす
	/// </summary>
	void MoveCenter(AliceMathF::Vector2 move_);

	/// <summary>
	/// 軸の方向をセット
	/// </summary>
	void SetAxis(const AliceMathF::Vector2& axis_);

	/// <summary>
	/// 軸の方向を取得
	/// </summary>
	const AliceMathF::Vector2& GetAxis()const;

	/// <summary>
	/// カプセルを線分と見たときの線分を取得
	/// </summary>
	/// <returns></returns>
	const Segment2D& GetSegment()const;

	/// <summary>
	/// 始点を取得
	/// </summary>
	Point2D GetStart()const;

	/// <summary>
	/// 終点を取得
	/// </summary>
	Point2D GetEnd()const;

	/// <summary>
	/// 線分の長さを取得
	/// </summary>
	float Getlength_()const;

};

