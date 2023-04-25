#include<Controller.h>

#include <xinput.h>
#pragma comment (lib, "xinput.lib")

namespace AliceInput
{
	class Controller : public IController
	{
	private:

		XINPUT_STATE xinputState{};
		XINPUT_STATE oldXinputState{};

		//コントローラー振動強さ
		float shakePower = 0.0f;
		//コントローラー振動長さ(フレーム数)
		int shakeTimer = 0;

		//最高入力強度
		const float STICK_INPUT_MAX = 32768.0f;
		char PADING[4]{};

		//デッドゾーンに入っているか(DeadRate : デッドゾーン判定の度合い、1.0fだとデフォルト)
		bool StickInDeadZone(AliceMathF::Vector2& thumb, const AliceMathF::Vector2& deadRate);


	public:
		//コンストラクタ・デストラクタ
		Controller() = default;
		~Controller() = default;

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerButton(ControllerButton button) override;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerButton(uint32_t button) override;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool InputButton(ControllerButton button) override;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool InputButton(uint32_t button) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerReleaseButton(ControllerButton button) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerReleaseButton(uint32_t button) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(ControllerButton button) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(uint32_t button) override;

		/// <summary>
		/// コントローラースティックのトリガー入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool TriggerStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラースティックの入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool InputStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラースティックの離した瞬間
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool TriggerReleaseStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラーの左スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラーの右スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラーを振動させる
		/// </summary>
		/// <param name="power">振動の強さ0.0f～1.0f</param>
		/// <param name="span">振動の時間フレーム</param>
		void ShakeController(const float& power, const int& span) override;

	private:
		//コピーコンストラクタ・代入演算子削除
		Controller& operator=(const Controller&) = delete;
		Controller(const Controller&) = delete;
	};


	bool Controller::StickInDeadZone(AliceMathF::Vector2& Thumb, const AliceMathF::Vector2& DeadRate)
	{
		bool x = false;
		bool y = false;

		if ((Thumb.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.x
			&& Thumb.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.x))
		{
			Thumb.x = 0.0f;
			x = true;
		}
		if ((Thumb.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.y
			&& Thumb.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.y))
		{
			Thumb.y = 0.0f;
			y = true;
		}

		if (x && y)
		{
			return true;
		}

		return false;
	}

	void Controller::Update()
	{
		oldXinputState = xinputState;
		ZeroMemory(&xinputState, sizeof(XINPUT_STATE));

		//コントローラー取得
		DWORD dwResult = XInputGetState(0, &xinputState);

		if (dwResult == ERROR_SUCCESS)
		{
			//コントローラーが接続されている
			if (0 < shakeTimer)
			{
				shakeTimer--;
				XINPUT_VIBRATION vibration;
				ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

				if (shakeTimer == 0)
				{
					vibration.wLeftMotorSpeed = static_cast<WORD>(0.0f); // use any value between 0-65535 here
					vibration.wRightMotorSpeed = static_cast<WORD>(0.0f); // use any value between 0-65535 here
				}
				else
				{
					vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * shakePower); // use any value between 0-65535 here
					vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * shakePower); // use any value between 0-65535 here
				}

				XInputSetState(dwResult, &vibration);
			}
		}
		else
		{
			//コントローラーが接続されていない
		}
	}

	bool Controller::TriggerButton(ControllerButton button)
	{
		if (button == ControllerButton::LT)
		{
			return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && InputButton(button);
		}
		else if (button == ControllerButton::RT)
		{
			return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && InputButton(button);
		}
		else
		{
			return !(oldXinputState.Gamepad.wButtons & static_cast<DWORD>(button)) && InputButton(button);
		}
	}

	bool Controller::TriggerButton(uint32_t button)
	{
		if (button == 0x4000)
		{
			return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && InputButton(button);
		}
		else if (button == 0x8000)
		{
			return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && InputButton(button);
		}
		else
		{
			return !(oldXinputState.Gamepad.wButtons & button) && InputButton(button);
		}
	}

	bool Controller::InputButton(ControllerButton button)
	{
		if (button == ControllerButton::LT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
		}
		else if (button == ControllerButton::RT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
		}
		else
		{
			return xinputState.Gamepad.wButtons & static_cast<DWORD>(button);
		}
	}

	bool Controller::InputButton(uint32_t button)
	{
		if (button == 0x4000)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
		}
		else if (button == 0x8000)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
		}
		else
		{
			return xinputState.Gamepad.wButtons & button;
		}
	}

	bool Controller::TriggerReleaseButton(ControllerButton button)
	{
		if (button == ControllerButton::LT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bLeftTrigger && !InputButton(button);
		}
		else if (button == ControllerButton::RT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bRightTrigger && !InputButton(button);
		}
		else
		{
			return (oldXinputState.Gamepad.wButtons & static_cast<DWORD>(button)) && !InputButton(button);
		}
	}

	bool Controller::TriggerReleaseButton(uint32_t button)
	{
		if (button == 0x4000)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bLeftTrigger && !InputButton(button);
		}
		else if (button == 0x8000)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bRightTrigger && !InputButton(button);
		}
		else
		{
			return (oldXinputState.Gamepad.wButtons & button) && !InputButton(button);
		}
	}

	bool Controller::ReleaseButton(ControllerButton button)
	{
		return !InputButton(button);
	}

	bool Controller::ReleaseButton(uint32_t button)
	{
		return !InputButton(button);
	}

	bool Controller::TriggerStick(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
	{
		AliceMathF::Vector2 oldVec;
		AliceMathF::Vector2 vec;

		uint32_t stickInputNum = static_cast<uint32_t>(stickInput);

		bool isLeftStick = stickInput <= ControllerStick::LRIGHT;
		if (isLeftStick)
		{
			oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
			vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
		}
		else
		{
			oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
			vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
		}

		if (!StickInDeadZone(oldVec, deadRate))
		{
			return false;
		}

		if (StickInDeadZone(vec, deadRate))
		{
			return false;
		}

		bool result = false;

		if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LUP)
		{
			result = !(deadRange < (oldVec.y / STICK_INPUT_MAX)) && deadRange < (vec.y / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LDOWN)
		{
			result = !(oldVec.y / STICK_INPUT_MAX < -deadRange) && vec.y / STICK_INPUT_MAX < -deadRange;
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LRIGHT)
		{
			result = !(deadRange < (oldVec.x / STICK_INPUT_MAX)) && deadRange < (vec.x / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LLEFT)
		{
			result = !(oldVec.x / STICK_INPUT_MAX < -deadRange) && vec.x / STICK_INPUT_MAX < -deadRange;
		}
		else
		{
			assert(0);
		}

		return result;
	}

	bool Controller::InputStick(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
	{
		AliceMathF::Vector2 vec;

		uint32_t stickInputNum = static_cast<uint32_t>(stickInput);

		bool isLeftStick = stickInput <= ControllerStick::LRIGHT;

		if (isLeftStick)
		{
			vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
		}
		else
		{
			vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
		}

		if (StickInDeadZone(vec, deadRate))return false;

		if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LUP)
		{
			return deadRange < (vec.y / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LDOWN)
		{
			return  vec.y / STICK_INPUT_MAX < -deadRange;
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LRIGHT)
		{
			return deadRange < (vec.x / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LLEFT)
		{
			return  vec.x / STICK_INPUT_MAX < -deadRange;
		}

		assert(0);
		return false;
	}

	bool Controller::TriggerReleaseStick(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
	{
		AliceMathF::Vector2 oldVec;
		AliceMathF::Vector2 vec;

		uint32_t stickInputNum = static_cast<uint32_t>(stickInput);

		bool isLeftStick = stickInput <= ControllerStick::LRIGHT;

		if (isLeftStick)
		{
			oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
			vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
		}
		else
		{
			oldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
			vec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
		}

		if (!StickInDeadZone(oldVec, deadRate))
		{
			return false;
		}

		if (StickInDeadZone(vec, deadRate))
		{
			return false;
		}

		bool result = false;

		if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LUP)
		{
			result = deadRange < (oldVec.y / STICK_INPUT_MAX) && !(deadRange < (vec.y / STICK_INPUT_MAX));
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LDOWN)
		{
			result = oldVec.y / STICK_INPUT_MAX < -deadRange && !(vec.y / STICK_INPUT_MAX < -deadRange);
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LRIGHT)
		{
			result = deadRange < (oldVec.x / STICK_INPUT_MAX) && !(deadRange < (vec.x / STICK_INPUT_MAX));
		}
		else if (static_cast<ControllerStick>(stickInputNum % 4) == ControllerStick::LLEFT)
		{
			result = oldVec.x / STICK_INPUT_MAX < -deadRange && !(vec.x / STICK_INPUT_MAX < -deadRange);
		}
		else
		{
			assert(0);
		}

		return result;
	}

	AliceMathF::Vector2 Controller::GetLeftStickVec(const AliceMathF::Vector2& deadRate)
	{

		AliceMathF::Vector2 result(static_cast<float>(xinputState.Gamepad.sThumbLX), static_cast<float>(-xinputState.Gamepad.sThumbLY));
		StickInDeadZone(result, deadRate);
		return result / STICK_INPUT_MAX;
	}

	AliceMathF::Vector2 Controller::GetRightStickVec(const AliceMathF::Vector2& deadRate)
	{
		AliceMathF::Vector2 result(static_cast<float>(xinputState.Gamepad.sThumbRX), static_cast<float>(-xinputState.Gamepad.sThumbRY));
		StickInDeadZone(result, deadRate);
		return result / STICK_INPUT_MAX;;
	}

	void Controller::ShakeController(const float& power, const int& span)
	{
		if (!(0 < power && power <= 1.0f))
		{
			assert(0);
		}

		shakePower = power;
		shakeTimer = span;
	}

	IController* CreateController()
	{
		IController* controller = new Controller();

		return controller;
	}

}