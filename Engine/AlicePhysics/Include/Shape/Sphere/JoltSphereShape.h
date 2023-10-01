#pragma once
#include "SphereShape.h"

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/SphereShape.h>

ALICE_SUPPRESS_WARNINGS_END

#include<JoltDebugRenderer.h>

namespace AlicePhysics
{
	class JoltSphereShape final :public SphereShape
	{
	private:
		JPH::SphereShape*shape;
		std::vector<std::unique_ptr<IConstantBuffer>>constantBuffers;
		static JoltDebugRenderer* renderer;
	public:

		JoltSphereShape() = default;
		~JoltSphereShape() = default;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		void* GetGetShape() override;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(float radius_) override;

		void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_) override;

		static void SetRenderer(JoltDebugRenderer* renderer_);
	};
}