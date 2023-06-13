#include "Line2D.h"

void Line2D::SetStart(const Point2D& pos_)
{
	start = pos_;
}

const Point2D& Line2D::GetStart()const
{
	return start;
}

void Line2D::SetAxis(const AliceMathF::Vector2& vec_)
{
	axis = vec_;
}

const AliceMathF::Vector2& Line2D::GetAxis()const
{
	return axis;
}

void Line2D::Setlength_(float len_)
{
	length_ = len_;
}

float Line2D::Getlength_()const
{
	return 0.0f;
}
