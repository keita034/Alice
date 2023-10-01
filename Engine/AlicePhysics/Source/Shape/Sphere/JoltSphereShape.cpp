#include "JoltSphereShape.h"

AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltSphereShape::renderer = nullptr;


void* AlicePhysics::JoltSphereShape::GetGetShape()
{
	return static_cast<void*>(shape);
}

void AlicePhysics::JoltSphereShape::Initialize(float radius_)
{
	shape = new JPH::SphereShape(radius_);
	type = SPHERE;
	radius = radius_;

	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
}

void AlicePhysics::JoltSphereShape::Draw(const AliceMathF::Matrix4& transform_, const AliceMathF::Vector3& scale_, const AliceMathF::Vector4& inColor, bool wireframe_)
{
	if (renderer)
	{
		renderer->SetConstant(&constantBuffers);
		shape->Draw(renderer,transform_,scale_,inColor,false,wireframe_);
		renderer->SetConstant(nullptr);
	}
}

void AlicePhysics::JoltSphereShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
