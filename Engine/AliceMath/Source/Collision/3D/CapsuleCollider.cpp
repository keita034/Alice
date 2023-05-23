#include "CapsuleCollider.h"

void CapsuleCollider::SetUpAxis(const AliceMathF::Vector3& axis)
{
	upAxis = axis;
}

const AliceMathF::Vector3& CapsuleCollider::GetUpAxis()
{
	return upAxis;
}

void CapsuleCollider::SetHeight(float h)
{
	height = h;
}

float CapsuleCollider::GetHeight()
{
	return height;
}

void CapsuleCollider::SetRadius(float r)
{
	radius = r;
}

float CapsuleCollider::GetRadius()
{
	return radius;
}

void CapsuleCollider::SetCenter(const AliceMathF::Vector3& pos)
{
	center = pos;
}

const AliceMathF::Vector3& CapsuleCollider::GetCenter()
{
	return center;
}

