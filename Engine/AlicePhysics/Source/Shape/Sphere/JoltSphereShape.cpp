#include "JoltSphereShape.h"

AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltSphereShape::renderer = nullptr;


void* AlicePhysics::JoltSphereShape::GetGetShape()
{
	return static_cast<void*>(shape.get());
}

void AlicePhysics::JoltSphereShape::Initialize(float radius_)
{
	JPH::SphereShape* lSphere = new JPH::SphereShape(radius_);

	shape.reset(lSphere);
	type = SPHERE;
	radius = radius_;
}

void AlicePhysics::JoltSphereShape::Draw(const AliceMathF::Matrix4& transform_, const AliceMathF::Vector3& scale_, const AliceMathF::Vector4& inColor, bool wireframe_)
{
	if (renderer)
	{
		shape->Draw(renderer, transform_, scale_, inColor, false, wireframe_);
	}
}

void AlicePhysics::JoltSphereShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
