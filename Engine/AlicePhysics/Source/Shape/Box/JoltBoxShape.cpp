#include "JoltBoxShape.h"

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
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
#if _DEBUG && defined(JPH_DEBUG_RENDERER)
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
#endif // _DEBUG

}

void AlicePhysics::JoltBoxShape::Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_)
{
#if _DEBUG && defined(JPH_DEBUG_RENDERER)
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

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
void AlicePhysics::JoltBoxShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}

#endif // _DEBUG

