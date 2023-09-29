#include "JoltMeshShape.h"

#include<AliceAssert.h>

AlicePhysics::JoltDebugRenderer* AlicePhysics::JoltMeshShape::renderer = nullptr;

void* AlicePhysics::JoltMeshShape::GetGetShape()
{
	return static_cast< void* >( shape.get() );
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

	JPH::MeshShapeSettings lSetting(lVertices,lTriangles);
	JPH::Shape::ShapeResult lShapeResult;
	shape = std::make_unique<JPH::MeshShape>(lSetting,lShapeResult);

	type = MESH;

}

void AlicePhysics::JoltMeshShape::Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_)
{
	if ( renderer )
	{
		shape->Draw(renderer,transform_,scale_,inColor,false,wireframe_);
	}
}

void AlicePhysics::JoltMeshShape::SetRenderer(JoltDebugRenderer* renderer_)
{
	renderer = renderer_;
}
