#pragma once

#include<stdint.h>

#include<Keys.h>

namespace AliceInput
{

	class IKeyboard
	{
	public:
		virtual void Initialize(void* directInput, void* hwnd) = 0;
		virtual void Update() = 0;

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

		IKeyboard() = default;
		virtual ~IKeyboard() = default;;
	};

	//キーボードを生成
	IKeyboard* CreateKeyboard(void* directInput, void* hwnd);
}