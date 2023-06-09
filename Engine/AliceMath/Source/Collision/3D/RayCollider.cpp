﻿#include "RayCollider.h"

const AliceMathF::Vector4& RayCollider::GetStart()const
{
    return start;
}

void RayCollider::SetStart(const AliceMathF::Vector4& start_)
{
    start = start_;
}

const AliceMathF::Vector4& RayCollider::GetDir()const
{
    return dir;
}

void RayCollider::SetDir(const AliceMathF::Vector4& dir_)
{
    dir = dir_;
}
