#include "SphereCollider.h"

float SphereCollider::GetRadius()const
{
	return radius;
}

void SphereCollider::SetRadius(float radius_)
{
	radius = radius_;
}

const AliceMathF::Vector4& SphereCollider::GetCenter()const
{
	return center;
}

void SphereCollider::SetCenter(const AliceMathF::Vector4& center_)
{
	center = center_;
}
