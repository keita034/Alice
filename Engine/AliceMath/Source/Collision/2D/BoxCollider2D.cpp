#include "BoxCollider2D.h"

void BoxCollider2D::SetCenter(const AliceMathF::Vector2& position_)
{
	position[CENTER] = position_;
}

const AliceMathF::Vector2& BoxCollider2D::GetCenter()
{
	return position[CENTER];
}

void BoxCollider2D::SetLeftRadius(const AliceMathF::Vector2& radius_)
{
	radius[LEFT2TOP] = radius_;
}

const AliceMathF::Vector2& BoxCollider2D::GetLeftRadius()
{
	
	return radius[LEFT2TOP];
}

void BoxCollider2D::SetRightRadius(const AliceMathF::Vector2& radius_)
{
	radius[RIGHT2BOTTOM] = radius_;
}

const AliceMathF::Vector2& BoxCollider2D::GetRightRadius()const
{
	return radius[RIGHT2BOTTOM];
}

const AliceMathF::Vector2& BoxCollider2D::GetLeftTop()const
{
	return position[LEFTTOP];
}

const AliceMathF::Vector2& BoxCollider2D::GetRightBottom()const
{
	return position[RIGHTBOTTOM];

}

void BoxCollider2D::Update()
{
	position[LEFTTOP] = { position[CENTER].x - radius[LEFT2TOP].x,position[CENTER].y - radius[LEFT2TOP].y };

	position[RIGHTBOTTOM] = { position[CENTER].x + radius[RIGHT2BOTTOM].x,position[CENTER].y + radius[RIGHT2BOTTOM].y };
}

BoxCollider2D::BoxCollider2D()
{
	shapeType = COLLISIONSHAPE_BOX;
}

BoxCollider2D::~BoxCollider2D()
{
}
