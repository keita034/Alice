#include "SphereShape.h"
#include<JoltSphereShape.h>

AlicePhysics::PhysicsShapeType AlicePhysics::SphereShape::GetShapeType()
{
	return type;
}

float AlicePhysics::SphereShape::GetRadius()const
{
	return radius;
}

AlicePhysics::SphereShape* AlicePhysics::CreateSphereShape(float radius_)
{
	SphereShape* lShape = nullptr;

	//バレット
#if defined(AlicePhysicsBullet) 


#endif

	//フィジックス
#if  defined(AlicePhysicsPhysX)


#endif

	//ジョルト
#if defined(AlicePhysicsJolt)

	lShape = new JoltSphereShape();
	lShape->Initialize(radius_);

#endif

	return lShape;
}
