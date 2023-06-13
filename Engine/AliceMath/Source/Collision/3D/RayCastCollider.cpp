#include "RayCastCollider.h"

float RayCastCollider::GetRange()const
{
    return range;
}

void RayCastCollider::SetRange(float range_)
{
    range = range_;
}
