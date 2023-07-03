#pragma once
#include<GameObject.h>

class PlayerUI
{
private:

	UI hpBarUI;
	const AliceMathF::Vector3 MAX_HP_BAR_SCALE = { 63.0f,1.5f,1.0f };

	UI staminaBarUI;
	const AliceMathF::Vector3 MAX_STAMINA_BAR_SCALE = { 40.3f,1.5f,1.0f };

	UI bulletUI;
	const int32_t MAX_BULLET_NUM = 20;
	int32_t bulletNum;

	UI healingUI;
	const int32_t MAX_HEALING_NUM = 20;
	int32_t healingNum;

	UI hpBarBackUI;
	float hpWidthScale = 0.0f;
	int32_t hpWidthMax = 0;

	UI staminaBarBackUI;
	float staminaWidthScale = 0.0f;
	int32_t staminaWidthMax = 0;

public:

	PlayerUI() = default;
	~PlayerUI() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	void SetHp(int32_t hp_,int32_t maxHp_);

	void SetStamina(int32_t stamina_, int32_t maxStamina_);

	void SetBullet(int32_t bullet_);

	void SetHealing(int32_t healing_);

private:

	//コピーコンストラクタ・代入演算子削除
	PlayerUI& operator=(const PlayerUI&) = delete;
	PlayerUI(const PlayerUI&) = delete;
};

