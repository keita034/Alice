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
#include<Windows.h>
#include<cassert>
#include<stdint.h>
#pragma warning(pop)

#include<Input.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace AliceInput
{
	class Input : public IInput
	{
	private:
		Microsoft::WRL::ComPtr<IDirectInput8> directInput;

		//キーボードデバイス
		IKeyboard* keyboard = nullptr;

		//マウス
		IMouse* mouse = nullptr;

		IController* controller = nullptr;

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(void* hwnd, void* hInstance) override;

		/// <summary>
		/// 更新
		/// </summary>
		void Update(float width, float height) override;

		//キーボード

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(const uint32_t key) override;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(const Keys key) override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(const uint32_t key) override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(const Keys key) override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(const uint32_t key) override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(const Keys key) override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(const uint32_t key) override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(const Keys key) override;

		//マウス

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		bool TriggerMouseButton(MouseButton button) override;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		bool InputMouseButton(MouseButton button) override;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		bool TriggerReleaseMouseButton(MouseButton button) override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetMousePos() override;;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetWorldMousePos() override;;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetLocalMousePos() override;;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector3& GetMouseMove() override;;

		//コントローラ-

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerButton(ControllerButton button) override;;

		/// <summary>
		/// コントローラーボタンのトリガー入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerButton(uint32_t button) override;;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool InputButton(ControllerButton button) override;;

		/// <summary>
		/// コントローラーボタンの入力
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool InputButton(uint32_t button) override;;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerReleaseButton(ControllerButton button) override;;

		/// <summary>
		/// コントローラーボタンの離した瞬間
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool TriggerReleaseButton(uint32_t button) override;;

		/// <summary>
		/// コントローラーボタンの離した
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(ControllerButton button) override;;

		/// <summary>
		/// コントローラーボタンの離した
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(uint32_t button) override;;

		/// <summary>
		/// コントローラースティックのトリガー入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool TriggerStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラースティックの入力
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool InputStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラースティックの離した瞬間
		/// </summary>
		/// <param name="stickInput">コントローラースティック方向</param>
		/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		bool TriggerReleaseStick(ControllerStick stickInput, const float& deadRange = 0.3f, const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラーの左スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラーの右スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラーを振動させる
		/// </summary>
		/// <param name="power">振動の強さ0.0f～1.0f</param>
		/// <param name="span">振動の時間フレーム</param>
		void ShakeController(const float& power, const int& span) override;;
	};

	void Input::Initialize(void* hwnd, void* hInstance)//初期化
	{

		HRESULT result;

		HINSTANCE* instance = static_cast<HINSTANCE*>(hInstance);

		//DirectInputの初期化
		result = DirectInput8Create(*instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)directInput.ReleaseAndGetAddressOf(), nullptr);
		assert(SUCCEEDED(result));

		//キーボードデバイスの初期化
		keyboard = CreateKeyboard(directInput.Get(), hwnd);

		//マウスデバイスの初期化
		mouse = CreateMouse(directInput.Get(), hwnd);

		//コントローラーの初期化
		controller = CreateController();
	}

	void Input::Update(float width, float height)//更新処理
	{
		//キーボードデバイスの更新
		keyboard->Update();

		//マウスデバイスの更新
		mouse->Update(width, height);

		//コントローラーデバイスの更新
		controller->Update();
	}

	bool Input::TriggerKey(const uint32_t key)
	{
		return keyboard->TriggerKey(key);
	}

	bool Input::TriggerKey(const Keys key)
	{
		return keyboard->TriggerKey(key);
	}

	bool Input::CheckKey(const uint32_t key)
	{
		return keyboard->CheckKey(key);
	}

	bool Input::CheckKey(const Keys key)
	{
		return keyboard->CheckKey(key);
	}

	bool Input::TriggerReleaseKey(const uint32_t key)
	{
		return keyboard->TriggerReleaseKey(key);
	}

	bool Input::TriggerReleaseKey(const Keys key)
	{
		return keyboard->TriggerReleaseKey(key);
	}

	bool Input::ReleaseKey(const uint32_t key)
	{
		return keyboard->ReleaseKey(key);
	}

	bool Input::ReleaseKey(const Keys key)
	{
		return keyboard->ReleaseKey(key);
	}

	bool Input::TriggerMouseButton(MouseButton button)
	{
		return mouse->TriggerMouseButton(button);
	}

	bool Input::InputMouseButton(MouseButton button)
	{
		return mouse->InputMouseButton(button);
	}

	bool Input::TriggerReleaseMouseButton(MouseButton button)
	{
		return mouse->TriggerReleaseMouseButton(button);
	}

	const AliceMathF::Vector2& Input::GetMousePos()
	{
		return mouse->GetMousePos();
	}

	const AliceMathF::Vector2& Input::GetWorldMousePos()
	{
		return mouse->GetWorldMousePos();
	}

	const AliceMathF::Vector2& Input::GetLocalMousePos()
	{
		return mouse->GetLocalMousePos();
	}

	const AliceMathF::Vector3& Input::GetMouseMove()
	{
		return mouse->GetMouseMove();
	}

	bool Input::TriggerButton(ControllerButton button)
	{
		return controller->TriggerButton(button);
	}

	bool Input::TriggerButton(uint32_t button)
	{
		return controller->TriggerButton(button);
	}

	bool Input::InputButton(ControllerButton button)
	{
		return controller->InputButton(button);
	}

	bool Input::InputButton(uint32_t button)
	{
		return controller->InputButton(button);
	}

	bool Input::TriggerReleaseButton(ControllerButton button)
	{
		return controller->TriggerReleaseButton(button);
	}

	bool Input::TriggerReleaseButton(uint32_t button)
	{
		return controller->TriggerReleaseButton(button);
	}

	bool Input::ReleaseButton(ControllerButton button)
	{
		return controller->ReleaseButton(button);
	}

	bool Input::ReleaseButton(uint32_t button)
	{
		return controller->ReleaseButton(button);
	}

	bool Input::TriggerStick(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
	{
		return controller->TriggerStick(stickInput, deadRange, deadRate);
	}

	bool Input::InputStick(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
	{
		return controller->InputStick(stickInput, deadRange, deadRate);
	}

	bool Input::TriggerReleaseStick(ControllerStick stickInput, const float& deadRange, const AliceMathF::Vector2& deadRate)
	{
		return controller->TriggerReleaseStick(stickInput, deadRange, deadRate);
	}

	AliceMathF::Vector2 Input::GetLeftStickVec(const AliceMathF::Vector2& deadRate)
	{
		return controller->GetLeftStickVec(deadRate);
	}

	AliceMathF::Vector2 Input::GetRightStickVec(const AliceMathF::Vector2& deadRate)
	{
		return controller->GetRightStickVec(deadRate);
	}

	void Input::ShakeController(const float& power, const int& span)
	{
		controller->ShakeController(power, span);
	}

	IInput* CreateInput(void* hwnd, void* hInstance)
	{
		IInput* input = new Input();
		input->Initialize(hwnd, hInstance);
		return input;
	}
}