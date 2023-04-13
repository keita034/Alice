﻿#include "Line2D.h"

void Line2D::SetStart(const Point2D& pos)
{
	start = pos;
}

const Point2D& Line2D::GetStart()
{
	return start;
}

void Line2D::SetAxis(const AliceMathF::Vector2& vec)
{
	axis = vec;
}

const AliceMathF::Vector2& Line2D::GetAxis()
{
	return axis;
}

void Line2D::SetLength(float len)
{
	length = len;
}

float Line2D::GetLength()
{
	return 0.0f;
}
