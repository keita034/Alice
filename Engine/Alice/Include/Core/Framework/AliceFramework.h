#pragma once
#include<AudioManager.h>
#include<ImGuiManager.h>
#include<FPS.h>
#include<Mesh.h>
#include<3DMesh.h>
#include<WindowsApp.h>
#include<Input.h>
#include<DirectX12Core.h>
#include<SceneManager.h>
#include<AbstractSceneFactory.h>
#include<PostEffectManager.h>
#include<AlicePhysicsSystem.h>
#include<TextureManager.h>
#include<GPUParticleEmitter.h>

class AliceFramework
{
protected:


	Mesh* mesh = nullptr;
	Mesh3D* mesh3D = nullptr;
	TextureManager* textureManager = nullptr;
	MaterialManager* materialManager = nullptr;
	SceneManager* sceneManager = nullptr;
	AbstractSceneFactory* sceneFactory = nullptr;
	PostEffectManager* postEffectManager = nullptr;

	std::unique_ptr<IAudioManager> audioManager;
	std::unique_ptr<AliceInput::IInput> input;
	std::unique_ptr<IFPS>fps;
	std::unique_ptr<IImGuiManager> imGuiManager;
	std::unique_ptr<IWindowsApp> windowsApp;
	std::unique_ptr<DirectX12Core> directX12Core;
	std::unique_ptr<AlicePhysics::AlicePhysicsSystem>physicsSystem;
	std::unique_ptr<GPUParticleEmitter>gpuParticleEmitter;
public:

	virtual ~AliceFramework();
	AliceFramework() = default;

	static void SDebugInitialize();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void PostUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	virtual bool IsEndRequst();

	/// <summary>
	/// 実行
	/// </summary>
	void Run();

	//コピーコンストラクタ・代入演算子削除
	AliceFramework& operator=(const AliceFramework&) = delete;
	AliceFramework(const AliceFramework&) = delete;
};
