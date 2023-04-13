#include "BoxCollider2D.h"

void BoxCollider2D::SetCenter(const AliceMathF::Vector2& pos)
{
	position[CENTER] = pos;
}

const AliceMathF::Vector2& BoxCollider2D::GetCenter()
{
	return position[CENTER];
}

void BoxCollider2D::SetLeftRadius(const AliceMathF::Vector2& rad)
{
	radius[LEFT2TOP] = rad;
}

const AliceMathF::Vector2& BoxCollider2D::GetLeftRadius()
{
	
	return radius[LEFT2TOP];
}

void BoxCollider2D::SetRightRadius(const AliceMathF::Vector2& rad)
{
	radius[RIGHT2BOTTOM] = rad;
}

const AliceMathF::Vector2& BoxCollider2D::GetRightRadius()
{
	return radius[RIGHT2BOTTOM];
}

const AliceMathF::Vector2& BoxCollider2D::GetLeftTop()
{
	return position[LEFTTOP];
}

const AliceMathF::Vector2& BoxCollider2D::GetRightBottom()
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
