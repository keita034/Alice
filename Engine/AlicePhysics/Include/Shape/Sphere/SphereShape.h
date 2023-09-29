#pragma once
#include "IShape.h"

namespace AlicePhysics
{
	class SphereShape :public IShape
	{
	protected:
		//タイプ
		PhysicsShapeType type;

		//半径
		float radius;

	public:

		SphereShape() = default;
		virtual ~SphereShape() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(float radius_) = 0;

		/// <summary>
		/// タイプを取得
		/// </summary>
		AlicePhysics::PhysicsShapeType GetShapeType() override;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		virtual void* GetGetShape() override = 0;

		/// <summary>
		/// 半径を取得
		/// </summary>
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
	/// ボックスを生成
	/// </summary>
	/// <param name="radius_">半径</param>
	/// <returns></returns>
	SphereShape* CreateSphereShape(float radius_);
}

