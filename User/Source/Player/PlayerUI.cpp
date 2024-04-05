#include "PlayerUI.h"

void PlayerUI::Initialize()
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

	hpBarUI.spriteHandle = TextureManager::SLoad("Resources/UI/HpBar.png");
	hpBarUI.sprite = CreateUniqueSprite2D(hpBarUI.spriteHandle);
	hpBarUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	hpBarUI.transform.Initialize();
	hpBarUI.transform.translation = { 85.5f,45.0f,0.0f };

	staminaBarUI.spriteHandle = TextureManager::SLoad("Resources/UI/StaminaBar.png");
	staminaBarUI.sprite = CreateUniqueSprite2D(staminaBarUI.spriteHandle);
	staminaBarUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	staminaBarUI.transform.Initialize();
	staminaBarUI.transform.translation = { 85.5f,58.0f,0.0f };

	bulletUI.spriteHandle = TextureManager::SLoad("Resources/UI/Bullet.png");
	bulletUI.sprite = CreateUniqueSprite2D(bulletUI.spriteHandle);
	bulletUI.transform.Initialize();
	bulletUI.transform.translation = { 100,120,0 };
	bulletUI.transform.scale = { 0.25f,0.25f,1.0f };

	healingUI.spriteHandle = TextureManager::SLoad("Resources/UI/Healing.png");
	healingUI.sprite = CreateUniqueSprite2D(healingUI.spriteHandle);
	healingUI.transform.translation = { 100,85,0 };
	healingUI.transform.scale = { 0.25f,0.25f,1.0f };
	healingUI.transform.Initialize();

	numberHandle[ 0 ] = TextureManager::SLoad("Resources/UI/0.png");
	numberHandle[ 1 ] = TextureManager::SLoad("Resources/UI/1.png");
	numberHandle[ 2 ] = TextureManager::SLoad("Resources/UI/2.png");
	numberHandle[ 3 ] = TextureManager::SLoad("Resources/UI/3.png");
	numberHandle[ 4 ] = TextureManager::SLoad("Resources/UI/4.png");
	numberHandle[ 5 ] = TextureManager::SLoad("Resources/UI/5.png");
	numberHandle[ 6 ] = TextureManager::SLoad("Resources/UI/6.png");
	numberHandle[ 7 ] = TextureManager::SLoad("Resources/UI/7.png");
	numberHandle[ 8 ] = TextureManager::SLoad("Resources/UI/8.png");
	numberHandle[ 9 ] = TextureManager::SLoad("Resources/UI/9.png");

	for ( size_t i = 0; i < numberSprite.size(); i++ )
	{
		numberSprite[ i ] = CreateUniqueSprite2D(numberHandle[ i ]);
	}

	healingNumberTransform[ 0 ].Initialize();
	healingNumberTransform[ 1 ].Initialize();
}

void PlayerUI::Update()
{
	hpBarUI.transform.scale = { hpWidthScale,MAX_HP_BAR_SCALE.y,MAX_HP_BAR_SCALE.z };
	staminaBarUI.transform.scale = { staminaWidthScale,MAX_STAMINA_BAR_SCALE.y,MAX_STAMINA_BAR_SCALE.z };
}

void PlayerUI::Draw()
{
	hpBarBackUI.sprite->Draw(hpBarBackUI.transform);
	hpBarUI.sprite->Draw(hpBarUI.transform);

	staminaBarBackUI.sprite->Draw(staminaBarBackUI.transform);
	staminaBarUI.sprite->Draw(staminaBarUI.transform);

	//bulletUI.sprite->Draw(bulletUI.transform);

	PDrawHealingUI();
}

void PlayerUI::Finalize()
{
}

void PlayerUI::SetHp(int32_t hp_, int32_t maxHp_)
{
	hpWidthScale = MAX_HP_BAR_SCALE.x * (static_cast<float>(hp_) / maxHp_);

	hpWidthMax = maxHp_;
}

void PlayerUI::SetStamina(int32_t stamina_, int32_t maxStamina_)
{
	staminaWidthScale = MAX_STAMINA_BAR_SCALE.x * (static_cast<float>(stamina_) / maxStamina_);

	staminaWidthMax = maxStamina_;
}

void PlayerUI::SetBullet(int32_t bullet_)
{
	bulletNum = bullet_;
}

void PlayerUI::SetHealing(int32_t healing_)
{
	healingNum = healing_;
}

void PlayerUI::PDrawHealingUI()
{
	int32_t lOnesPlace = healingNum % 10;
	int32_t lTensPlace = healingNum / 10;

	if ( lTensPlace != 0 )
	{
		healingNumberTransform[ 0 ].translation = healingNumberTrans[ 0 ];
		healingNumberTransform[ 0 ].scale = healingUI.transform.scale;

		healingNumberTransform[ 1 ].translation = healingNumberTrans[ 1 ];
		healingNumberTransform[ 1 ].scale = healingUI.transform.scale;

		numberSprite[ lTensPlace ]->Draw(healingNumberTransform[ 0 ]);
		numberSprite[ lOnesPlace ]->Draw(healingNumberTransform[ 1 ]);
	}
	else
	{
		healingNumberTransform[ 1 ].translation = healingNumberTrans[ 0 ];
		healingNumberTransform[ 1 ].scale = healingUI.transform.scale;
		numberSprite[ lOnesPlace ]->Draw(healingNumberTransform[ 1 ]);
	}


	healingUI.sprite->Draw(healingUI.transform);
}
