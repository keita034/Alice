#include "PlayerGreatWeapon.h"

#include<CollisionAttribute.h>
#include<BoxShape.h>

void PlayerGreatWeapon::Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,AlicePhysics::IRigidBody* rigidParent_)
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Player/Weapon/GreatSword");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.Initialize();
	transform.parent = parent_;
	transform.translation = { 100.0f * 110,100.0f * 10,100.0f * -6.0f };
	transform.scale = { 120.0f,120.0f ,120.0f };
	transform.rotation = { 60.0f * AliceMathF::DEG_TO_RAD,0.0f,-90.0f * AliceMathF::DEG_TO_RAD };

	shape.reset(AlicePhysics::CreateBoxShape({ 2.0f,20.0f,3.0f }));

	{
		AlicePhysics::IRigidBodyCreationSettings lSetting;
		if ( rigidParent_ )
		{
			lSetting.parent = rigidParent_;
		}
		lSetting.name = "PlayerGreatWeapon";
		lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
		lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
		lSetting.allowSleeping = false;
		lSetting.collisionGroup = CollisionGroup::PLAYER;
		lSetting.collisionAttribute = CollisionAttribute::WEAPON;
		lSetting.rotation = AliceMathF::Quaternion(transform.rotation);
		lSetting.position = transform.translation;
		lSetting.shape = shape.get();
		lSetting.userData = &usData;
		lSetting.trigger = true;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
	}
}

void PlayerGreatWeapon::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void PlayerGreatWeapon::Update(const std::string& boneName_, AliceBlendTree* tree_,AliceModel* playerModel_)
{
	oldPos = AliceMathF::GetWorldPosition(transform.matWorld);

	animationTransform = playerModel_->GetAnimationTransform(tree_, boneName_);

}

void PlayerGreatWeapon::Update(const AliceMathF::Matrix4& mat_)
{
	animationTransform.boneMatrix = mat_;
}

void PlayerGreatWeapon::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_, &animationTransform.boneMatrix);

	AliceMathF::Vector3 pos = AliceMathF::GetWorldPosition(transform.matWorld);

	usData.velocity = pos- oldPos;
	usData.velocity = -usData.velocity.Normal();

	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
	rigidBody->SetPosition(AliceMathF::GetWorldPosition(transform.matWorld));
}

void PlayerGreatWeapon::Draw()
{
	//model->Draw(transform);
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,1.0f ,1.0f ,1.0f },true);
}

void PlayerGreatWeapon::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) {}
void PlayerGreatWeapon::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) {}
void PlayerGreatWeapon::OnCollisionExit() {}

const AliceMathF::Matrix4& PlayerGreatWeapon::GetWorldMat() const
{
	return transform.matWorld;
}

void PlayerGreatWeapon::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_){}
void PlayerGreatWeapon::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_){}
void PlayerGreatWeapon::Update() {};
