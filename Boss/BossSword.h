#pragma once

#include<GameObject.h>

struct BossSwordUsData
{
	bool isAttack = false;
};

class BossSword : public GameObject
{
private:
	AliceModel::AnimationTransform animationTransform;
	bool isUpdate = false;
	BossSwordUsData bossSwordUsData;
public:

	void Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,AlicePhysics::IRigidBody* rigidParent_ = nullptr);

	void Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_) override;

	void Update(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_);

	void Draw() override;

	void OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) override;

	void OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) override;

	void OnCollisionExit() override;

	void TransUpdate(Camera* camera_)override;

	void SetIsUpdate(bool flag_);

	const AliceMathF::Matrix4& GetWorldMat();

private:

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_,const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& rot_,const AliceMathF::Vector3& scl_,const Transform* parent_) override {};

	void Update() override {};

	void Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_) override {};

};

