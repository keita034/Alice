#include<Keyboard.h>

#pragma warning(push)
#pragma warning(disable: 4005)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)

#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include<dinput.h>
#include<wrl.h>
#include<Windows.h>
#include<cassert>
#include<stdint.h>
#include<array>
#pragma warning(pop)

namespace AliceInput
{
	class Keyboard : public IKeyboard
	{
	private:
		IDirectInputDevice8* directInputDevice;

		//1フレーム前のキーボードの状態
		std::array<uint8_t, 256>oldKeys;
		//現フレームのキーボードの状態
		std::array<uint8_t, 256>keys;

	public:
		void Initialize(void* directInput_, void* hwnd_)override;
		void Update()override;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(uint32_t key_)override;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(Keys key_)override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(uint32_t key_)override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(Keys key_)override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(uint32_t key_)override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(Keys key_)override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(uint32_t key_)override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(Keys key_)override;
	};

	void Keyboard::Initialize(void* directInput_, void* hwnd_)
	{
		HRESULT lResult;
		IDirectInput8* lDirectInput = static_cast<IDirectInput8*>(directInput_);
		HWND* lHwnd = static_cast<HWND*>(hwnd_);

		//キーボードデバイスの生成
		lResult = lDirectInput->CreateDevice(GUID_SysKeyboard, &directInputDevice, NULL);
		assert(SUCCEEDED(lResult));

		//入力データ形式のセット
		lResult = directInputDevice->SetDataFormat(&c_dfDIKeyboard);//標準形式
		assert(SUCCEEDED(lResult));

		//排他制御レベルのセット
		//フォアグラウンド（前面）の入力を受け付け
		lResult = directInputDevice->SetCooperativeLevel(*lHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(lResult));
	}

	void Keyboard::Update()
	{
		uint32_t lKeysDataSize = static_cast<uint32_t>(sizeof(uint8_t) * oldKeys.size());

		//1フレーム前のキーボードの状態を保存
		memcpy(oldKeys.data(), keys.data(), lKeysDataSize);

		//キーボード情報の取得開始
		directInputDevice->Acquire();
		directInputDevice->GetDeviceState(lKeysDataSize, keys.data());
	}

	bool Keyboard::TriggerKey(uint32_t key_)
	{
		return keys[key_] && !oldKeys[key_];
	}

	bool Keyboard::TriggerKey(Keys key_)
	{
		uint32_t lKeyIndex = static_cast<uint32_t>(key_);

		return TriggerKey(lKeyIndex);
	}

	bool Keyboard::CheckKey(uint32_t key_)
	{
		return static_cast<bool>(keys[key_]);
	}

	bool Keyboard::CheckKey(Keys key_)
	{
		uint32_t lKeyIndex = static_cast<uint32_t>(key_);

		return CheckKey(lKeyIndex);
	}

	bool Keyboard::TriggerReleaseKey(uint32_t key_)
	{
		return !keys[key_] && oldKeys[key_];
	}

	bool Keyboard::TriggerReleaseKey(Keys key_)
	{
		uint32_t lKeyIndex = static_cast<uint32_t>(key_);

		return TriggerReleaseKey(lKeyIndex);
	}

	bool Keyboard::ReleaseKey(uint32_t key_)
	{
		return !keys[key_] && !oldKeys[key_];
	}

	bool Keyboard::ReleaseKey(Keys key_)
	{
		uint32_t lKeyIndex = static_cast<uint32_t>(key_);
		return ReleaseKey(lKeyIndex);
	}

	std::unique_ptr<IKeyboard> CreateUniqueKeyboard(void* directInput_, void* hwnd_)
	{
		std::unique_ptr<IKeyboard>lKeyboard = std::make_unique<Keyboard>();
		lKeyboard->Initialize(directInput_, hwnd_);
		return std::move(lKeyboard);
	}

	std::shared_ptr<IKeyboard> CreateSharedKeyboard(void* directInput_, void* hwnd_)
	{
		std::shared_ptr<IKeyboard>lKeyboard = std::make_shared<Keyboard>();
		lKeyboard->Initialize(directInput_, hwnd_);
		return lKeyboard;
	}
}
