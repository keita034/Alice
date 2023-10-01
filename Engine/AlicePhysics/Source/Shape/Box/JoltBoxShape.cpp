#include "JoltBoxShape.h"

AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltBoxShape::renderer =nullptr;

void* AlicePhysics::JoltBoxShape::GetGetShape()
{
    return static_cast<void*>(shape);
}

void AlicePhysics::JoltBoxShape::Initialize(const AliceMathF::Vector3& halfExtent_)
{
    shape = new JPH::BoxShape(halfExtent_);
    type = BOX;
    halfExtent = halfExtent_;
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
}

void AlicePhysics::JoltBoxShape::Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_)
{
	if ( renderer )
	{
		renderer->SetConstant(&constantBuffers);
		shape->Draw(renderer,transform_,scale_,inColor,false,wireframe_);
		renderer->SetConstant(nullptr);
	}
}

void AlicePhysics::JoltBoxShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
