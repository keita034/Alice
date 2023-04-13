#include "AABBCollider.h"

void AABBCollider::SetCenter(const AliceMathF::Vector3& center_)
{
	center = center_;
}

const AliceMathF::Vector3& AABBCollider::GetCenter()
{
	return center;
}

void AABBCollider::SetSize(const AliceMathF::Vector3& size_)
{
	size = size_;
}

const AliceMathF::Vector3& AABBCollider::GetSize()
{
	return size;
}
