#pragma once
#include<GameObject.h>

class BossUI
{
private:

	UI hpBarUI;
	const AliceMathF::Vector3 MAX_HP_BAR_SCALE = { 210.0f,1.5f,1.0f };

	UI hpBarBackUI;
	float hpWidthScale = 0.0f;
	int32_t hpWidthMax = 0;

public:

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

	void SetHp(int32_t hp_, int32_t maxHp_);

	BossUI() = default;
	~BossUI() = default;

public:

	//コピーコンストラクタ・代入演算子削除
	BossUI& operator=(const BossUI&) = delete;
	BossUI(const BossUI&) = delete;
};

