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

	CreateMaterial(0.6f,0.6f,0.0f);
	CreateShape(6.0f, 8.0f, CollisionAttribute::PLAYER, (CollisionAttribute::BOSS | CollisionAttribute::ENEMY | CollisionAttribute::FIELD));
	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	SetInitializePos(pos);
	SetInitializeRot(AliceMathF::Quaternion({0,0,1}, AliceMathF::DEG_TO_RAD * 90.0f));
	CreateRigidBody(RigidBodyType::DYNAMIC);

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
	//objectCollsionDraw->DebugDraw(collisionShape, shape, debugIndex, pxTransform, camera);
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

void Player::PMove(BaseGameCamera* camera_)
{
	AliceMathF::Vector3 lMove;

	AliceMathF::Vector3 lCameraForward = camera_->GetGameCamera()->GetTarget().Normal();
	lCameraForward = { lCameraForward.x, 0.0f, lCameraForward.z };

	if (input->InputStick(ControllerStick::LUP))
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
	}

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
	dynamicBody->setAngularVelocity({ 0,0,0 });
	transform.translation = GetGlobalPos() + -rigidBodyoffset;


}

void Player::PRotate()
{
	direction = oldTrans - transform.translation;

	direction = direction.Normal();

}
