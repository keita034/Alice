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

	PUIInitialize();

	{
		OBBCollider* lObb = new OBBCollider();

		const std::vector<PosNormUvTangeColSkin>& ver = model->GetMeshs()[0]->GetVertices();

		lObb->CreateOBB(ver, transform.translation, transform.rotation, transform.scale);

		lObb->SetCollsionName("Player");
		//衝突属性を設定
		lObb->SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
		//衝突対象を自分以外に設定
		lObb->SetCollisionMask(~COLLISION_ATTRIBUTE_PLAYER);


		SetCollider(lObb);
	}
}

void Player::PUIInitialize()
{
	hpBarBackUI.spriteHandle = TextureManager::SLoad("Resources/UI/BarBack.png");
	hpBarBackUI.sprite = CreateUniqueSprite2D(hpBarBackUI.spriteHandle);
	hpBarBackUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	hpBarBackUI.transform.Initialize();
	hpBarBackUI.transform.translation = { 83,45,0 };
	hpBarBackUI.transform.scale = { 2.0f,0.08f,1.0f };

	staminaBarBackUI.spriteHandle = TextureManager::SLoad("Resources/UI/BarBack.png");
	staminaBarBackUI.sprite = CreateUniqueSprite2D(staminaBarBackUI.spriteHandle);
	staminaBarBackUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	staminaBarBackUI.transform.Initialize();
	staminaBarBackUI.transform.translation = { 83,58,0 };
	staminaBarBackUI.transform.scale = { 1.3f,0.08f,1.0f };

	hp = MAX_HP;
	hpBarUI.spriteHandle = TextureManager::SLoad("Resources/UI/HpBar.png");
	hpBarUI.sprite = CreateUniqueSprite2D(hpBarUI.spriteHandle);
	hpBarUI.sprite->SetAnchorPoint({0.0f, 0.5f});
	hpBarUI.transform.Initialize();
	hpBarUI.transform.translation = { 85.5f,45.0f,0.0f };
	hpBarUI.transform.scale = MAX_HP_BAR_SCALE;

	stamina = MAX_STAMINA;
	staminaBarUI.spriteHandle = TextureManager::SLoad("Resources/UI/StaminaBar.png");
	staminaBarUI.sprite = CreateUniqueSprite2D(staminaBarUI.spriteHandle);
	staminaBarUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	staminaBarUI.transform.Initialize();
	staminaBarUI.transform.translation = { 85.5f,58.0f,0.0f };
	staminaBarUI.transform.scale = MAX_STAMINA_BAR_SCALE;

	bullet = MAX_BULLET;
	bulletUI.spriteHandle = TextureManager::SLoad("Resources/UI/Bullet.png");
	bulletUI.sprite = CreateUniqueSprite2D(bulletUI.spriteHandle);
	bulletUI.transform.Initialize();
	bulletUI.transform.translation = { 100,120,0 };
	bulletUI.transform.scale = { 0.25f,0.25f,1.0f };

	healing = MAX_HEALING;
	healingUI.spriteHandle = TextureManager::SLoad("Resources/UI/Healing.png");
	healingUI.sprite = CreateUniqueSprite2D(healingUI.spriteHandle);
	healingUI.transform.translation = { 100,85,0 };
	healingUI.transform.scale = {0.25f,0.25f,1.0f };
	healingUI.transform.Initialize();

	hitTex = TextureManager::SLoad("Resources/Model/Player/hit.png");
	defTex = TextureManager::SLoad("Resources/Model/Player/white1x1.png");

	PRotate();
}

void Player::Update(BaseGameCamera* camera_, GameCameraManager::CameraIndex index_)
{
	oldTrans = transform.translation;
	isStationary = true;
	model->TransTexture("マテリアル.001", 0, TextureManager::SGetTextureData(defTex));

	if (index_ == GameCameraManager::CameraIndex::PLAYER_CAMERA)
	{
		PMove(camera_);
		
	}

	if (!isStationary)
	{
		PRotate();
	}
}

void Player::Draw()
{
	model->Draw(transform);
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

	{
		OBBCollider* lObb = dynamic_cast<OBBCollider*>(collider.get());
		lObb->UpdateOBB(transform.matWorld, transform.rotation);
	}
}

void Player::UIDraw()
{
	hpBarBackUI.sprite->Draw(hpBarBackUI.transform);
	hpBarUI.sprite->Draw(hpBarUI.transform);

	staminaBarBackUI.sprite->Draw(staminaBarBackUI.transform);
	staminaBarUI.sprite->Draw(staminaBarUI.transform);

	bulletUI.sprite->Draw(bulletUI.transform);

	healingUI.sprite->Draw(healingUI.transform);
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

	transform.translation += lMove;
}

void Player::OnCollision()
{
	model->TransTexture("マテリアル.001",0,TextureManager::SGetTextureData(hitTex));
}

void Player::PRotate()
{                      
	direction = oldTrans - transform.translation;

	direction = direction.Normal();

}
