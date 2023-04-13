#pragma once

//#DirectXのやつ
#include"ErrorException.h"


//自作.h
#include"GameScene.h"
#include"AliceFramework.h"

class GameMain : public AliceFramework
{
private:

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;;

	GameMain();
	~GameMain();
private:

	//コピーコンストラクタ・代入演算子削除
	GameMain& operator=(const GameMain&) = delete;
	GameMain(const GameMain&) = delete;
};

