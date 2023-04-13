#include "CapsuleCllider2D.h"

CapsuleCllider2D::CapsuleCllider2D()
{
	shapeType = COLLISIONSHAPE_CAPSULE;
}

void CapsuleCllider2D::SetRadius(float rad)
{
	radius = rad;
}

float CapsuleCllider2D::GetRadius()
{
	return radius;
}

void CapsuleCllider2D::SetHeight(float h)
{
	height = h;
}

float CapsuleCllider2D::GetHeight()
{
	return height;
}

void CapsuleCllider2D::SetCenter(const Point2D& pos)
{
	center = pos;
}

const AliceMathF::Vector2& CapsuleCllider2D::GetCenter()
{
	return center;
}

void CapsuleCllider2D::MoveCenter(AliceMathF::Vector2 move)
{
	center += move;
}

void CapsuleCllider2D::SetAxis(const AliceMathF::Vector2& vec)
{
	axis = vec;
}

const AliceMathF::Vector2& CapsuleCllider2D::GetAxis()
{
	return axis;
}

Segment2D& CapsuleCllider2D::GetSegment()
{
	segment.CreateSegment2D(center, axis, height);
	return segment;
}

Point2D CapsuleCllider2D::GetStart()
{
	return center + (-axis * height);;
}

Point2D CapsuleCllider2D::GetEnd()
{
	return center + (axis * height);
}

void CapsuleCllider2D::SetLength()
{
	Segment2D seg = GetSegment();
	length = (seg.GetEnd() - seg.GetStart()).Length();
}

float CapsuleCllider2D::GetLength()
{
	return 0.0f;
}
