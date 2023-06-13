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
#include<Keyboard.h>
#include<Mouse.h>
#include<Controller.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace AliceInput
{
	class Input : public IInput
	{
	private:
		Microsoft::WRL::ComPtr<IDirectInput8> directInput;

		//キーボードデバイス
		std::unique_ptr<IKeyboard> keyboard;

		//マウス
		std::unique_ptr<IMouse> mouse;

		std::unique_ptr<IController> controller;

	public:

		Input() = default;
		~Input() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize(void* hwnd_, void* hInstance_) override;

		/// <summary>
		/// 更新
		/// </summary>
		void Update(float width_, float height_) override;

		//キーボード

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(uint32_t key_) override;

		/// <summary>
		/// キーのトリガー入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerKey(Keys key_) override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(uint32_t key_) override;

		/// <summary>
		/// キーの入力
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool CheckKey(Keys key_) override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(uint32_t key_) override;

		/// <summary>
		/// キーの離した瞬間
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool TriggerReleaseKey(Keys key_) override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(uint32_t key_) override;

		/// <summary>
		/// キーを離してる
		/// </summary>
		/// <param name="key">チェックしたいキー</param>
		bool ReleaseKey(Keys key_) override;

		//マウス

		/// <summary>
		/// マウスボタンのトリガー入力
		/// </summary>
		bool TriggerMouseButton(MouseButton button_) override;

		/// <summary>
		/// マウスボタンの入力
		/// </summary>
		bool InputMouseButton(MouseButton button_) override;

		/// <summary>
		/// マウスボタンの離した瞬間
		/// </summary>
		bool TriggerReleaseMouseButton(MouseButton button_) override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetMousePos() override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetWorldMousePos() override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector2& GetLocalMousePos() override;

		/// <summary>
		/// マウスの位置
		/// </summary>
		/// <returns>マウスの位置</returns>
		const AliceMathF::Vector3& GetMouseMove() override;

		//コントローラ-

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
		/// コントローラーボタンの離した
		/// </summary>
		/// <param name="button">チェックしたいボタン</param>
		bool ReleaseButton(ControllerButton button_) override;

		/// <summary>
		/// コントローラーボタンの離した
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
		bool TriggerReleaseStick(ControllerStick stickInput_, float deadRange_ = 0.3f, const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラーの左スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetLeftStickVec(const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラーの右スティックのベクトル
		/// </summary>
		/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
		AliceMathF::Vector2 GetRightStickVec(const AliceMathF::Vector2& deadRate_ = { 1.0f,1.0f }) override;;

		/// <summary>
		/// コントローラーを振動させる
		/// </summary>
		/// <param name="power">振動の強さ0.0f～1.0f</param>
		/// <param name="span">振動の時間フレーム</param>
		void ShakeController(float power_, int32_t span_) override;

	private:

		//コピーコンストラクタ・代入演算子削除
		Input& operator=(const Input&) = delete;
		Input(const Input&) = delete;
	};

	void Input::Initialize(void* hwnd_, void* hInstance_)//初期化
	{

		HRESULT lResult;
		HINSTANCE* lInstance = static_cast<HINSTANCE*>(hInstance_);

		//DirectInputの初期化
		lResult = DirectInput8Create(*lInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)directInput.ReleaseAndGetAddressOf(), nullptr);
		assert(SUCCEEDED(lResult));

		//キーボードデバイスの初期化
		keyboard = CreateUniqueKeyboard(directInput.Get(), hwnd_);

		//マウスデバイスの初期化
		mouse = CreateUniqueMouse(directInput.Get(), hwnd_);

		//コントローラーの初期化
		controller = CreateUniqueController();
	}

	void Input::Update(float width_, float height_)
	{
		//キーボードデバイスの更新
		keyboard->Update();

		//マウスデバイスの更新
		mouse->Update(width_, height_);

		//コントローラーデバイスの更新
		controller->Update();
	}

	bool Input::TriggerKey(uint32_t key_)
	{
		return keyboard->TriggerKey(key_);
	}

	bool Input::TriggerKey(Keys key)
	{
		return keyboard->TriggerKey(key);
	}

	bool Input::CheckKey(uint32_t key_)
	{
		return keyboard->CheckKey(key_);
	}

	bool Input::CheckKey(Keys key_)
	{
		return keyboard->CheckKey(key_);
	}

	bool Input::TriggerReleaseKey(uint32_t key_)
	{
		return keyboard->TriggerReleaseKey(key_);
	}

	bool Input::TriggerReleaseKey(Keys key_)
	{
		return keyboard->TriggerReleaseKey(key_);
	}

	bool Input::ReleaseKey(uint32_t key_)
	{
		return keyboard->ReleaseKey(key_);
	}

	bool Input::ReleaseKey(Keys key_)
	{
		return keyboard->ReleaseKey(key_);
	}

	bool Input::TriggerMouseButton(MouseButton button_)
	{
		return mouse->TriggerMouseButton(button_);
	}

	bool Input::InputMouseButton(MouseButton button_)
	{
		return mouse->InputMouseButton(button_);
	}

	bool Input::TriggerReleaseMouseButton(MouseButton button_)
	{
		return mouse->TriggerReleaseMouseButton(button_);
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

	bool Input::TriggerButton(ControllerButton button_)
	{
		return controller->TriggerButton(button_);
	}

	bool Input::TriggerButton(uint32_t button_)
	{
		return controller->TriggerButton(button_);
	}

	bool Input::InputButton(ControllerButton button_)
	{
		return controller->InputButton(button_);
	}

	bool Input::InputButton(uint32_t button_)
	{
		return controller->InputButton(button_);
	}

	bool Input::TriggerReleaseButton(ControllerButton button_)
	{
		return controller->TriggerReleaseButton(button_);
	}

	bool Input::TriggerReleaseButton(uint32_t button_)
	{
		return controller->TriggerReleaseButton(button_);
	}

	bool Input::ReleaseButton(ControllerButton button_)
	{
		return controller->ReleaseButton(button_);
	}

	bool Input::ReleaseButton(uint32_t button_)
	{
		return controller->ReleaseButton(button_);
	}

	bool Input::TriggerStick(ControllerStick stickInput_, float deadRange_, const AliceMathF::Vector2& deadRate_)
	{
		return controller->TriggerStick(stickInput_, deadRange_, deadRate_);
	}

	bool Input::InputStick(ControllerStick stickInput_, float deadRange_, const AliceMathF::Vector2& deadRate_)
	{
		return controller->InputStick(stickInput_, deadRange_, deadRate_);
	}

	bool Input::TriggerReleaseStick(ControllerStick stickInput_, float deadRange_, const AliceMathF::Vector2& deadRate_)
	{
		return controller->TriggerReleaseStick(stickInput_, deadRange_, deadRate_);
	}

	AliceMathF::Vector2 Input::GetLeftStickVec(const AliceMathF::Vector2& deadRate_)
	{
		return controller->GetLeftStickVec(deadRate_);
	}

	AliceMathF::Vector2 Input::GetRightStickVec(const AliceMathF::Vector2& deadRate_)
	{
		return controller->GetRightStickVec(deadRate_);
	}

	void Input::ShakeController(float power, int32_t span)
	{
		controller->ShakeController(power, span);
	}

	std::unique_ptr<IInput> CreateUniqueInput(void* hwnd_, void* hInstance_)
	{
		std::unique_ptr<IInput> lInput = std::make_unique<Input>();
		lInput->Initialize(hwnd_, hInstance_);
		return std::move(lInput);
	}

	std::shared_ptr<IInput> CreateSharedInput(void* hwnd_, void* hInstance_)
	{
		std::shared_ptr<IInput> lInput = std::make_shared<Input>();
		lInput->Initialize(hwnd_, hInstance_);
		return lInput;
	}
}