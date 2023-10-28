#pragma once
#include<Input.h>
#include<AudioManager.h>
#include<WindowsApp.h>
#include<AlicePhysicsSystem.h>
/// <summary>
/// シーン基底クラス
/// </summary>

class SceneManager;

class BaseScene
{
protected:

	static AliceInput::IInput* sInput;
	static IAudioManager* sAudioManager;
	static IWindowsApp* sWinApp;
	static AlicePhysics::AlicePhysicsSystem* sPhysicsSystem;

	//シーンマネージャー
	SceneManager* sceneManager = nullptr;

	std::string sceneName;


public:

	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const std::string& previewSceneName_) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 後始末
	/// </summary>
	virtual void Finalize() = 0;

	virtual const AliceMathF::Matrix4& GetSceneViewMatrix() = 0;

	virtual const AliceMathF::Matrix4& GetSceneProjectionMatrix() = 0;

	virtual void SetSceneManager(SceneManager* manager_);

	void SetSceneName(const std::string& sceneName_);

	const std::string& GetSceneName()const;

	static void SSetInput(AliceInput::IInput* input_);
	static void SSetAudioManager(IAudioManager* audioManager_);
	static void SSetWindowsApp(IWindowsApp* windowsApp_);
	static void SSetPhysicsSystem(AlicePhysics::AlicePhysicsSystem* physicsSystem_);

};

