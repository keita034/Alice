#include "JoltSphereShape.h"

#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltSphereShape::renderer = nullptr;
#endif // _DEBUG


void* AlicePhysics::JoltSphereShape::GetGetShape()
{
	return static_cast<void*>(shape);
}

void AlicePhysics::JoltSphereShape::Initialize(float radius_)
{
	shape = new JPH::SphereShape(radius_);
	type = SPHERE;
	radius = radius_;

#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
#endif // _DEBUG

}

void AlicePhysics::JoltSphereShape::Draw(const AliceMathF::Matrix4& transform_, const AliceMathF::Vector3& scale_, const AliceMathF::Vector4& inColor, bool wireframe_)
{
#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)

	if (renderer)
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

#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
void AlicePhysics::JoltSphereShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
#endif // _DEBUG

