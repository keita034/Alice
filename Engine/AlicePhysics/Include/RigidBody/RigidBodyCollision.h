#pragma once
#include<RigidBodyUserData.h>

namespace AlicePhysics
{
	class RigidBodyCollision
	{
	public:

		/// <summary>
		/// 当たった瞬間に呼ばれる
		/// </summary>
		virtual void OnCollisionEnter(RigidBodyUserData* BodyData_) = 0;

		/// <summary>
		/// 当たってる時に呼ばれる
		/// </summary>
		virtual void OnCollisionStay(RigidBodyUserData* BodyData_) = 0;

		/// <summary>
		/// 離れた瞬間に呼ばれる
		/// </summary>
		virtual void OnCollisionExit() = 0;


		RigidBodyCollision() = default;
		virtual ~RigidBodyCollision() = default;
	};
}