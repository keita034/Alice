#include "Player.h"

#include<OBBCollider.h>

#include<GameCameraManager.h>
#include<PlayerGameCamera.h>
#include<GameCollisionConflg.h>

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

	testHandle = AliceMotionData::SCreateMotion("Resources/Model/Player/Motion/Idle.almb");
	testAnime = std::make_unique<AliceMotionData>();
	testAnime->SetMotion(testHandle);

}

void Player::Update(BaseGameCamera* camera_, GameCameraManager::CameraIndex index_)
{
	oldTrans = transform.translation;
	isStationary = true;

	if (index_ == GameCameraManager::CameraIndex::PLAYER_CAMERA)
	{
		PMove(camera_);
		
	}

	if (!isStationary)
	{
		PRotate();
	}

	ui->Update();
	animation->Update();
}

void Player::Draw()
{
	/*model->Draw(transform, testAnime.get());*/
	model->Draw(transform, animation->GetAnimation());
	//model->Draw(transform);
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

void Player::PMove(BaseGameCamera* camera_)
{
	AliceMathF::Vector3 lMove;

	AliceMathF::Vector3 lCameraForward = camera_->GetGameCamera()->GetTarget().Normal();
	lCameraForward = { lCameraForward.x, 0.0f, lCameraForward.z };

	/*if (input->InputStick(ControllerStick::LUP))
	{
		lMove += {0.0f, 0.0f, speed};
		isStationary = false;
	}
	if (input->InputStick(ControllerStick::LDOWN))
	{
		lMove += {0.0f, 0.0f, -speed};
		isStationary = false;
	}
	if (input->InputStick(ControllerStick::LLEFT))
	{
		lMove += {-speed, 0.0f, 0.0f};
		isStationary = false;
	}
	if (input->InputStick(ControllerStick::LRIGHT))
	{
		lMove += {speed, 0.0f, 0.0f};
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

	transform.translation += lMove;
}

void Player::PRotate()
{
	direction = oldTrans - transform.translation;

	direction = direction.Normal();

}
