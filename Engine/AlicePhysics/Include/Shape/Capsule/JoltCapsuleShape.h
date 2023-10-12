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
		JPH::CapsuleShape* shape;
#if _DEBUG && defined(JPH_DEBUG_RENDERER)
		std::vector<std::unique_ptr<IConstantBuffer>>constantBuffers;
		static JoltDebugRenderer* renderer;
#endif // _DEBUG

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

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
		static void SetRenderer(JoltDebugRenderer* renderer_);
#endif // _DEBUG

	};
}

