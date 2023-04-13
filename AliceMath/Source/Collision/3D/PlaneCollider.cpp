#include "PlaneCollider.h"

const float PlaneCollider::GetDistance()
{
    return distance;
}

void PlaneCollider::SetDistance(float distance_)
{
    distance = distance_;
}

const AliceMathF::Vector4& PlaneCollider::GetNormal()
{
    return normal;
}

void PlaneCollider::SetNormal(const AliceMathF::Vector4& normal_)
{
    normal = normal_;
}
