#pragma once

#include<AliceMathF.h>
#include<BossAnimation.h>
#include<LaserGPUParticle.h>
#include<Transform.h>
#include<GameObject.h>

struct BossBeamUsData
{
	float distance = 0.0f;
	AliceMathF::Vector3 pos;
	bool isFinish;
};

class BossBeamAttack : public GameObject
{
private:

	BossAnimation* bossAnimation = nullptr;
	LaserGPUParticle* laserGPUParticle = nullptr;
	AlicePhysics::AlicePhysicsSystem* physicsSystem = nullptr;

	float rigidHalfHeight = 1000.0f;
	float rigidRadius = 50.0f;
	float time = 0.0f;

	Transform firePosition;
	Transform rigidBodyTransform;

	int32_t particleIndex;

	bool isFinish = false;
	bool isFire = false;

	AliceMathF::Vector3 direction = { 0,0,1 };

	BossBeamUsData usData;

public:

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_,LaserGPUParticle* laserGPUParticle_,Transform* parent_);

	void Update();

	bool IsFinish();

	void End();

	void Fire();

	const AliceMathF::Vector3& GetDirection() const;

	void TransUpdate(Camera* camera_)override;

	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override;

	void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;
	void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)override;
	void OnCollisionExit() override;
	void Draw()override;

	BossBeamAttack() = default;
	~BossBeamAttack() = default;

private:
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& rot_,const AliceMathF::Vector3& scl_,const Transform* parent_)override {};
	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)override {};
};

