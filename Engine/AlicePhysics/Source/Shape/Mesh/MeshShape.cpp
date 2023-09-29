#include "MeshShape.h"

#include<JoltMeshShape.h>

AlicePhysics::PhysicsShapeType AlicePhysics::MeshShape::GetShapeType()
{
    return type;
}

AlicePhysics::MeshShape* AlicePhysics::CreateMeshShape(const std::vector<AliceMathF::Vector3>& vertices_,const std::vector<uint32_t>& triangles_)
{
	MeshShape* lShape = nullptr;

	//バレット
#if AlicePhysicsBullet


#endif

	//フィジックス
#if AlicePhysicsPhysX


#endif

	//ジョルト
#if AlicePhysicsJolt

	lShape = new JoltMeshShape();
	lShape->Initialize(vertices_,triangles_);

#endif

	return lShape;
}
