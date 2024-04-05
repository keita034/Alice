#include "BossSword.h"

#include<CollisionAttribute.h>
#include<BoxShape.h>

void BossSword::Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,AlicePhysics::IRigidBody* rigidParent_)
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Boss/Weapon/Sword");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.Initialize();
	transform.parent = parent_;
	transform.translation = { 100.0f * 50,100.0f * 25,100.0f * -6.0f };
	transform.scale = { 1200.0f,1200.0f ,1200.0f };
	transform.rotation = { 60.0f * AliceMathF::DEG_TO_RAD,0.0f,-90.0f * AliceMathF::DEG_TO_RAD };

	shape.reset(AlicePhysics::CreateBoxShape({ 3.0f,70.0f,2.0f }));

	{
		AlicePhysics::IRigidBodyCreationSettings lSetting;
		if ( rigidParent_ )
		{
			lSetting.parent = rigidParent_;
		}
		lSetting.name = "BossWeapon";
		lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
		lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
		lSetting.allowSleeping = false;
		lSetting.collisionGroup = CollisionGroup::BOSS;
		lSetting.collisionAttribute = CollisionAttribute::WEAPON;
		lSetting.rotation = AliceMathF::Quaternion(transform.rotation);
		lSetting.position = transform.translation;
		lSetting.shape = shape.get();
		lSetting.trigger = true;
		lSetting.userData = &bossSwordUsData;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
	}
}

void BossSword::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void BossSword::Update(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_)
{
	if ( isUpdate )
	{
		animationTransform = bossModel_->GetAnimationTransform(tree_,boneName_);
	}
}


void BossSword::Draw()
{
	if ( isUpdate )
	{
		model->Draw(transform);
		shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,1.0f ,1.0f ,1.0f },true);
	}
}

void BossSword::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
}

void BossSword::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
}

void BossSword::OnCollisionExit()
{
}

void BossSword::TransUpdate(Camera* camera_)
{
	bossSwordUsData.isAttack = isUpdate;

	transform.TransUpdate(camera_,&animationTransform.boneMatrix);

	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
	rigidBody->SetPosition(AliceMathF::GetWorldPosition(transform.matWorld));
}

void BossSword::SetIsUpdate(bool flag_)
{
	isUpdate = flag_;
}

const AliceMathF::Matrix4& BossSword::GetWorldMat()
{
	return transform.matWorld;
}
