#include "JoltCapsuleShape.h"

AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltCapsuleShape::renderer = nullptr;

void* AlicePhysics::JoltCapsuleShape::GetGetShape()
{
	return static_cast< void* >( shape );
}

void AlicePhysics::JoltCapsuleShape::Initialize(float halfHeightOfCylinder_,float radius_)
{
	shape = new JPH::CapsuleShape(halfHeightOfCylinder_,radius_);
	type = CAPSULE;
	halfHeightOfCylinder = halfHeightOfCylinder_;
	radius = radius_;

	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
}

void AlicePhysics::JoltCapsuleShape::Draw(const AliceMathF::Matrix4& transform_, const AliceMathF::Vector3& scale_, const AliceMathF::Vector4& inColor, bool wireframe_)
{
	if (renderer)
	{
		renderer->SetConstant(&constantBuffers);
		shape->Draw(renderer, transform_, scale_, inColor, false, wireframe_);
		renderer->SetConstant(nullptr);
	}
}

void AlicePhysics::JoltCapsuleShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
