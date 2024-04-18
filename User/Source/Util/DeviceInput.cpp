#include "DeviceInput.h"

void DeviceInput::Initialize(AliceInput::IInput* input_)
{
	input = input_;
}

bool DeviceInput::InputButton(ControllerButton button_, float frame_, float* progress_)
{
	if (input->TriggerButton(button_))
	{
		holdTimes[GetIndex(button_)] = 0;
	}

	if (input->InputButton(button_))
	{
		holdTimes[GetIndex(button_)]++;
	}

	if (progress_)
	{
		*progress_ = holdTimes[GetIndex(button_)] / frame_;
	}

	if (input->TriggerReleaseButton(button_))
	{
		holdTimes[GetIndex(button_)] = 0;
	}

	if (holdTimes[GetIndex(button_)] > frame_)
	{
		return true;
	}

	return false;
}

bool DeviceInput::TriggerButton(ControllerButton button_, float frame_, float* progress_)
{
	if (input->TriggerButton(button_))
	{
		triggerHoldTimes[GetIndex(button_)] = 0;
	}

	if (input->InputButton(button_))
	{
		triggerHoldTimes[GetIndex(button_)]++;
	}

	if (progress_)
	{
		*progress_ = triggerHoldTimes[GetIndex(button_)] / frame_;
	}

	if (input->TriggerReleaseButton(button_))
	{
		if (triggerHoldTimes[GetIndex(button_)] < frame_)
		{
			triggerHoldTimes[GetIndex(button_)] = 0;
			return true;
		}
		else
		{
			triggerHoldTimes[GetIndex(button_)] = 0;
			return false;
		}
	}



	return false;
}

size_t DeviceInput::GetIndex(ControllerButton button_)
{
	switch (button_)
	{
	case ControllerButton::UP:
		return 0;
	case ControllerButton::DOWN:
		return 1;
	case ControllerButton::LEFT:
		return 2;
	case ControllerButton::RIGHT:
		return 3;
	case ControllerButton::START:
		return 4;
	case ControllerButton::BACK:
		return 5;
	case ControllerButton::LB:
		return 6;
	case ControllerButton::RB:
		return 7;
	case ControllerButton::LT:
		return 8;
	case ControllerButton::RT:
		return 9;
	case ControllerButton::LSTICK:
		return 10;
	case ControllerButton::RSTICK:
		return 11;
	case ControllerButton::A:
		return 12;
	case ControllerButton::B:
		return 13;
	case ControllerButton::X:
		return 14;
	case ControllerButton::Y:
		return 15;
	default:
		return 100;
	}
}

bool DeviceInput::NotAction()
{
	if (
		input->InputButton(ControllerButton::A) ||//ローリング、走り
		input->InputButton(ControllerButton::LB) //攻撃
		)
	{
		notActionFrame = 0;

		return false;
	}
	else
	{
		if (notActionFrame > notMaxActionFrame)
		{
			return true;
		}
		else
		{
			notActionFrame++;
		}
	}

	return false;
}
