#include "BossUI.h"

void BossUI::Initialize()
{
	hpBarBackUI.spriteHandle = TextureManager::SLoad("Resources/UI/BarBack.png");
	hpBarBackUI.sprite = CreateUniqueSprite2D(hpBarBackUI.spriteHandle);
	hpBarBackUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	hpBarBackUI.transform.Initialize();
	hpBarBackUI.transform.translation = { 220.0f,620.0f,0.0f };
	hpBarBackUI.transform.scale = { 6.57f,0.08f,1.0f };

	hpBarUI.spriteHandle = TextureManager::SLoad("Resources/UI/HpBar.png");
	hpBarUI.sprite = CreateUniqueSprite2D(hpBarUI.spriteHandle);
	hpBarUI.sprite->SetAnchorPoint({ 0.0f, 0.5f });
	hpBarUI.transform.Initialize();
	hpBarUI.transform.translation = { 220.0f,620.0f,0.0f };

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
}

void BossUI::Update()
{
	hpBarUI.transform.scale = { hpWidthScale,MAX_HP_BAR_SCALE.y,MAX_HP_BAR_SCALE.z };
}

void BossUI::Draw()
{
	hpBarBackUI.sprite->Draw(hpBarBackUI.transform);
	hpBarUI.sprite->Draw(hpBarUI.transform);
}

void BossUI::Finalize()
{
}

void BossUI::SetHp(int32_t hp_, int32_t maxHp_)
{
	hpWidthScale = MAX_HP_BAR_SCALE.x * (static_cast<float>(hp_) / maxHp_);

	hpWidthMax = maxHp_;
}
