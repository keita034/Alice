#include "JoltBoxShape.h"

#ifdef _DEBUG
AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltBoxShape::renderer =nullptr;
#endif // _DEBUG


void* AlicePhysics::JoltBoxShape::GetGetShape()
{
    return static_cast<void*>(shape);
}

void AlicePhysics::JoltBoxShape::Initialize(const AliceMathF::Vector3& halfExtent_)
{
    shape = new JPH::BoxShape(halfExtent_);
    type = BOX;
    halfExtent = halfExtent_;
#ifdef _DEBUG
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
#endif // _DEBUG

}

void AlicePhysics::JoltBoxShape::Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_)
{
#ifdef _DEBUG
	if ( renderer )
	{
		renderer->SetConstant(&constantBuffers);
		shape->Draw(renderer,transform_,scale_,inColor,false,wireframe_);
		renderer->SetConstant(nullptr);
	}
#else

	static_cast< void >( transform_ );
	static_cast< void >( scale_ );
	static_cast< void >( inColor );
	static_cast< void >( wireframe_ );

#endif // _DEBUG

}

#ifdef _DEBUG
void AlicePhysics::JoltBoxShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}

#endif // _DEBUG

