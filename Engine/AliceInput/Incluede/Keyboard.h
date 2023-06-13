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

#include<Keys.h>

namespace AliceInput
{
	/// <summary>
	/// キーボード
	/// </summary>
	class IKeyboard
	{
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="directInput_">ダイレクトインプット</param>
		/// <param name="hwnd_">ウィンドウハンドル</param>
		virtual void Initialize(void* directInput_, void* hwnd_) = 0;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerKey(uint32_t key_)= 0;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool TriggerKey(Keys key_) = 0;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		virtual bool CheckKey(uint32_t key_) = 0;

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

		IKeyboard() = default;
		virtual ~IKeyboard() = default;
	};

	/// <summary>
	/// キーボードの生成(ユニーク)
	/// </summary>
	/// <param name="directInput_">ダイレクトインプット</param>
	/// <param name="hwnd_">ウィンドウハンドル</param>
	/// <returns>生成されたポインタ</returns>
	std::unique_ptr<IKeyboard> CreateUniqueKeyboard(void* directInput_, void* hwnd_);

	/// <summary>
	/// キーボードの生成(シェアード)
	/// </summary>
	/// <param name="directInput_">ダイレクトインプット</param>
	/// <param name="hwnd_">ウィンドウハンドル</param>
	/// <returns>生成されたポインタ</returns>
	std::shared_ptr<IKeyboard> CreateSharedKeyboard(void* directInput_, void* hwnd_);
}