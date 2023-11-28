#include "BoxShape.h"

#include<JoltBoxShape.h>

AlicePhysics::PhysicsShapeType AlicePhysics::BoxShape::GetShapeType()
{
	return type;
}

const AliceMathF::Vector3& AlicePhysics::BoxShape::GetHalfExtent()const
{
	return halfExtent;
}

AlicePhysics::BoxShape* AlicePhysics::CreateBoxShape(const AliceMathF::Vector3& halfExtent_)
{
	BoxShape* lShape = nullptr;

	//バレット
#if defined(AlicePhysicsBullet) 


#endif

	//フィジックス
#if  defined(AlicePhysicsPhysX)


#endif

	//ジョルト
#if defined(AlicePhysicsJolt)

	lShape = new JoltBoxShape();
	lShape->Initialize(halfExtent_);

#endif

	return lShape;
}
