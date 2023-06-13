#include "Segment2D.h"

void Segment2D::SetStart(const AliceMathF::Vector2& pos_)
{
	start = pos_;
}

const AliceMathF::Vector2& Segment2D::GetStart()const
{
	return start;
}

void Segment2D::SetEnd(const AliceMathF::Vector2& pos_)
{
	end = pos_;
}

const AliceMathF::Vector2& Segment2D::GetEnd()const
{
	return end;
}

void Segment2D::CreateSegment2D(const AliceMathF::Vector2& center_, const AliceMathF::Vector2& axis_, float height_)
{
	start = center_ + (-axis_ * height_);
	end = center_ + (axis_ * height_);
}

AliceMathF::Vector2 Segment2D::GetVector()const
{
	return end - start;
}
