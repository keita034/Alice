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

	animation = std::make_unique<BossAnimation>();
	animation->Initialize();

	actionManager = std::make_unique<BossActionManager>();
	actionManager->Initialize(animation.get());

	hands[static_cast<size_t>(BossHandIndex::LEFT)] = std::make_unique<BossHand>();
	hands[static_cast<size_t>(BossHandIndex::RIGHT)] = std::make_unique<BossHand>();
	hands[static_cast<size_t>(BossHandIndex::LEFT)]->Initialize(&transform);
	hands[static_cast<size_t>(BossHandIndex::RIGHT)]->Initialize(&transform);

	bossUI = std::make_unique<BossUI>();
	bossUI->Initialize();

	Reset();
}

void Boss::Update()
{
	oldTrans = transform.translation;

	actionManager->Update(player->GetPosition(), transform.translation);

	bossUI->SetHp(hp, MAX_HP);

	if (actionManager->GetinternalAction() == BossInternalAction::ATTACK)
	{
		situation |= ActorSituation::ATTACK;
	}
	else
	{
		if (situation & ActorSituation::ATTACK)
		{
			situation &= ~ActorSituation::ATTACK;
		}
	}

	//移動

	AliceMathF::Vector3 lMove = actionManager->GetDistanceTraveled();
	dynamicBody->setLinearVelocity(lMove);

	{
		physx::PxTransform lTransform;
		lTransform = dynamicBody->getGlobalPose();

		if (lTransform.q != pxTransform.q)
		{
			lTransform.q = pxTransform.q;
			dynamicBody->setGlobalPose(lTransform);
			transform.translation = GetGlobalPos() + -rigidBodyoffset;

			actionManager->SetBossPos(GetGlobalPos());
		}
	}

	if (AliceMathF::Vector3(0,0,0) != lMove)

	{
		direction = lMove.Normal();
		direction = -direction;
		direction = direction.Normal();

	}
	else
	{
		int b = 0;
		b++;
	}

	if (situation & ActorSituation::DAMAGE && !player->IsAttack())
	{
		situation &= ~ActorSituation::DAMAGE;
	}

	animation->Update();
	hands[static_cast<size_t>(BossHandIndex::RIGHT)]->Update("mixamorig:RightHand", animation->GetAnimation(), model.get());
	hands[static_cast<size_t>(BossHandIndex::LEFT)]->Update("mixamorig:LeftHand", animation->GetAnimation(), model.get());
	fireWorkParticle->Update();
	bossUI->Update();

	{
		uint32_t lBitTmp = userData.attribute & 0xfff;
		userData.attribute &= ~lBitTmp;
		userData.attribute |= situation;
		hands[static_cast<size_t>(BossHandIndex::RIGHT)]->SetSituation(situation);
		hands[static_cast<size_t>(BossHandIndex::LEFT)]->SetSituation(situation);
	}




}

void Boss::Draw()
{
	model->Draw(transform, animation->GetAnimation());

	bossUI->Draw();

	fireWorkParticle->Draw(camera);
}

void Boss::Finalize()
{
}

void Boss::Reset()
{
	hp = MAX_HP;
	bossUI->SetHp(hp, MAX_HP);
}

void Boss::TransUpdate(Camera* camera_)
{

	transform.LookAtMatrixAxisFix(direction, { 0,1,0 }, camera_);
	//transform.TransUpdate( camera_);
	hands[static_cast<size_t>(BossHandIndex::RIGHT)]->TransUpdate(camera_);
	hands[static_cast<size_t>(BossHandIndex::LEFT)]->TransUpdate(camera_);

	camera = camera_;
}

void Boss::OnTrigger(uint32_t attribute_)
{
	if (attribute_ >> 16 & CollisionAttribute::PLAYERWEAPON &&
		player->IsAttack() &&
		!(situation & ActorSituation::DAMAGE))
	{
		hp -= player->GetDamage();
		situation |= ActorSituation::DAMAGE;
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

int32_t Boss::GetHp()
{
	return hp;
}
