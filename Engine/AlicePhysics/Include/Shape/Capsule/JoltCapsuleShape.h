#pragma once
#include <CapsuleShape.h>

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

ALICE_SUPPRESS_WARNINGS_END

#include<JoltDebugRenderer.h>

namespace AlicePhysics
{
	class JoltCapsuleShape final :public CapsuleShape
	{
	private:
		std::unique_ptr<JPH::CapsuleShape>shape;
		static JoltDebugRenderer* renderer;
	public:

		JoltCapsuleShape() = default;
		~JoltCapsuleShape() = default;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		void* GetGetShape() override;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(float halfHeightOfCylinder_,float radius_) override;

		void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_) override;

		static void SetRenderer(JoltDebugRenderer* renderer_);
	};
}

