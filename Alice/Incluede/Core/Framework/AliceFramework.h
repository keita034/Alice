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

class AliceFramework
{
protected:

	WindowsApp* windowsApp = nullptr;

	DirectX12Core* directX12Core = nullptr;

	Mesh* mesh = nullptr;

	Mesh3D* mesh3D = nullptr;

	TextureManager* textureManager = nullptr;

	AudioManager* audioManager = nullptr;

	AliceInput::IInput* input = nullptr;

	FPS* fps = nullptr;

	std::unique_ptr<ImGuiManager> imGuiManager;

	SceneManager* sceneManager = nullptr;

	//シーンファクトリー
	AbstractSceneFactory* sceneFactory = nullptr;

	PostEffectManager* postEffectManager = nullptr;

	//終了フラグ

public:

	virtual ~AliceFramework() = default;
	AliceFramework() = default;

	static void DebugInitialize();

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
