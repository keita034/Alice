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
