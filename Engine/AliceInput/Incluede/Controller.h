#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<memory>
#include<stdint.h>

#pragma warning(pop)

#include<AliceMathUtility.h>
#include"ControllerButtons.h"

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
		virtual bool TriggerButton(ControllerButton button_) = 0;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerButton(uint32_t button_) = 0;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool InputButton(ControllerButton button_) = 0;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool InputButton(uint32_t button_) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerReleaseButton(ControllerButton button_) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool TriggerReleaseButton(uint32_t button_) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(ControllerButton button_) = 0;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(uint32_t button_) = 0;

		/// <summary>
		/// コントローラースティックのトリガー入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool TriggerStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラースティックの入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool InputStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラースティックの離した瞬間
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool TriggerReleaseStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) = 0;


		/// <summary>
		/// コントローラーの左スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラーの右スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) = 0;

		/// <summary>
		/// コントローラーを振動させる
		/// </summary>
		/// <param name="power">振動の強さ0.0f～1.0f</param>
		/// <param name="span">振動の時間フレーム</param>
		virtual void ShakeController(float power, int32_t span) = 0;
	};

	/// <summary>
	/// コントローラーの生成(ユニーク)
	/// </summary>
	/// <returns>生成されたポインタ</returns>
	std::unique_ptr<IController> CreateUniqueController();

	/// <summary>
	/// コントローラーの生成(シェアード)
	/// </summary>
	/// <returns>生成されたポインタ</returns>
	std::shared_ptr<IController> CreateSharedController();

}