#pragma once

class Player;

class BossBaseAction
{
public:

	BossBaseAction() = default;
	virtual ~BossBaseAction() = default;

	virtual void SetPlayerData(Player* player_) = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 後始末
	/// </summary>
	virtual void Finalize() = 0;

};

