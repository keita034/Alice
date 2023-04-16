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

#include<AliceMathUtility.h>
#include<WindowsApp.h>

enum MouseButton
{
	LEFT,
	RIGHT,
	CENTER
};

class Mouse
{
private:
	//マウス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDev;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	//マウスのゲーム空間内でのレイ
	AliceMathF::Vector2 mousePos;

	AliceMathF::Vector3 mouseMove;
	char PADING[4];

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(IDirectInput8* dinput);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// マウスボタンのトリガー入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseButtonTrigger(MouseButton button);

	/// <summary>
	/// マウスボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseButtonInput(MouseButton button);

	/// <summary>
	/// マウスボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool MouseButtonOffTrigger(MouseButton button);

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns>マウスの位置</returns>
	const AliceMathF::Vector2 GetMousePos()const;

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns>マウスの位置</returns>
	const AliceMathF::Vector3 GetMouseMove();
};

