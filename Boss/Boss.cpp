#include "Boss.h"

#include<CollisionAttribute.h>

void Boss::Initialize()
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Boss");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.translation = { 0.0f,0.0f,10.0f };
	transform.Initialize();

	rigidBodyoffset = { 0.0f, 15.0f + 35.0f, 0.0f };

	CreateMaterial(0.6f, 0.6f, 0.0f);
	CreateShape(15.0f, 35.0f, CollisionAttribute::BOSS, CollisionAttribute::PLAYER);
	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	SetInitializePos(pos);
	SetInitializeRot(AliceMathF::Quaternion({ 0,0,1 }, AliceMathF::DEG_TO_RAD * 90.0f));
	CreateRigidBody(RigidBodyType::DYNAMIC);

}

void Boss::Update()
{
	dynamicBody->setGlobalPose(pxTransform);
}

void Boss::Draw()
{
	model->Draw(transform);
}

void Boss::Finalize()
{
}

void Boss::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_);
}
