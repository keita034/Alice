#pragma once
#include<AliceMathUtility.h>

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

	//左スティック
	LSTICK,
	//右スティック
	RSTICK,

	//左バンパー
	LB = 0x0100,
	//右バンパー
	RB = 0x0200,
	//左トリガー
	LT = 0x4000,
	//右トリガー
	RT = 0x8000,

	//A
	A = 0x1000,
	//B
	B = 0x2000,
	//X
	X = 0x4000,
	//Y
	Y = 0x8000,
};

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

namespace AliceInput
{
	class IController
	{
	public:
		//コンストラクタ・デストラクタ
		IController() = default;
		virtual ~IController() = default;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerButton(ControllerButton button) = 0;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerButton(uint32_t button) = 0;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool InputButton(ControllerButton button) = 0;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool InputButton(uint32_t button) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerReleaseButton(ControllerButton button) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerReleaseButton(uint32_t button) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(ControllerButton button) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(uint32_t button) = 0;

		/// <summary>
		/// コントローラースティックのトリガー入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool TriggerStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラースティックの入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool InputStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラースティックの離した瞬間
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool TriggerReleaseStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) = 0;


		/// <summary>
		/// コントローラーの左スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラーの右スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラーを振動させる
		/// </summary>
		/// <param name="power">振動の強さ0.0f～1.0f</param>
		/// <param name="span">振動の時間フレーム</param>
		virtual void ShakeController(const float& power, const int& span) = 0;
	};

	//キーボードを生成
	IController* CreateController();

}