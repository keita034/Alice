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
	float length;

	char PADING2[4]{};

public:

	CapsuleCllider2D();
	virtual ~CapsuleCllider2D() = default;

	/// <summary>
	/// 半径をセット
	/// </summary>
	void SetRadius(float rad);

	/// <summary>
	/// 半径を取得
	/// </summary>
	float GetRadius();

	/// <summary>
	/// 縦幅をセット
	/// </summary>
	void SetHeight(float h);

	/// <summary>
	/// 縦幅を取得
	/// </summary>
	float GetHeight();

	/// <summary>
	/// 中心座標をセット
	/// </summary>
	void SetCenter(const Point2D& pos);

	/// <summary>
	/// 中心座標を取得
	/// </summary>
	const AliceMathF::Vector2& GetCenter();

	/// <summary>
	/// 中心座標を動かす
	/// </summary>
	void MoveCenter(AliceMathF::Vector2 move);

	/// <summary>
	/// 軸の方向をセット
	/// </summary>
	void SetAxis(const AliceMathF::Vector2& vec);

	/// <summary>
	/// 軸の方向を取得
	/// </summary>
	const AliceMathF::Vector2& GetAxis();

	/// <summary>
	/// カプセルを線分と見たときの線分を取得
	/// </summary>
	/// <returns></returns>
	Segment2D& GetSegment();

	/// <summary>
	/// 始点を取得
	/// </summary>
	Point2D GetStart();

	/// <summary>
	/// 終点を取得
	/// </summary>
	Point2D GetEnd();

	/// <summary>
	/// 線分の長さを生成
	/// </summary>
	void SetLength();

	/// <summary>
	/// 線分の長さを取得
	/// </summary>
	float GetLength();

};

