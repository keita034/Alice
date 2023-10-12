#pragma once
#include <BoxShape.h>

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

ALICE_SUPPRESS_WARNINGS_END

#include<JoltDebugRenderer.h>

namespace AlicePhysics
{
	class JoltBoxShape final :public BoxShape
	{
	private:
		JPH::BoxShape*shape;
#if _DEBUG && defined(JPH_DEBUG_RENDERER)
		static JoltDebugRenderer* renderer;
		std::vector<std::unique_ptr<IConstantBuffer>>constantBuffers;
#endif // _DEBUG

	public:

		JoltBoxShape() = default;
		~JoltBoxShape() = default;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		void* GetGetShape() override;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(const AliceMathF::Vector3& halfExtent_) override;

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="transform_">ワールド変換行列</param>
		/// <param name="scale_">スケール</param>
		/// <param name="inColor">カラー</param>
		/// <param name="wireframe_">ワイヤーフレームで描画するか</param>
		void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor,bool wireframe_) override;

#if _DEBUG && defined(JPH_DEBUG_RENDERER)
		static void SetRenderer(JoltDebugRenderer* renderer_);
#endif // _DEBUG

	};
}
