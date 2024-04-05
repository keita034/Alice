#pragma once

#include<Input.h>

class DeviceInput
{
private:
	AliceInput::IInput* input;

	std::array<float, 16>holdTimes;
	std::array<float,16>triggerHoldTimes;

	float notActionFrame;
	float notMaxActionFrame;

public:

	DeviceInput() = default;
	~DeviceInput() = default;

	void Initialize(AliceInput::IInput* input_);

	bool InputButton(ControllerButton button_, float frame_, float* progress_ = nullptr);

	bool TriggerButton(ControllerButton button_, float frame_, float* progress_ = nullptr);

	bool NotAction();

private:

	size_t GetIndex(ControllerButton button_);
};

