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
#pragma warning(pop)

namespace AliceInput
{
	class Keyboard : public IKeyboard
	{
	private:
		IDirectInputDevice8* directInputDve;
		BYTE oldKeys[256] = {};
		BYTE keys[256] = {};
	public:
		void Initialize(void* dInput, void* hwnd)override;
		void Update()override;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(const uint32_t key)override;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(const Keys key)override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(const uint32_t key)override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(const Keys key)override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(const uint32_t key)override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(const Keys key)override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(const uint32_t key)override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(const Keys key)override;
	};

	void Keyboard::Initialize(void* directInput, void* hwnd)
	{
		HRESULT result;
		IDirectInput8* input = static_cast<IDirectInput8*>(directInput);
		HWND* hnd = static_cast<HWND*>(hwnd);

		//キーボードデバイスの生成
		result = input->CreateDevice(GUID_SysKeyboard, &directInputDve, NULL);
		assert(SUCCEEDED(result));

		//入力データ形式のセット
		result = directInputDve->SetDataFormat(&c_dfDIKeyboard);//標準形式
		assert(SUCCEEDED(result));

		//排他制御レベルのセット
		//フォアグラウンド（前面）の入力を受け付け
		result = directInputDve->SetCooperativeLevel(*hnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(result));
	}

	void Keyboard::Update()
	{
		//1フレーム前のキーボードの状態を保存
		memcpy(oldKeys, keys, sizeof(BYTE) * _countof(oldKeys));

		//キーボード情報の取得開始
		directInputDve->Acquire();
		directInputDve->GetDeviceState(sizeof(keys), keys);
	}

	bool Keyboard::TriggerKey(const uint32_t key)
	{
		return keys[key] && !oldKeys[key];
	}

	bool Keyboard::TriggerKey(const Keys key)
	{
		uint32_t keyIndex = static_cast<uint32_t>(key);

		return keys[keyIndex] && !oldKeys[keyIndex];
	}

	bool Keyboard::CheckKey(const uint32_t key)
	{
		return static_cast<bool>(keys[key]);
	}

	bool Keyboard::CheckKey(const Keys key)
	{
		uint32_t keyIndex = static_cast<uint32_t>(key);

		return static_cast<bool>(keys[keyIndex]);
	}

	bool Keyboard::TriggerReleaseKey(const uint32_t key)
	{
		return !keys[key] && oldKeys[key];
	}

	bool Keyboard::TriggerReleaseKey(const Keys key)
	{
		uint32_t keyIndex = static_cast<uint32_t>(key);

		return !keys[keyIndex] && oldKeys[keyIndex];
	}

	bool Keyboard::ReleaseKey(const uint32_t key)
	{
		return !keys[key] && !oldKeys[key];
	}

	bool Keyboard::ReleaseKey(const Keys key)
	{
		uint32_t keyIndex = static_cast<uint32_t>(key);
		return !keys[keyIndex] && !oldKeys[keyIndex];
	}


	IKeyboard* CreateKeyboard(void* directInput, void* hwnd)
	{
		IKeyboard* keyboard = new Keyboard();
		keyboard->Initialize(directInput, hwnd);
		return keyboard;
	}

}
