#include "PlayerWeapon.h"

#include<CollisionAttribute.h>
#include<BoxShape.h>

void PlayerWeapon::Initialize(Transform* parent_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,AlicePhysics::IRigidBody* rigidParent_)
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Player/Weapon/katana");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.Initialize();
	transform.parent = parent_;
	transform.translation = { 100.0f * 50,100.0f * 10,100.0f * -6.0f };
	transform.scale = { 120.0f,120.0f ,120.0f };
	transform.rotation = { 60.0f * AliceMathF::DEG_TO_RAD,0.0f,-90.0f * AliceMathF::DEG_TO_RAD };

	shape.reset(AlicePhysics::CreateBoxShape({ 2.0f,12.0f,3.0f }));

	{
		AlicePhysics::IRigidBodyCreationSettings lSetting;
		if ( rigidParent_ )
		{
			lSetting.parent = rigidParent_;
		}
		lSetting.name = "PlayerWeapon";
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

void PlayerWeapon::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
}

void PlayerWeapon::Update(const std::string& boneName_, AliceBlendTree* tree_,AliceModel* playerModel_)
{
	oldPos = AliceMathF::GetWorldPosition(transform.matWorld);

	animationTransform = playerModel_->GetAnimationTransform(tree_, boneName_);

}

void PlayerWeapon::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_, &animationTransform.boneMatrix);

	AliceMathF::Vector3 pos = AliceMathF::GetWorldPosition(transform.matWorld);

	usData.velocity = pos- oldPos;
	usData.velocity = -usData.velocity.Normal();

	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
	rigidBody->SetPosition(AliceMathF::GetWorldPosition(transform.matWorld));
}

void PlayerWeapon::Draw()
{
	model->Draw(transform);
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,1.0f ,1.0f ,1.0f },true);
}

void PlayerWeapon::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) {}
void PlayerWeapon::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_) {}
void PlayerWeapon::OnCollisionExit() {}

void PlayerWeapon::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_){}
void PlayerWeapon::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_){}
void PlayerWeapon::Update() {};
