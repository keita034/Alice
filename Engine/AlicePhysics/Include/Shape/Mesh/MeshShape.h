#pragma once

#include "IShape.h"

namespace AlicePhysics
{
	class MeshShape :public IShape
	{
	protected:
		//タイプ
		PhysicsShapeType type;

	public:

		MeshShape() = default;
		virtual ~MeshShape() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(const std::vector<AliceMathF::Vector3>& vertices_,const std::vector<uint32_t>& triangles_) = 0;

		/// <summary>
		/// タイプを取得
		/// </summary>
		PhysicsShapeType GetShapeType() override;

		/// <summary>
		/// シェイプを取得
		/// </summary>
		virtual void* GetGetShape() override = 0;

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
	/// <param name="vertices_">頂点配列</param>
	/// <param name="triangles_">インデックス配列</param>
	MeshShape* CreateMeshShape(const std::vector<AliceMathF::Vector3>& vertices_,const std::vector<uint32_t>& triangles_);
}

