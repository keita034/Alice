#include "RayCastCollider.h"

const float RayCastCollider::GetRange()
{
    return range;
}

void RayCastCollider::SetRange(float range_)
{
    range = range_;
}
