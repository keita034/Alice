#pragma once
#include<AliceMathF.h>
#include<PhysicsShapeType.h>

namespace AlicePhysics
{
	enum PhysicsShapeType : uint8_t
	{
		SPHERE,//球
		BOX,//箱
		CAPSULE,//カプセル
		MESH,//メッシュ

		PHYSICSSHAPETYPE_COUNT
	};

	class IShape
	{	
	public:
		IShape() = default;
		virtual ~IShape() = default;

		/// <summary>
		/// 形を取得
		/// </summary>
		virtual AlicePhysics::PhysicsShapeType GetShapeType() = 0;

		virtual void* GetGetShape() = 0;

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="transform_">ワールド変換行列</param>
		/// <param name="scale_">スケール</param>
		/// <param name="inColor">カラー</param>
		/// <param name="wireframe_">ワイヤーフレームで描画するか</param>
		virtual void Draw(const AliceMathF::Matrix4& transform_,const AliceMathF::Vector3& scale_ = { 1.0f,1.0f ,1.0f },const AliceMathF::Vector4& inColor = { 1.0f ,1.0f ,1.0f ,1.0f },bool wireframe_ = false) = 0;
	};

}
