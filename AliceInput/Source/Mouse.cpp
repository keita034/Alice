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
		Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDev;
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
		void Initialize(void* dinput, void* hwnd) override;

		/// <summary>
		/// 更新
		/// </summary>
		void Update(float width, float height)override;

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		bool TriggerMouseButton(MouseButton button)override;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		bool InputMouseButton(MouseButton button)override;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		bool TriggerReleaseMouseButton(MouseButton button)override;

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

	void Mouse::Initialize(void* dinput, void* hwnd)
	{
		HRESULT result;

		IDirectInput8* directInput = static_cast<IDirectInput8*>(dinput);
		handl = static_cast<HWND*>(hwnd);

		//マウスデバイスの生成
		result = directInput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
		assert(SUCCEEDED(result));

		result = mouseDev->SetDataFormat(&c_dfDIMouse2);
		assert(SUCCEEDED(result));

		result = mouseDev->SetCooperativeLevel(*handl, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(result));
	}

	void Mouse::Update(float width, float height)
	{
		HRESULT result;

		//マウス
		result = mouseDev->Acquire();
		oldMouseState = mouseState;
		result = mouseDev->GetDeviceState(sizeof(mouseState), &mouseState);

		POINT point{};
		LPPOINT p = &point;

		if (!GetCursorPos(p))
		{
			assert(0);
		}

		worldMousePos = AliceMathF::Vector2(p->x, p->y);

		ScreenToClient(*handl, p);

		localMousePos = AliceMathF::Vector2(p->x, p->y);

		mousePos.x = AliceMathF::Clamp(localMousePos.x, 0.0f, width);
		mousePos.y = AliceMathF::Clamp(localMousePos.y, 0.0f, height);
	}

	bool Mouse::TriggerMouseButton(MouseButton button)
	{
		uint32_t bottonIndex = static_cast<uint32_t>(button);

		return (!oldMouseState.rgbButtons[bottonIndex] && mouseState.rgbButtons[bottonIndex]);
	}

	bool Mouse::InputMouseButton(MouseButton button)
	{
		uint32_t bottonIndex = static_cast<uint32_t>(button);

		return (bool)(mouseState.rgbButtons[bottonIndex]);
	}

	bool Mouse::TriggerReleaseMouseButton(MouseButton button)
	{
		uint32_t bottonIndex = static_cast<uint32_t>(button);

		return (oldMouseState.rgbButtons[bottonIndex] && !mouseState.rgbButtons[bottonIndex]);
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
		mouseMove = { mouseState.lX, mouseState.lY, mouseState.lZ };

		return mouseMove;
	}

	IMouse* CreateMouse(void* directInput, void* hwnd)
	{
		IMouse* mouse = new Mouse();
		mouse->Initialize(directInput, hwnd);

		return mouse;
	}

}