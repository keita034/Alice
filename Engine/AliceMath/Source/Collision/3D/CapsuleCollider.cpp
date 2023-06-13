#include "CapsuleCollider.h"

void CapsuleCollider::SetUpAxis(const AliceMathF::Vector3& upAxis_)
{
	upAxis = upAxis_;
}

const AliceMathF::Vector3& CapsuleCollider::GetUpAxis()const
{
	return upAxis;
}

void CapsuleCollider::SetHeight(float height_)
{
	height = height_;
}

float CapsuleCollider::GetHeight()
{
	return height;
}

void CapsuleCollider::SetRadius(float radius_)
{
	radius = radius_;
}

float CapsuleCollider::GetRadius()
{
	return radius;
}

void CapsuleCollider::SetCenter(const AliceMathF::Vector3& position_)
{
	center = position_;
}

const AliceMathF::Vector3& CapsuleCollider::GetCenter()const
{
	return center;
}

