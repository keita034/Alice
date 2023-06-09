﻿#pragma once
#include <Vector2.h>

/// <summary>
/// 無限直線
/// </summary>
class Line2D
{
private:

	//始点
	Point2D start;
	//軸の方向
	AliceMathF::Vector2 axis;
	//長さ
	float length_;

public:

	Line2D() = default;
	~Line2D() = default;

	/// <summary>
	/// 始点をセット
	/// </summary>
	void SetStart(const Point2D& pos_);

	/// <summary>
	/// 始点を取得
	/// </summary>
	const Point2D& GetStart()const;

	/// <summary>
	/// 軸の方向をセット
	/// </summary>
	void SetAxis(const AliceMathF::Vector2& vec_);

	/// <summary>
	/// 軸の方向を取得
	/// </summary>
	const AliceMathF::Vector2& GetAxis()const;

	/// <summary>
	/// 長さをセット
	/// </summary>
	void Setlength_(float len_);

	/// <summary>
	/// 長さを取得
	/// </summary>
	float Getlength_()const;
};

