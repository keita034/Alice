#pragma once
#include<MeshShape.h>

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

ALICE_SUPPRESS_WARNINGS_END

#include<JoltDebugRenderer.h>

namespace AlicePhysics
{
	class JoltMeshShape : public MeshShape
	{
	private:
		JPH::MeshShape* shape;
		JPH::Shape::ShapeResult lShapeResult;
#if _DEBUG && defined(JPH_DEBUG_RENDERER)
		static JoltDebugRenderer* renderer;
		std::vector<std::unique_ptr<IConstantBuffer>>constantBuffers;
#endif // _DEBUG

	public:

		JoltMeshShape() = default;
		~JoltMeshShape() = default;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		void* GetGetShape() override;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(const std::vector<AliceMathF::Vector3>& vertices_,const std::vector<uint32_t>& triangles_) override;

		void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_) override;

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
		static void SetRenderer(JoltDebugRenderer* renderer_);
#endif // _DEBUG

	};
}

