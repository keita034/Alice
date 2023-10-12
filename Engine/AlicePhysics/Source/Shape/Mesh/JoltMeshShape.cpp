#include "JoltMeshShape.h"

#include<AliceAssert.h>

#include <Jolt/Physics/Collision/PhysicsMaterialSimple.h>

#ifdef _DEBUG
AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltMeshShape::renderer = nullptr;
#endif // _DEBUG


void* AlicePhysics::JoltMeshShape::GetGetShape()
{
	return static_cast< void* >( shape);
}

void AlicePhysics::JoltMeshShape::Initialize(const std::vector<AliceMathF::Vector3>& vertices_,const std::vector<uint32_t>& triangles_)
{
	JPH::VertexList lVertices;
	for ( AliceMathF::Vector3 vertice : vertices_ )
	{
		JPH::Float3 vert = { vertice.x,vertice.y,vertice.z };
		lVertices.push_back(vert);
	}
	AliceAssert(lVertices.size() == vertices_.size(),"頂点の数が違います");

	JPH::IndexedTriangleList lTriangles;
	for ( size_t i = 0; i < triangles_.size(); i += 3 )
	{
		JPH::IndexedTriangle triangle = { triangles_[ i ],triangles_[ i + 1 ],triangles_[ i + 2 ] };
		lTriangles.push_back(triangle);
	}
	AliceAssert(lTriangles.size() == triangles_.size() / 3,"インデックの数が違います");

	JPH::PhysicsMaterialList materials;

	materials.push_back(new JPH::PhysicsMaterialSimple("DefaultMaterial ",JPH::Color::sGrey));

	JPH::MeshShapeSettings lSetting(lVertices,lTriangles,materials);
	shape = new JPH::MeshShape(lSetting,lShapeResult);

	type = MESH;

#ifdef _DEBUG
	constantBuffers.push_back(CreateUniqueConstantBuffer(sizeof(AlicePhysics::JoltDebugRenderer::ConstBufferData)));
#endif // _DEBUG


}

void AlicePhysics::JoltMeshShape::Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_)
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
void AlicePhysics::JoltMeshShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
#endif // _DEBUG

