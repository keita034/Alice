#include<Mouse.h>

#pragma once
#pragma warning(push)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
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

namespace AliceInput
{
	class Mouse : public IMouse
	{
	private:
		//マウス
		Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDevice;
		DIMOUSESTATE2 mouseState = {};
		DIMOUSESTATE2 oldMouseState = {};

		AliceMathF::Vector2 mousePos;
		AliceMathF::Vector2 worldMousePos;
		AliceMathF::Vector2 localMousePos;
		AliceMathF::Vector3 mouseMove;

		char PADING[4];

		HWND* handl;

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(void* directInput_, void* hwnd_) override;

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="width_">ウィンドウ横幅</param>
		/// <param name="height_">ウィンドウ縦幅</param>
		void Update(float width_, float height_)override;

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		bool TriggerMouseButton(MouseButton button_)override;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		bool InputMouseButton(MouseButton button_)override;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		bool TriggerReleaseMouseButton(MouseButton button_)override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetMousePos()override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetWorldMousePos()override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetLocalMousePos()override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector3& GetMouseMove()override;
	};

	void Mouse::Initialize(void* directInput_, void* hwnd_)
	{
		HRESULT lResult;

		IDirectInput8* lDirectInput = static_cast<IDirectInput8*>(directInput_);
		handl = static_cast<HWND*>(hwnd_);

		//マウスデバイスの生成
		lResult = lDirectInput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
		assert(SUCCEEDED(lResult));

		lResult = mouseDevice->SetDataFormat(&c_dfDIMouse2);
		assert(SUCCEEDED(lResult));

		lResult = mouseDevice->SetCooperativeLevel(*handl, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(lResult));
	}

	void Mouse::Update(float width_, float height_)
	{
		HRESULT lResult;

		//マウス
		lResult = mouseDevice->Acquire();
		oldMouseState = mouseState;
		lResult = mouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

		POINT lPoint{};
		LPPOINT lLPoint = &lPoint;

		GetCursorPos(lLPoint);

		worldMousePos = AliceMathF::Vector2(static_cast<float>(lLPoint->x), static_cast<float>(lLPoint->y));

		ScreenToClient(*handl, lLPoint);

		localMousePos = AliceMathF::Vector2(static_cast<float>(lLPoint->x), static_cast<float>(lLPoint->y));

		mousePos.x = AliceMathF::Clamp(localMousePos.x, 0.0f, width_);
		mousePos.y = AliceMathF::Clamp(localMousePos.y, 0.0f, height_);
	}

	bool Mouse::TriggerMouseButton(MouseButton button_)
	{
		uint32_t lBottonIndex = static_cast<uint32_t>(button_);

		return (!oldMouseState.rgbButtons[lBottonIndex] && mouseState.rgbButtons[lBottonIndex]);
	}

	bool Mouse::InputMouseButton(MouseButton button_)
	{
		uint32_t lBottonIndex = static_cast<uint32_t>(button_);

		return static_cast<bool>(mouseState.rgbButtons[lBottonIndex]);
	}

	bool Mouse::TriggerReleaseMouseButton(MouseButton button_)
	{
		uint32_t lBottonIndex = static_cast<uint32_t>(button_);

		return (oldMouseState.rgbButtons[lBottonIndex] && !mouseState.rgbButtons[lBottonIndex]);
	}

	const AliceMathF::Vector2& Mouse::GetMousePos()
	{
		return mousePos;
	}

	const AliceMathF::Vector2& Mouse::GetWorldMousePos()
	{
		return worldMousePos;
	}

	const AliceMathF::Vector2& Mouse::GetLocalMousePos()
	{
		return localMousePos;
	}

	const AliceMathF::Vector3& Mouse::GetMouseMove()
	{
		mouseMove = { static_cast<float>(mouseState.lX), static_cast<float>(mouseState.lY), static_cast<float>(mouseState.lZ) };

		return mouseMove;
	}

	std::unique_ptr<IMouse> CreateUniqueMouse(void* directInput_, void* hwnd_)
	{
		std::unique_ptr<IMouse>lMouse = std::make_unique<Mouse>();
		lMouse->Initialize(directInput_, hwnd_);
		return std::move(lMouse);
	}

	std::shared_ptr<IMouse> CreateSharedMouse(void* directInput_, void* hwnd_)
	{
		std::shared_ptr<IMouse>lMouse = std::make_shared<Mouse>();
		lMouse->Initialize(directInput_, hwnd_);
		return lMouse;
	}
}