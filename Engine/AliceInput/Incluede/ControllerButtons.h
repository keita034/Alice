#pragma once

/// <summary>
/// コントローラーボタン
/// </summary>
enum class ControllerButton
{
	//上
	UP = 0x0001,
	//下
	DOWN = 0x0002,
	//左
	LEFT = 0x0004,
	//右
	RIGHT = 0x0008,

	//スタート
	START = 0x0010,
	//バック
	BACK = 0x0020,

	//左バンパー
	LB = 0x0040,
	//右バンパー
	RB = 0x0080,
	//左トリガー
	LT = 0x0041,
	//右トリガー
	RT = 0x0081,


	//左スティック
	LSTICK = 0x0100,
	//右スティック
	RSTICK = 0x0200,

	//A
	A = 0x1000,
	//B
	B = 0x2000,
	//X
	X = 0x4000,
	//Y
	Y = 0x8000,
};

/// <summary>
/// コントローラースティック
/// </summary>
enum  class ControllerStick
{
	//左スティック
	//上入力
	LUP,
	//左スティック
	//下入力
	LDOWN,
	//左スティック
	//左入力	
	LLEFT,
	//左スティック
	//右入力
	LRIGHT,

	//右スティック
	//上入力
	RUP,
	//右スティック
	//下入力
	RDOWN,
	//右スティック
	//左入力	
	RLEFT,
	//右スティック
	//右入力
	RRIGHT,

	XBOXSTICKNUM
};
