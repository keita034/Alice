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
#include<Keys.h>
#include<ControllerButtons.h>
#include<MouseButton.h>

namespace AliceInput
{
	class IInput
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize(void* hwnd_, void* hInstance_) = 0;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update(float width_, float height_) = 0;

		//キーボード

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerKey(uint32_t key_) = 0;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerKey(Keys key_) = 0;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool CheckKey(const uint32_t key_) = 0;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool CheckKey(Keys key_) = 0;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerReleaseKey(uint32_t key_) = 0;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerReleaseKey(Keys key_) = 0;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool ReleaseKey(uint32_t key_) = 0;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool ReleaseKey(Keys key_) = 0;

		//マウス

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		virtual bool TriggerMouseButton(MouseButton button_) = 0;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		virtual bool InputMouseButton(MouseButton button_) = 0;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		virtual bool TriggerReleaseMouseButton(MouseButton button_) = 0;

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
		/// コントローラーボタンの離した
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(ControllerButton button_) = 0;

		/// <summary>
		/// コントローラーボタンの離した
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		virtual bool ReleaseButton(uint32_t button_) = 0;

		/// <summary>
		/// コントローラースティックのトリガー入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		virtual bool TriggerStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) = 0;

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
		virtual void ShakeController(float power_, int32_t span_) = 0;

		virtual ~IInput() = default;
		IInput() = default;
	};

	/// <summary>
	/// インプットデバイスの生成(ユニーク)
	/// </summary>
	/// <returns>生成されたポインタ</returns>
	std::unique_ptr<IInput> CreateUniqueInput(void* hwnd_, void* hInstance_);

	/// <summary>
	/// インプットデバイスの生成(シェアード)
	/// </summary>
	/// <returns>生成されたポインタ</returns>
	std::shared_ptr<IInput> CreateSharedInput(void* hwnd_, void* hInstance_);
}