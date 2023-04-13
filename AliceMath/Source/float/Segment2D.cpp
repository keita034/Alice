#include "Segment2D.h"

void Segment2D::SetStart(const AliceMathF::Vector2& pos)
{
	start = pos;
}

const AliceMathF::Vector2& Segment2D::GetStart()
{
	return start;
}

void Segment2D::SetEnd(const AliceMathF::Vector2& pos)
{
	end = pos;
}

const AliceMathF::Vector2& Segment2D::GetEnd()
{
	return end;
}

void Segment2D::CreateSegment2D(const AliceMathF::Vector2& center, const AliceMathF::Vector2& axis, float height)
{
	start = center + (-axis * height);
	end = center + (axis * height);
}

AliceMathF::Vector2 Segment2D::GetVector()
{
	return end - start;
}
