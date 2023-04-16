#pragma once
#pragma warning(push)
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

#pragma warning(pop)

#include<WindowsApp.h>

class Keyboard
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE oldkey[256] = {};
	BYTE key[256] = {};
public:
	void Initialize(IDirectInput8* directInput);
	void Update();

	/// <summary>
	/// キーボードのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool TriggerPush(int keyName);

	/// <summary>
	/// キーボードの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool KeepPush(int keyName);

	/// <summary>
	/// キーボードの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool TriggerRelease(int keyName);

	/// <summary>
	/// キーボードの離してる間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離してるか</returns>
	bool KeepRelease(int keyName);
};

