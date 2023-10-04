#pragma once
#include "IShape.h"

namespace AlicePhysics
{
	class BoxShape :public IShape
	{
	protected:

		//辺の半分の長さ
		AliceMathF::Vector3 halfExtent;
		//タイプ
		PhysicsShapeType type;

	private:
		int8_t PADING[ 3 ];
	public:

		BoxShape() = default;
		virtual ~BoxShape() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(const AliceMathF::Vector3& halfExtent_) = 0;

		/// <summary>
		/// タイプを取得
		/// </summary>
		PhysicsShapeType GetShapeType() override;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		virtual void* GetGetShape() override = 0;

		/// <summary>
		/// 辺の半分の長さを取得
		/// </summary>
		const AliceMathF::Vector3& GetHalfExtent()const;

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="transform_">ワールド変換行列</param>
		/// <param name="scale_">スケール</param>
		/// <param name="inColor">カラー</param>
		/// <param name="wireframe_">ワイヤーフレームで描画するか</param>
		virtual void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_,const AliceMathF::Vector4& inColor ,bool wireframe_)override = 0;
	};

	/// <summary>
	/// ボックスを生成
	/// </summary>
	/// <param name="halfExtent_">辺の半分の長さ</param>
	BoxShape* CreateBoxShape(const AliceMathF::Vector3& halfExtent_);
}

