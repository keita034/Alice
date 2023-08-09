#include "Player.h"

#include<OBBCollider.h>

#include<GameCameraManager.h>
#include<PlayerGameCamera.h>
#include<GameCollisionConflg.h>
#include<CollisionAttribute.h>

void Player::Initialize(AliceInput::IInput* input_)
{
	assert(input_);

	input = input_;

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

	rigidBodyoffset = { 0.0f, 8.0f + 6.0f, 0.0f };

	CreateMaterial(0.6f, 0.6f, 0.0f);
	CreateShape(6.0f, 8.0f, CollisionAttribute::PLAYER, (CollisionAttribute::BOSS | CollisionAttribute::ENEMY | CollisionAttribute::FIELD));
	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	SetInitializePos(pos);
	SetInitializeRot(AliceMathF::Quaternion({ 0,0,1 }, AliceMathF::DEG_TO_RAD * 90.0f));
	CreateRigidBody(RigidBodyType::DYNAMIC);

	weapon = std::make_unique<PlayerWeapon>();
	weapon->Initialize(&transform);
}

void Player::Update(BaseGameCamera* camera_, GameCameraManager::CameraIndex index_)
{
	oldTrans = transform.translation;
	isStationary = true;

	if (fieldHit)
	{
		fieldHit = false;

		physx::PxTransform lTransform;
		lTransform = dynamicBody->getGlobalPose();

		if (lTransform.q != pxTransform.q)
		{
			lTransform.q = pxTransform.q;
			dynamicBody->setGlobalPose(lTransform);
			transform.translation = GetGlobalPos() + -rigidBodyoffset;
		}

	}

	PRowling(camera_);

	PAttack();

	if (index_ == GameCameraManager::CameraIndex::PLAYER_CAMERA && !(situation & PlayerSituation::ROWLING))
	{
		PMove(camera_);

	}


	if (!isStationary)
	{
		PRotate();
	}

	ui->Update();
	animation->Update();
	weapon->Update("mixamorig:RightHand", animation->GetAnimation(), model.get());
}

void Player::Draw()
{
	model->Draw(transform, animation->GetAnimation());
	weapon->Draw();
}

void Player::Finalize()
{
}

const AliceMathF::Vector3& Player::GetPosition() const
{
	return transform.translation;
}

void Player::TransUpdate(Camera* camera_)
{
	transform.LookAtMatrixAxisFix(direction, { 0,1,0 }, camera_);
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
	ui->SetHp(hp, MAX_HP);

	stamina = MAX_STAMINA;
	ui->SetStamina(stamina, MAX_STAMINA);

	bullet = MAX_BULLET;
	ui->SetBullet(bullet);

	healing = MAX_HEALING;
	ui->SetHealing(healing);

	PRotate();
}

void Player::OnContact(uint32_t attribute_)
{
	static_cast<void>(attribute_);

	if (attribute_ & CollisionAttribute::FIELD)
	{
		fieldHit = true;
	}

}

bool Player::IsAttack()
{
	return situation & PlayerSituation::ATTACK && animation->GetInsertAnimationPhase() == InsertAnimationPhase::DURING;
}

int32_t Player::GetDamage()
{
	return 1;
}

void Player::PMove(BaseGameCamera* camera_)
{
	AliceMathF::Vector3 lMove;

	float lSpeed = speed;

	AliceMathF::Vector3 lCameraForward = camera_->GetGameCamera()->GetTarget().Normal();
	lCameraForward = { lCameraForward.x, 0.0f, lCameraForward.z };

	AliceMathF::Vector2 lLeftStickPower = input->GetLeftStickVec();

	float lStickPower = AliceMathUtility::Max<float>(AliceMathF::Abs(lLeftStickPower.x), AliceMathF::Abs(lLeftStickPower.y));

	if (!input->InputButton(ControllerButton::A))
	{
		lLeftStickPower *= 0.5f;
	}

	if (lStickPower >= 0.01f)
	{
		lMove = { lLeftStickPower.x * lSpeed, 0.0f, -
			lLeftStickPower.y * lSpeed };
		isStationary = false;
	}


	//if (input->InputButton(ControllerButton::A))
	//{
	//	lSpeed *= lStickPower;
	//}
	//else
	//{
	//	lSpeed *= (0.5f * lStickPower);
	//}

	/*if (input->InputStick(ControllerStick::LUP))
	{
		lMove += {0.0f, 0.0f, lSpeed};
		isStationary = false;
	}
	if (input->InputStick(ControllerStick::LDOWN))
	{
		lMove += {0.0f, 0.0f, -lSpeed};
		isStationary = false;
	}
	if (input->InputStick(ControllerStick::LLEFT))
	{
		lMove += {-lSpeed, 0.0f, 0.0f};
		isStationary = false;
	}
	if (input->InputStick(ControllerStick::LRIGHT))
	{
		lMove += {lSpeed, 0.0f, 0.0f};
		isStationary = false;
	}*/

	PlayerGameCamera* lPlayerCamera = dynamic_cast<PlayerGameCamera*>(camera_);

	// カメラの角度に合わせて移動ベクトルを回転してから加算
	float SinParam = AliceMathF::Sin(lPlayerCamera->GetAngle().y / AliceMathF::DEG_TO_RAD);
	float CosParam = AliceMathF::Cos(lPlayerCamera->GetAngle().y / AliceMathF::DEG_TO_RAD);

	lMove =
	{
		lMove.x * CosParam - lMove.z * SinParam ,
		0.0f ,
		lMove.x * SinParam + lMove.z * CosParam
	};

	dynamicBody->setLinearVelocity(lMove);
	//dynamicBody->setAngularVelocity({ 0,0,0 });
	transform.translation = GetGlobalPos() + -rigidBodyoffset;


}

void Player::PRowling(BaseGameCamera* camera_)
{
	AliceMathF::Vector2 lLeftStickPower = input->GetLeftStickVec();

	float lStickPower = AliceMathUtility::Max<float>(AliceMathF::Abs(lLeftStickPower.x), AliceMathF::Abs(lLeftStickPower.y));

	if (lStickPower >= 0.5f)
	{
		if (input->TriggerButton(ControllerButton::A) && !(situation & PlayerSituation::ROWLING))
		{
			situation |= PlayerSituation::ROWLING;

			rowlingWay = AliceMathF::Vector3(lLeftStickPower.x, 0.0f, -lLeftStickPower.y);

			PlayerGameCamera* lPlayerCamera = dynamic_cast<PlayerGameCamera*>(camera_);

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

			animation->InsertRowlingAnimation();
		}
	}

	if (situation & PlayerSituation::ROWLING && animation->IsInsert())
	{
		transform.translation = AliceMathF::Easing::EaseInSine(animation->GetRatio(), 1.0f, rowlingStartPos, rowlingWay);
	}

	if (situation & PlayerSituation::ROWLING && !animation->IsInsert())
	{
		situation &= ~PlayerSituation::ROWLING;

		physx::PxTransform lTransform;

		lTransform.q = pxTransform.q;
		lTransform.p = transform.translation + rigidBodyoffset;;
		dynamicBody->setGlobalPose(lTransform);
	}
}

void Player::PRotate()
{
	direction = oldTrans - transform.translation;

	direction = direction.Normal();

}

void Player::PAttack()
{
	if (input->InputButton(ControllerButton::LB))
	{
		animation->InsertAttackAnimation();

		situation |= PlayerSituation::ATTACK;
	}

	if (situation & PlayerSituation::ATTACK && !animation->IsInsert())
	{
		situation &= ~PlayerSituation::ATTACK;
	}
}
