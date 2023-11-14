#include "Player.h"

#include<GameCameraManager.h>
#include<PlayerGameCamera.h>
#include<GameCollisionConflg.h>
#include<CollisionAttribute.h>
#include<CapsuleShape.h>
#include<BossHand.h>
#include<BossJumpAttackMove.h>
#include<BossBeamAttack.h>

void Player::Initialize(AliceInput::IInput* input_,IAudioManager* audioManager_,AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	assert(input_);
	assert(audioManager_);

	input = input_;
	audioManager = audioManager_;
	deviceInput = std::make_unique<DeviceInput>();
	deviceInput->Initialize(input_);

	modelHandle = AliceModel::SCreateModel("Resources/Model/Player");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.translation = { 0.0f,0.0f,-110.0f };

	transform.Initialize();
	transform.scale = { scale,scale ,scale };

	transform.rotation = { 0.0f,AliceMathF::DEG_TO_RAD * 180.0f,0.0f };

	ui = std::make_unique<PlayerUI>();
	ui->Initialize();

	animation = std::make_unique<PlayerAnimation>();
	animation->Initialize(input);

	Reset();

	rigidBodyoffset = { 1.5f, 12.0f + 5.0f, 0.0f };
	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	shape.reset(AlicePhysics::CreateCapsuleShape(12.0f,5.0f));

	AlicePhysics::IRigidBodyCreationSettings lSetting;
	lSetting.name = "Player";
	lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
	lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
	lSetting.allowSleeping = false;
	lSetting.collisionGroup = CollisionGroup::PLAYER;
	lSetting.collisionAttribute = CollisionAttribute::BODY;
	lSetting.position = pos;
	lSetting.shape = shape.get();
	lSetting.userData = static_cast< void* >( &usData );
	physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
	physicsSystem_->AddRigidBody(rigidBody);
	rigidBody->SetRigidBodyCollision(this);

	weapon = std::make_unique<PlayerWeapon>();
	weapon->Initialize(&transform,physicsSystem_,rigidBody);

	deathSE = audioManager->LoadAudio("Resources/SE/PlayerDeath.mp3");

	animation->Update();
}

void Player::Update(BaseGameCamera* camera_,GameCameraManager::CameraIndex index_)
{
	if ( !shockwaveHit )
	{
		if (  usData.situation & ActorSituation::SHOCKWAVE_DAMAGE )
		{
			usData.situation &= ~ActorSituation::SHOCKWAVE_DAMAGE;
		}
	}

	shockwaveHit = false;

	oldTrans = transform.translation;
	isStationary = true;

	if ( fieldHit && !( usData.situation & ActorSituation::ROWLING ) )
	{
		fieldHit = false;

		AliceMathF::Vector3 pos = rigidBody->GetPosition() + rigidBodyoffset;
		rigidBody->SetRotation(AliceMathF::Quaternion());

		transform.translation = rigidBody->GetPosition() + -rigidBodyoffset;

	}

	if ( hp > 0 )
	{
		PHealing();

		if ( !( usData.situation & ActorSituation::HEALING ) )
		{
			PRowling(camera_);

			PAttack();

			if ( index_ == GameCameraManager::CameraIndex::PLAYER_CAMERA && !( usData.situation & ActorSituation::ROWLING ) )
			{
				PMove(camera_);

			}
		}
	}

	if ( !isStationary )
	{
		PRotate();
	}

	if ( deviceInput->NotAction() && usData.situation == 0 )
	{
		for ( int32_t i = 0; i < 5; i++ )
		{
			if ( stamina <= MAX_STAMINA )
			{
				stamina++;
			}
			else
			{
				break;
			}

		}

	}

	if ( usData.situation & ActorSituation::DAMAGE )
	{
		damageInterval--;

		if ( damageInterval == 0 )
		{
			usData.situation &= ~ActorSituation::DAMAGE;
			damageInterval = MAX_DAMAGE_INTERVAL;
		}
	}

	PUIUpdate();

	ui->Update();
	weapon->Update("mixamorig:RightHand",animation->GetAnimation(),model.get());
	animation->Update();
}

void Player::Draw()
{
	model->Draw(transform,animation->GetAnimation());
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,1.0f ,1.0f ,1.0f },true);
	weapon->Draw();
}

void Player::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	weapon->Finalize(physicsSystem_);
	physicsSystem_->RemoveRigidBody(rigidBody);
}

const AliceMathF::Vector3& Player::GetPosition() const
{
	return transform.translation;
}

void Player::TransUpdate(Camera* camera_)
{
	transform.LookAtMatrixAxisFix(direction,{ 0,1,0 },camera_);
	weapon->TransUpdate(camera_);

	camera = camera_;
}

void Player::UIDraw()
{
	ui->Draw();
}

void Player::Reset()
{
	hp = MAX_HP;
	ui->SetHp(hp,MAX_HP);

	stamina = MAX_STAMINA;
	ui->SetStamina(stamina,MAX_STAMINA);

	bullet = MAX_BULLET;
	ui->SetBullet(bullet);

	healing = MAX_HEALING;
	ui->SetHealing(healing);

	damageInterval = MAX_DAMAGE_INTERVAL;

	PRotate();
}

void Player::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_)
{
	if ( BodyData_->GetGroup() == CollisionGroup::BOSS )
	{
		if ( BodyData_->GetAttribute() == CollisionAttribute::HAND )
		{
			uint32_t lSituation = static_cast< BossUsData* >( BodyData_->GetUserData() )->situation;

			if ( ( lSituation & 0xfff ) == ActorSituation::ATTACK &&
				!( usData.situation & ActorSituation::DAMAGE ) &&
				static_cast< BossUsData* >( BodyData_->GetUserData() )->index == BossHandIndex::RIGHT )
			{
				if ( hp > 0 )
				{
					for ( int32_t i = 0; i < 10; i++ )
					{
						if ( hp == 0 )
						{
							break;
						}

						//hp--;
					}

					usData.situation |= ActorSituation::DAMAGE;

					if ( hp <= 0 )
					{
						animation->InsertDeathAnimation();
						animation->AnimationEndStop();
						audioManager->PlayWave(deathSE);
						audioManager->ChangeVolume(deathSE,deathSEVolume);
					}
					else
					{
						if ( !( usData.situation & ActorSituation::WALKING ) )
						{
							animation->InsertHitAnimation();

						}
					}
				}
			}
		}
	}
}

void Player::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_)
{
	if ( BodyData_->GetGroup() == CollisionGroup::FIELD )
	{
		fieldHit = true;
	}

	if ( BodyData_->GetAttribute() == CollisionAttribute::SHOCKWAVE )
	{
		bool isHit = false;

		BossShockWaveUsData* lUsData = static_cast< BossShockWaveUsData* >( BodyData_->GetUserData() );
		AliceMathF::Vector3 lDistance = lUsData->pos - transform.translation;
		isHit = lUsData->radius >= AliceMathF::Abs(lDistance.Length());

		shockwaveHit = true;

		if ( !( usData.situation & ActorSituation::SHOCKWAVE_DAMAGE ) && isHit && ( !lUsData->isFinish ) )
		{
			if ( hp > 0 )
			{
				for ( int32_t i = 0; i < 10; i++ )
				{
					if ( hp == 0 )
					{
						break;
					}

					//hp--;
				}

				usData.situation |= ActorSituation::SHOCKWAVE_DAMAGE;

				if ( hp <= 0 )
				{
					animation->InsertDeathAnimation();
					animation->AnimationEndStop();
					audioManager->PlayWave(deathSE);
					audioManager->ChangeVolume(deathSE,deathSEVolume);
				}
				else
				{
					if ( !( usData.situation & ActorSituation::WALKING ) )
					{
						animation->InsertHitAnimation();

					}
				}
			}
		}
	}

	if ( BodyData_->GetAttribute() == CollisionAttribute::BEAM )
	{
		bool isHit = false;

		BossBeamUsData* lUsData = static_cast< BossBeamUsData* >( BodyData_->GetUserData() );
		AliceMathF::Vector3 lDistance = lUsData->pos - AliceMathF::Vector3(transform.translation.x,0.0f,transform.translation.z);
		isHit = lUsData->distance >= AliceMathF::Abs(lDistance.Length());

		shockwaveHit = true;

		if ( !( usData.situation & ActorSituation::DAMAGE ) && isHit && ( !lUsData->isFinish ) )
		{
			if ( hp > 0 )
			{
				for ( int32_t i = 0; i < 10; i++ )
				{
					if ( hp == 0 )
					{
						break;
					}

					//hp--;
				}

				usData.situation |= ActorSituation::DAMAGE;

				if ( hp <= 0 )
				{
					animation->InsertDeathAnimation();
					animation->AnimationEndStop();
					audioManager->PlayWave(deathSE);
					audioManager->ChangeVolume(deathSE,deathSEVolume);
				}
				else
				{
					if ( !( usData.situation & ActorSituation::WALKING ) )
					{
						animation->InsertHitAnimation();

					}
				}
			}
		}
	}
}


void Player::OnCollisionExit()
{
}

bool Player::IsAttack()
{
	return usData.situation & ActorSituation::ATTACK && animation->GetInsertAnimationPhase() == InsertAnimationPhase::DURING;
}

int32_t Player::GetDamage()
{
	return 1;
}

int32_t Player::GetHp()
{
	return hp;
}

bool Player::IsEnd()
{
	return hp <= 0 && !animation->IsInsert();
}

void Player::AnimationStop()
{
	animation->AnimationStop();
}

void Player::AnimationEndStop()
{
	animation->AnimationEndStop();
}

void Player::DeathSEChangeVolume(float volume_)
{
	audioManager->ChangeVolume(deathSE,deathSEVolume * volume_);
}

void Player::PMove(BaseGameCamera* camera_)
{
	AliceMathF::Vector3 lMove;

	float lSpeed = speed;

	AliceMathF::Vector3 lCameraForward = camera_->GetGameCamera()->GetTarget().Normal();
	lCameraForward = { lCameraForward.x, 0.0f, lCameraForward.z };

	AliceMathF::Vector2 lLeftStickPower = input->GetLeftStickVec();

	float lStickPower = AliceMathUtility::Max<float>(AliceMathF::Abs(lLeftStickPower.x),AliceMathF::Abs(lLeftStickPower.y));

	if ( !deviceInput->InputButton(ControllerButton::A,10.0f) )
	{
		lLeftStickPower = AliceMathF::Clamp(lLeftStickPower,0,0.45f);
		lStickPower = AliceMathF::Clamp(lStickPower,0,0.45f);
	}
	else
	{
		if ( lStickPower >= 0.5f )
		{
			if ( stamina < subRunStamina )
			{
				lLeftStickPower = AliceMathF::Clamp(lLeftStickPower,0,0.45f);

				lStickPower = AliceMathF::Clamp(lStickPower,0,0.45f);
			}
			else
			{
				stamina -= subRunStamina;
			}
		}
	}

	animation->WalkAnimationUpdate(lStickPower);

	if ( lStickPower >= 0.01f )
	{
		usData.situation |= ActorSituation::WALKING;

		lMove = { lLeftStickPower.x * lSpeed, 0.0f, -lLeftStickPower.y * lSpeed };
		isStationary = false;
	}
	else
	{
		usData.situation &= ~ActorSituation::WALKING;

	}

	PlayerGameCamera* lPlayerCamera = dynamic_cast< PlayerGameCamera* >( camera_ );

	// カメラの角度に合わせて移動ベクトルを回転してから加算
	float SinParam = AliceMathF::Sin(lPlayerCamera->GetAngle().y / AliceMathF::DEG_TO_RAD);
	float CosParam = AliceMathF::Cos(lPlayerCamera->GetAngle().y / AliceMathF::DEG_TO_RAD);

	lMove =
	{
		lMove.x * CosParam - lMove.z * SinParam ,
		0.0f ,
		lMove.x * SinParam + lMove.z * CosParam
	};

	rigidBody->SetLinearVelocity(lMove);
	//dynamicBody->setAngularVelocity({ 0,0,0 });
	transform.translation = rigidBody->GetPosition() + -rigidBodyoffset;
}

void Player::PRowling(BaseGameCamera* camera_)
{
	AliceMathF::Vector2 lLeftStickPower = input->GetLeftStickVec();

	float lStickPower = AliceMathUtility::Max<float>(AliceMathF::Abs(lLeftStickPower.x),AliceMathF::Abs(lLeftStickPower.y));

	if ( lStickPower >= 0.5f )
	{
		if ( deviceInput->TriggerButton(ControllerButton::A,10.0f) && !( usData.situation & ActorSituation::ROWLING ) )
		{
			usData.situation |= ActorSituation::ROWLING;

			rowlingWay = AliceMathF::Vector3(lLeftStickPower.x,0.0f,-lLeftStickPower.y);

			PlayerGameCamera* lPlayerCamera = dynamic_cast< PlayerGameCamera* >( camera_ );

			// カメラの角度に合わせて移動ベクトルを回転してから加算
			float SinParam = AliceMathF::Sin(lPlayerCamera->GetAngle().y / AliceMathF::DEG_TO_RAD);
			float CosParam = AliceMathF::Cos(lPlayerCamera->GetAngle().y / AliceMathF::DEG_TO_RAD);

			rowlingWay =
			{
				rowlingWay.x * CosParam - rowlingWay.z * SinParam ,
				0.0f ,
				rowlingWay.x * SinParam + rowlingWay.z * CosParam
			};

			rowlingStartPos = transform.translation;

			rowlingWay *= rowlingDistance;

			stamina -= subRowlingStamina;

			animation->InsertRowlingAnimation();
			animation->SetAddFrame(0.035f);
		}
	}

	if ( usData.situation & ActorSituation::ROWLING && animation->IsInsert() )
	{
		transform.translation = AliceMathF::Easing::EaseInSine(animation->GetRatio(),1.0f,rowlingStartPos,rowlingWay);
	}

	if ( usData.situation & ActorSituation::ROWLING && !animation->IsInsert() )
	{
		usData.situation &= ~ActorSituation::ROWLING;

		rigidBody->SetRotation(AliceMathF::Quaternion());
		animation->SetAddFrame();
	}
}

void Player::PUIUpdate()
{
	ui->SetHp(hp,MAX_HP);

	ui->SetStamina(stamina,MAX_STAMINA);

	ui->SetBullet(bullet);

	ui->SetHealing(healing);
}

void Player::PRotate()
{
	direction = oldTrans - transform.translation;

	direction = direction.Normal();

}

void Player::PAttack()
{
	if ( input->InputButton(ControllerButton::LB) && !( usData.situation & ActorSituation::ATTACK ) )
	{
		animation->InsertAttackAnimation();
		stamina -= subAttackStamina;

		usData.situation |= ActorSituation::ATTACK;
	}

	if ( usData.situation & ActorSituation::ATTACK && !animation->IsInsert() )
	{
		usData.situation &= ~ActorSituation::ATTACK;
	}
}

void Player::PHealing()
{
	if ( deviceInput->InputButton(ControllerButton::Y,1.0f) && usData.situation == 0 )
	{
		if ( healing != 0 )
		{
			usData.situation |= ActorSituation::HEALING;
			animation->InsertHealingAnimation();
			healing--;
			ui->SetHealing(healing);

			for ( int32_t i = 0; i < healingNum; i++ )
			{
				if ( hp == MAX_HP )
				{
					break;
				}

				hp++;
			}
		}

	}

	if ( usData.situation & ActorSituation::HEALING && !animation->IsInsert() )
	{
		usData.situation &= ~ActorSituation::HEALING;
	}
}
