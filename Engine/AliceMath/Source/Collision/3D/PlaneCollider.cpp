#include "PlaneCollider.h"

float PlaneCollider::GetDistance()const
{
    return distance;
}

void PlaneCollider::SetDistance(float distance_)
{
    distance = distance_;
}

const AliceMathF::Vector4& PlaneCollider::GetNormal()const
{
    return normal;
}

void PlaneCollider::SetNormal(const AliceMathF::Vector4& normal_)
{
    normal = normal_;
}
