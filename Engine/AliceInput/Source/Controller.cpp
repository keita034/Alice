#include<Controller.h>

#include <xinput.h>
#pragma comment (lib, "xinput.lib")

namespace AliceInput
{
	class Controller : public IController
	{
	private:

		//最高入力強度
		const float STICK_INPUT_MAX = 32768.0f;

		//コントローラー振動長さ(フレーム数)
		int32_t shakeTimer = 0;

		//コントローラー振動強さ
		float shakePower = 0.0f;

		XINPUT_STATE xinputState{};
		XINPUT_STATE oldXinputState{};

		int32_t PADING;

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
		bool TriggerButton(ControllerButton button_) override;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerButton(uint32_t button_) override;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool InputButton(ControllerButton button_) override;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool InputButton(uint32_t button_) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerReleaseButton(ControllerButton button_) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerReleaseButton(uint32_t button_) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(ControllerButton button_) override;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(uint32_t button_) override;

		/// <summary>
		/// コントローラースティックのトリガー入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool TriggerStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラースティックの入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool InputStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラースティックの離した瞬間
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool TriggerReleaseStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラーの左スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラーの右スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;

		/// <summary>
		/// コントローラーを振動させる
		/// </summary>
		/// <param name="power">振動の強さ0.0f～1.0f</param>
		/// <param name="span">振動の時間フレーム</param>
		void ShakeController(float power, int32_t span) override;

	private:
		//コピーコンストラクタ・代入演算子削除
		Controller& operator=(const Controller&) = delete;
		Controller(const Controller&) = delete;

		//デッドゾーンに入っているか
		bool StickInDeadZone(AliceMathF::Vector2& stick_, const AliceMathF::Vector2& deadRate);
	};


	bool Controller::StickInDeadZone(AliceMathF::Vector2& stick_, const AliceMathF::Vector2& DeadRate)
	{
		bool lX = false;
		bool lY = false;

		if ((stick_.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.x
			&& stick_.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.x))
		{
			stick_.x = 0.0f;
			lX = true;
		}
		if ((stick_.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.y
			&& stick_.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * DeadRate.y))
		{
			stick_.y = 0.0f;
			lY = true;
		}

		if (lX && lY)
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
		DWORD lResult = XInputGetState(0, &xinputState);

		if (lResult == ERROR_SUCCESS)
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

				XInputSetState(lResult, &vibration);
			}
		}
		else
		{
			//コントローラーが接続されていない
		}
	}

	bool Controller::InputButton(ControllerButton button_)
	{
		if (button_ == ControllerButton::LT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
		}
		else if (button_ == ControllerButton::RT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
		}
		else
		{
			return xinputState.Gamepad.wButtons & static_cast<DWORD>(button_);
		}
	}

	bool Controller::InputButton(uint32_t button_)
	{
		return InputButton(static_cast<ControllerButton>(button_));
	}

	bool Controller::TriggerButton(ControllerButton button_)
	{
		if (button_ == ControllerButton::LT)
		{
			return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && InputButton(button_);
		}
		else if (button_ == ControllerButton::RT)
		{
			return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && InputButton(button_);
		}
		else
		{
			return !(oldXinputState.Gamepad.wButtons & static_cast<DWORD>(button_)) && InputButton(button_);
		}
	}

	bool Controller::TriggerButton(uint32_t button_)
	{
		return TriggerButton(static_cast<ControllerButton>(button_));
	}

	bool Controller::TriggerReleaseButton(ControllerButton button_)
	{
		if (button_ == ControllerButton::LT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bLeftTrigger && !InputButton(button_);
		}
		else if (button_ == ControllerButton::RT)
		{
			return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bRightTrigger && !InputButton(button_);
		}
		else
		{
			return (oldXinputState.Gamepad.wButtons & static_cast<DWORD>(button_)) && !InputButton(button_);
		}
	}

	bool Controller::TriggerReleaseButton(uint32_t button_)
	{
		return TriggerReleaseButton(static_cast<ControllerButton>(button_));

	}

	bool Controller::ReleaseButton(ControllerButton button)
	{
		return !InputButton(button);
	}

	bool Controller::ReleaseButton(uint32_t button)
	{
		return ReleaseButton(static_cast<ControllerButton>(button));
	}

	bool Controller::TriggerStick(ControllerStick stickInput_, float deadRange_, const AliceMathF::Vector2& deadRate_)
	{
		AliceMathF::Vector2 lStickOldVec;
		AliceMathF::Vector2 lStickVec;

		uint32_t lStickInputNum = static_cast<uint32_t>(stickInput_);

		bool lIsLeftStick = stickInput_ <= ControllerStick::LRIGHT;

		if (lIsLeftStick)
		{
			lStickOldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
			lStickVec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
		}
		else
		{
			lStickOldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
			lStickVec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
		}

		if (!StickInDeadZone(lStickOldVec, deadRate_)|| StickInDeadZone(lStickVec, deadRate_))
		{
			return false;
		}

		bool lResult = false;

		if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LUP)
		{
			lResult = !(deadRange_ < (lStickOldVec.y / STICK_INPUT_MAX)) && deadRange_ < (lStickVec.y / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LDOWN)
		{
			lResult = !(lStickOldVec.y / STICK_INPUT_MAX < -deadRange_) && lStickVec.y / STICK_INPUT_MAX < -deadRange_;
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LRIGHT)
		{
			lResult = !(deadRange_ < (lStickOldVec.x / STICK_INPUT_MAX)) && deadRange_ < (lStickVec.x / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LLEFT)
		{
			lResult = !(lStickOldVec.x / STICK_INPUT_MAX < -deadRange_) && lStickVec.x / STICK_INPUT_MAX < -deadRange_;
		}
		else
		{
			assert(0);
		}

		return lResult;
	}

	bool Controller::InputStick(ControllerStick stickInput_, float deadRange_, const AliceMathF::Vector2& deadRate_)
	{
		AliceMathF::Vector2 lStickVec;

		uint32_t lStickInputNum = static_cast<uint32_t>(stickInput_);

		bool lIsLeftStick = stickInput_ <= ControllerStick::LRIGHT;

		if (lIsLeftStick)
		{
			lStickVec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
		}
		else
		{
			lStickVec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
		}

		if (StickInDeadZone(lStickVec, deadRate_))
		{
			return false;
		}

		if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LUP)
		{
			return deadRange_ < (lStickVec.y / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LDOWN)
		{
			return lStickVec.y / STICK_INPUT_MAX < -deadRange_;
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LRIGHT)
		{
			return deadRange_ < (lStickVec.x / STICK_INPUT_MAX);
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LLEFT)
		{
			return  lStickVec.x / STICK_INPUT_MAX < -deadRange_;
		}

		assert(0);
		return false;
	}

	bool Controller::TriggerReleaseStick(ControllerStick stickInput_, float deadRange_, const AliceMathF::Vector2& deadRate_)
	{
		AliceMathF::Vector2 lStickOldVec;
		AliceMathF::Vector2 lStickVec;

		uint32_t lStickInputNum = static_cast<uint32_t>(stickInput_);

		bool lIsLeftStick = stickInput_ <= ControllerStick::LRIGHT;

		if (lIsLeftStick)
		{
			lStickOldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
			lStickVec = AliceMathF::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
		}
		else
		{
			lStickOldVec = AliceMathF::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
			lStickVec = AliceMathF::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
		}

		if (!StickInDeadZone(lStickOldVec, deadRate_) || StickInDeadZone(lStickVec, deadRate_))
		{
			return false;
		}

		bool lResult = false;

		if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LUP)
		{
			lResult = deadRange_ < (lStickOldVec.y / STICK_INPUT_MAX) && !(deadRange_ < (lStickVec.y / STICK_INPUT_MAX));
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LDOWN)
		{
			lResult = lStickOldVec.y / STICK_INPUT_MAX < -deadRange_ && !(lStickVec.y / STICK_INPUT_MAX < -deadRange_);
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LRIGHT)
		{
			lResult = deadRange_ < (lStickOldVec.x / STICK_INPUT_MAX) && !(deadRange_ < (lStickVec.x / STICK_INPUT_MAX));
		}
		else if (static_cast<ControllerStick>(lStickInputNum % 4) == ControllerStick::LLEFT)
		{
			lResult = lStickOldVec.x / STICK_INPUT_MAX < -deadRange_ && !(lStickVec.x / STICK_INPUT_MAX < -deadRange_);
		}
		else
		{
			assert(0);
		}

		return lResult;
	}

	AliceMathF::Vector2 Controller::GetLeftStickVec(const AliceMathF::Vector2& deadRate)
	{
		AliceMathF::Vector2 lResult(static_cast<float>(xinputState.Gamepad.sThumbLX), static_cast<float>(-xinputState.Gamepad.sThumbLY));
		StickInDeadZone(lResult, deadRate);
		return lResult / STICK_INPUT_MAX;
	}

	AliceMathF::Vector2 Controller::GetRightStickVec(const AliceMathF::Vector2& deadRate)
	{
		AliceMathF::Vector2 lResult(static_cast<float>(xinputState.Gamepad.sThumbRX), static_cast<float>(-xinputState.Gamepad.sThumbRY));
		StickInDeadZone(lResult, deadRate);
		return lResult / STICK_INPUT_MAX;;
	}

	void Controller::ShakeController(float power, int32_t span)
	{
		if (!(0 < power && power <= 1.0f))
		{
			assert(0);
		}

		shakePower = power;
		shakeTimer = span;
	}

	std::unique_ptr<IController> CreateUniqueController()
	{
		std::unique_ptr<IController>lController = std::make_unique<Controller>();
		return std::move(lController);
	}

	std::shared_ptr<IController> CreateSharedController()
	{
		std::shared_ptr<IController>lController = std::make_shared<Controller>();
		return lController;
	}

}