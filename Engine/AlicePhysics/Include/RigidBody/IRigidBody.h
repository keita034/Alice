#pragma once
#include<AliceMathF.h>

#include<RigidBodyCollision.h>
#include<RigidBodyUserData.h>
#include<IRigidBodyCreationSettings.h>

namespace AlicePhysics
{
	class IRigidBody
	{
	public:
		virtual void* GetBody()  = 0;

		/// <summary>
		/// 当たった瞬間に呼ばれる
		/// </summary>
		virtual void OnCollisionEnter(RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) = 0;

		/// <summary>
		/// 当たってる時に呼ばれる
		/// </summary>
		virtual void OnCollisionStay(RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) = 0;

		/// <summary>
		/// 離れた瞬間に呼ばれる
		/// </summary>
		virtual void OnCollisionExit() = 0;

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
		virtual float GetRestitution() = 0;

		/// <summary>
		/// 線速度を取得（単位：m/s）
		/// </summary>
		virtual AliceMathF::Vector3 GetLinearVelocity() = 0;

		/// <summary>
		/// 角速度を取得（単位：rad/s）
		/// </summary>
		virtual AliceMathF::Vector3 GetAngularVelocity() = 0;

		/// <summary>
		/// スケールを抜いた中心のワールド行列
		/// </summary>
		virtual AliceMathF::Matrix4 GetCenterOfMassTransform() = 0;

		/// <summary>
		/// このボディが現在アクティブか
		/// </summary>
		virtual bool IsActive() = 0;

		/// <summary>
		/// 当たり判定用の関数追加
		/// </summary>
		virtual void SetRigidBodyCollision(RigidBodyCollision* rigidBodyCollision_) = 0;

		/// <summary>
		/// 座標と回転角を設定
		/// </summary>
		/// <param name="position_">座標</param>
		/// <param name="rotation_">回転角</param>
		virtual void SetPositionAndRotationInternal(const AliceMathF::Vector3& position_,const AliceMathF::Quaternion& rotation_) = 0;

		/// <summary>
		/// 座標を設定
		/// </summary>
		virtual void SetPosition(const AliceMathF::Vector3& position_) = 0;

		/// <summary>
		/// 回転角を設定
		/// </summary>
		virtual void SetRotation(const AliceMathF::Quaternion& rotation_) = 0;

		virtual void SetMatrix(const AliceMathF::Matrix4& matRigidBody_,const AliceMathF::Matrix4& matWorld_) = 0;

		virtual void AddForce(const AliceMathF::Vector3& force_) = 0;

		IRigidBody() = default;
		virtual ~IRigidBody() = default;
	};
}
