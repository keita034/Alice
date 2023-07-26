#include "PlayerWeapon.h"

void PlayerWeapon::Initialize(Transform* parent_)
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Player/Weapon/katana");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.Initialize();
	transform.parent = parent_;
	transform.translation = { 10.0f,5.0f,-0.6f };
	transform.scale = { 0.7f,0.7f ,0.7f };
	transform.rotation = { 70.0f * AliceMathF::DEG_TO_RAD,0.0f,-90.0f * AliceMathF::DEG_TO_RAD };
}

void PlayerWeapon::Finalize()
{
}

void PlayerWeapon::Update(const std::string& boneName_, AliceBlendTree* tree_,AliceModel* playerModel_)
{
	boneMat = playerModel_->GetAnimationTransform(tree_, boneName_);
}

void PlayerWeapon::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_, &boneMat);
}

void PlayerWeapon::Draw()
{
	model->Draw(transform);
}

void PlayerWeapon::Initialize(uint32_t handle_, const AliceMathF::Vector3& pos_, const AliceMathF::Vector3& rot_, const AliceMathF::Vector3& scl_, const Transform* parent_){}
void PlayerWeapon::Initialize(){}
void PlayerWeapon::Update() {};
