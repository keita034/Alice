#include "CapsuleShape.h"

#include<JoltCapsuleShape.h>

AlicePhysics::PhysicsShapeType AlicePhysics::CapsuleShape::GetShapeType()
{
	return type;
}

float AlicePhysics::CapsuleShape::GetHalfHeightOfCylinder()const
{
	return halfHeightOfCylinder;
}

float AlicePhysics::CapsuleShape::GetRadius()const
{
	return radius;
}

AlicePhysics::CapsuleShape* AlicePhysics::CreateCapsuleShape(float halfHeightOfCylinder_,float radius_)
{
	CapsuleShape* lShape = nullptr;

	//ジョルト
#if AlicePhysicsJolt

	lShape = new JoltCapsuleShape();
	lShape->Initialize(halfHeightOfCylinder_,radius_);

#endif

	return lShape;
}