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
	CreateShape(15.0f, 35.0f, CollisionAttribute::BOSS, (CollisionAttribute::PLAYER | CollisionAttribute::PLAYERWEAPON));
	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	SetInitializePos(pos);
	SetInitializeRot(AliceMathF::Quaternion({ 0,0,1 }, AliceMathF::DEG_TO_RAD * 90.0f));
	CreateRigidBody(RigidBodyType::DYNAMIC);

	fireWorkParticle = std::make_unique<FireWorkParticle>();
	fireWorkParticle->Initialize();
	fireWorkParticle->SetTex(TextureManager::SLoad("Resources/Default/Particle/effect1.png"));

	damageSE = audioManager->LoadAudio("Resources/SE/Damage.mp3", 0.3f);

}

void Boss::Update()
{
	dynamicBody->setGlobalPose(pxTransform);

	if (situation & BossSituation::DAMAGE && !player->IsAttack())
	{
		situation &= ~BossSituation::DAMAGE;
	}

	fireWorkParticle->Update();
}

void Boss::Draw()
{
	model->Draw(transform);

	fireWorkParticle->Draw(camera);
}

void Boss::Finalize()
{
}

void Boss::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_);
	camera = camera_;
}

void Boss::OnTrigger(uint32_t attribute_)
{
	if (attribute_ & CollisionAttribute::PLAYERWEAPON &&
		player->IsAttack() &&
		!(situation & BossSituation::DAMAGE))
	{
		hp -= player->GetDamage();
		situation |= BossSituation::DAMAGE;
		fireWorkParticle->Add(transform.translation + rigidBodyoffset);
		audioManager->PlayWave(damageSE);
	}
}

void Boss::SetPlayer(Player* player_)
{
	player = player_;
}

void Boss::SetAudioManager(IAudioManager* audioManager_)
{
	audioManager = audioManager_;
}
