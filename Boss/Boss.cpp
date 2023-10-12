#include "Boss.h"

#include<CollisionAttribute.h>
#include<CapsuleShape.h>

void Boss::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Boss");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.translation = { 0.0f,0.0f,10.0f };
	transform.Initialize();

	rigidBodyoffset = { 4.0f, 15.0f + 35.0f, 0.0f };

	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	shape.reset(AlicePhysics::CreateCapsuleShape(35.0f,15.0f));

	AlicePhysics::IRigidBodyCreationSettings lSetting;
	lSetting.name = "Boss";
	lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
	lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
	lSetting.allowSleeping = false;
	lSetting.collisionGroup = CollisionGroup::BOSS;
	lSetting.collisionAttribute = CollisionAttribute::BODY;
	lSetting.position = pos;
	lSetting.shape = shape.get();
	lSetting.userData = static_cast< void* >( &situation );
	physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
	physicsSystem_->AddRigidBody(rigidBody);
	rigidBody->SetRigidBodyCollision(this);

	fireWorkParticle = std::make_unique<FireWorkParticle>();
	fireWorkParticle->Initialize();
	fireWorkParticle->SetTex(TextureManager::SLoad("Resources/Default/Particle/effect1.png"));

	damageSE = audioManager->LoadAudio("Resources/SE/Damage.mp3",0.3f);
	deathSE = audioManager->LoadAudio("Resources/SE/BossDeath.mp3");

	animation = std::make_unique<BossAnimation>();
	animation->Initialize();

	actionManager = std::make_unique<BossActionManager>();
	actionManager->Initialize(animation.get());

	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ] = std::make_unique<BossHand>();
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ] = std::make_unique<BossHand>();
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Initialize(&transform,physicsSystem_);
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Initialize(&transform,physicsSystem_);

	bossUI = std::make_unique<BossUI>();
	bossUI->Initialize();

	Reset();
}

void Boss::Update()
{
	oldTrans = transform.translation;

	if ( hp > 0 )
	{
		actionManager->Update(player->GetPosition(),transform.translation);
	}

	bossUI->SetHp(hp,MAX_HP);

	if ( hp > 0 )
	{
		if ( actionManager->GetinternalAction() == BossInternalAction::ATTACK )
		{
			situation |= ActorSituation::ATTACK;
		}
		else
		{
			if ( situation & ActorSituation::ATTACK )
			{
				situation &= ~ActorSituation::ATTACK;
			}
		}

		//移動

		AliceMathF::Vector3 lMove = actionManager->GetDistanceTraveled();
		rigidBody->SetLinearVelocity(lMove);

		{
			rigidBody->SetRotation(AliceMathF::Quaternion());

			transform.translation = rigidBody->GetPosition() + -rigidBodyoffset;
			actionManager->SetBossPos(rigidBody->GetPosition());
		}

		if ( AliceMathF::Vector3(0,0,0) != lMove )

		{
			direction = lMove.Normal();
			direction = -direction;
			direction = direction.Normal();

		}
	}

	if ( situation & ActorSituation::DAMAGE && !player->IsAttack() )
	{
		situation &= ~ActorSituation::DAMAGE;
	}

	animation->Update();
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Update("mixamorig:RightHandThumb1",animation->GetAnimation(),model.get());
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Update("mixamorig:LeftHandThumb1",animation->GetAnimation(),model.get());
	fireWorkParticle->Update();
	bossUI->Update();

	{
		hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->SetSituation(situation);
		hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->SetSituation(situation);
	}

}

void Boss::Draw()
{
	model->Draw(transform,animation->GetAnimation());
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,0.0f ,0.0f ,1.0f },true);
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Draw();
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Draw();
	fireWorkParticle->Draw(camera);
}

void Boss::UIDraw()
{
	bossUI->Draw();
}

void Boss::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Finalize(physicsSystem_);
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Finalize(physicsSystem_);

	physicsSystem_->RemoveRigidBody(rigidBody);
}

void Boss::Reset()
{
	hp = MAX_HP;
	bossUI->SetHp(hp,MAX_HP);
}

void Boss::TransUpdate(Camera* camera_)
{

	transform.LookAtMatrixAxisFix(direction,{ 0,1,0 },camera_);
	//transform.TransUpdate( camera_);
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->TransUpdate(camera_);
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->TransUpdate(camera_);

	camera = camera_;
}

void Boss::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_)
{
	if ( BodyData_->GetGroup() == CollisionGroup::PLAYER &&
		BodyData_->GetAttribute() == CollisionAttribute::WEAPON &&
		player->IsAttack() &&
		!( situation & ActorSituation::DAMAGE ) )
	{
		if ( hp > 0 )
		{
			for ( int32_t i = 0; i < player->GetDamage(); i++ )
			{
				if ( hp <= 0 )
				{
					break;
				}

				hp--;
			}

			situation |= ActorSituation::DAMAGE;
			fireWorkParticle->Add(transform.translation + rigidBodyoffset);
			audioManager->PlayWave(damageSE);

			if ( hp <= 0 )
			{
				animation->InsertDeathAnimation();
				animation->AnimationEndStop();
				audioManager->PlayWave(deathSE);
				audioManager->ChangeVolume(deathSE,deathSEVolume);
			}
		}
	}
}

void Boss::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_)
{
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

bool Boss::IsEnd()
{
	return hp <= 0 && !animation->IsInsert();
}

void Boss::AnimationStop()
{
	animation->AnimationStop();
}

void Boss::AnimationEndStop()
{
	animation->AnimationEndStop();

}

void Boss::DeathSEChangeVolume(float volume_)
{
	audioManager->ChangeVolume(deathSE,deathSEVolume * volume_);
}

void Boss::OnCollisionExit()
{
}
