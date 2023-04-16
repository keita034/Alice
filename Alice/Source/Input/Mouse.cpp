#include<Mouse.h>
#include<WindowsApp.h>
void Mouse::Initialize(IDirectInput8* dinput)
{
	HRESULT result;

	//マウスデバイスの生成
	result = dinput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
	assert(SUCCEEDED(result));

	result = mouseDev->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));

	result = mouseDev->SetCooperativeLevel(*WindowsApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Mouse::Update()
{
	HRESULT result;

	//マウス
	result = mouseDev->Acquire();
	oldMouseState = mouseState;
	result = mouseDev->GetDeviceState(sizeof(mouseState), &mouseState);

	float width = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().width);
	float height = static_cast<float>(WindowsApp::GetInstance()->GetWindowSize().height);

	POINT point;
	LPPOINT p = &point;

	if (!GetCursorPos(p))
	{
		assert(0);
	}

	ScreenToClient(*WindowsApp::GetInstance()->GetHwnd(), p);

	mousePos.x = static_cast<float>(p->x);
	mousePos.x = AliceMathF::Clamp(mousePos.x, 0.0f, width);
	mousePos.y = static_cast<float>(p->y);
	mousePos.y = AliceMathF::Clamp(mousePos.y, 0.0f, height);
}

bool Mouse::MouseButtonTrigger(MouseButton button)
{
	return (!oldMouseState.rgbButtons[button] && mouseState.rgbButtons[button]);
}

bool Mouse::MouseButtonInput(MouseButton button)
{
	return (bool)(mouseState.rgbButtons[button]);
}

bool Mouse::MouseButtonOffTrigger(MouseButton button)
{
	return (oldMouseState.rgbButtons[button] && !mouseState.rgbButtons[button]);
}

const AliceMathF::Vector2 Mouse::GetMousePos() const
{
	return mousePos;
}

const AliceMathF::Vector3 Mouse::GetMouseMove()
{
	AliceMathF::Vector3 result = { static_cast<float>(mouseState.lX), static_cast<float>(mouseState.lY), static_cast<float>(mouseState.lZ) };
	return result;
}
