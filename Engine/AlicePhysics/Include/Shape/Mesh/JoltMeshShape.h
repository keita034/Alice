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
	class JoltMeshShape final :public MeshShape
	{
	private:
		std::unique_ptr<JPH::MeshShape>shape;
		static JoltDebugRenderer* renderer;
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

		static void SetRenderer(JoltDebugRenderer* renderer_);
	};
}

