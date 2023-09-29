#pragma once
#include<IRigidBody.h>

class TestRigidBody :public AlicePhysics::RigidBodyCollision
{
public:

	/// <summary>
	/// 当たった瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionEnter()override;

	/// <summary>
	/// 当たってる時に呼ばれる
	/// </summary>
	virtual void OnCollisionStay()override;

	/// <summary>
	/// 離れた瞬間に呼ばれる
	/// </summary>
	virtual void OnCollisionExit()override;

	TestRigidBody() = default;
	~TestRigidBody() = default;
};

