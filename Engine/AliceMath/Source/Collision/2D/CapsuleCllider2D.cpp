#include "CapsuleCllider2D.h"

CapsuleCllider2D::CapsuleCllider2D()
{
	shapeType = COLLISIONSHAPE_CAPSULE;
}

CapsuleCllider2D::~CapsuleCllider2D()
{
}

void CapsuleCllider2D::Update()
{
	segment.CreateSegment2D(center, axis, height);

	Segment2D seg = GetSegment();
	length_ = (seg.GetEnd() - seg.GetStart()).length_();
}

void CapsuleCllider2D::SetRadius(float radius_)
{
	radius = radius_;
}

float CapsuleCllider2D::GetRadius()const
{
	return radius;
}

void CapsuleCllider2D::SetHeight(float height_)
{
	height = height_;
}

float CapsuleCllider2D::GetHeight()const
{
	return height;
}

void CapsuleCllider2D::SetCenter(const Point2D& position_)
{
	center = position_;
}

const AliceMathF::Vector2& CapsuleCllider2D::GetCenter()const
{
	return center;
}

void CapsuleCllider2D::MoveCenter(AliceMathF::Vector2 move_)
{
	center += move_;
}

void CapsuleCllider2D::SetAxis(const AliceMathF::Vector2& axis_)
{
	axis = axis_;
}

const AliceMathF::Vector2& CapsuleCllider2D::GetAxis()const
{
	return axis;
}

const Segment2D& CapsuleCllider2D::GetSegment()const
{
	return segment;
}

Point2D CapsuleCllider2D::GetStart()const
{
	return center + (-axis * height);
}

Point2D CapsuleCllider2D::GetEnd()const
{
	return center + (axis * height);
}

float CapsuleCllider2D::Getlength_()const
{
	return 0.0f;
}
