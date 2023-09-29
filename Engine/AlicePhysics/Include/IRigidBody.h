#pragma once
#include<AliceMathF.h>

#include<RigidBodyCollision.h>

namespace AlicePhysics
{
	class IRigidBody
	{
	public:
		virtual void* GetBody()  = 0;

		/// <summary>
		/// 当たった瞬間に呼ばれる
		/// </summary>
		virtual void OnCollisionEnter();

		/// <summary>
		/// 当たってる時に呼ばれる
		/// </summary>
		virtual void OnCollisionStay();

		/// <summary>
		/// 離れた瞬間に呼ばれる
		/// </summary>
		virtual void OnCollisionExit();

		/// <summary>
		/// 空間線速度を設定
		/// </summary>
		virtual void SetLinearVelocity(const AliceMathF::Vector3& inLinearVelocity_) = 0;

		/// <summary>
		/// 座標を取得
		/// </summary>
		virtual AliceMathF::Vector3 GetPosition() = 0;

		/// <summary>
		/// 回転角を取得
		/// </summary>
		/// <returns></returns>
		virtual AliceMathF::Quaternion GetRotation() = 0;

		/// <summary>
		/// 摩擦係数を取得
		/// </summary>
		virtual float GetFriction() = 0;

		/// <summary>
		/// 反発係数を取得
		/// </summary>
		/// <returns></returns>
		virtual float GetRestitution() = 0;

		/// <summary>
		/// 線速度を取得（単位：m/s）
		/// </summary>
		virtual AliceMathF::Vector3 GetLinearVelocity() = 0;

		/// <summary>
		/// 角速度を取得（単位：rad/s）
		/// </summary>
		virtual AliceMathF::Vector3 GetAngularVelocity() = 0;

		virtual AliceMathF::Matrix4 GetCenterOfMassTransform() = 0;

		/// <summary>
		/// このボディが現在アクティブか
		/// </summary>
		/// <returns></returns>
		virtual bool IsActive() = 0;

		/// <summary>
		/// 当たり判定用の関数追加
		/// </summary>
		virtual void SetRigidBodyCollision(RigidBodyCollision* rigidBodyCollision_) = 0;

		IRigidBody() = default;
		virtual ~IRigidBody() = default;
	};
}
