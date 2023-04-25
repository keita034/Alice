#pragma once
#include<Keyboard.h>
#include<Mouse.h>
#include<Controller.h>

namespace AliceInput
{
	class IInput
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(void* hwnd, void* hInstance) = 0;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update(float width, float height) = 0;

		//キーボード

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerKey(const uint32_t key) = 0;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerKey(const Keys key) = 0;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool CheckKey(const uint32_t key) = 0;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool CheckKey(const Keys key) = 0;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerReleaseKey(const uint32_t key) = 0;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerReleaseKey(const Keys key) = 0;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool ReleaseKey(const uint32_t key) = 0;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool ReleaseKey(const Keys key) = 0;

		//マウス

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		virtual bool TriggerMouseButton(MouseButton button) = 0;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		virtual bool InputMouseButton(MouseButton button) = 0;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		virtual bool TriggerReleaseMouseButton(MouseButton button) = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetMousePos() = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetWorldMousePos() = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector2& GetLocalMousePos() = 0;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		virtual const AliceMathF::Vector3& GetMouseMove() = 0;

		//コントローラ-

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
		/// コントローラーボタンの離した
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(ControllerButton button) = 0;

		/// <summary>
		/// コントローラーボタンの離した
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

		virtual ~IInput() = default;
		IInput() = default;
	};

	IInput* CreateInput(void* hwnd, void* hInstance);
}