#pragma once

#include "IShape.h"

namespace AlicePhysics
{
	class CapsuleShape :public IShape
	{
	protected:

		//中心から先の円までの長さ
		float halfHeightOfCylinder;

		//半径
		float radius;

		//タイプ
		PhysicsShapeType type;

	private:
		int8_t PADING[ 7 ];

	public:

		CapsuleShape() = default;
		virtual ~CapsuleShape() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(float halfHeightOfCylinder_,float radius_) = 0;

		/// <summary>
		/// タイプを取得
		/// </summary>
		PhysicsShapeType GetShapeType() override;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		virtual void* GetGetShape() override = 0;

		/// <summary>
		/// 中心から先の円までの長さを取得
		/// </summary>
		float GetHalfHeightOfCylinder()const;

		//直径
		float GetRadius()const;

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="transform_">ワールド変換行列</param>
		/// <param name="scale_">スケール</param>
		/// <param name="inColor">カラー</param>
		/// <param name="wireframe_">ワイヤーフレームで描画するか</param>
		virtual void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_ = { 1.0f,1.0f ,1.0f },const AliceMathF::Vector4& inColor = { 1.0f ,1.0f ,1.0f ,1.0f },bool wireframe_ = false) = 0;
	};

	/// <summary>
	/// カプセルを生成
	/// </summary>
	/// <param name="halfHeightOfCylinder_">中心から先の円までの長さ</param>
	/// <param name="radius_">半径</param>
	CapsuleShape* CreateCapsuleShape(float halfHeightOfCylinder_,float radius_);
}
