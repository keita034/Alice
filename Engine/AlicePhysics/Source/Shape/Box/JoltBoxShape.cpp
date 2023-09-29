#include "JoltBoxShape.h"

AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltBoxShape::renderer =nullptr;

void* AlicePhysics::JoltBoxShape::GetGetShape()
{
    return static_cast<void*>(shape.get());
}

void AlicePhysics::JoltBoxShape::Initialize(const AliceMathF::Vector3& halfExtent_)
{
    shape = std::make_unique<JPH::BoxShape>(halfExtent_);
    type = BOX;
    halfExtent = halfExtent_;
}

void AlicePhysics::JoltBoxShape::Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_)
{
	if ( renderer )
	{
		JPH::ColorArg lCol = inColor;

		shape->Draw(renderer,transform_,scale_,lCol,false,wireframe_);
	}
}

void AlicePhysics::JoltBoxShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
