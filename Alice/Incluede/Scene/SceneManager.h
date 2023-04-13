#pragma once
#include"BaseScene.h"
#include"AbstractSceneFactory.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
private:

	//今のシーン(実行中のシーン)
	BaseScene* scene = nullptr;

	//次のシーン
	BaseScene* nextScene = nullptr;

	AbstractSceneFactory* sceneFactory = nullptr;

public:

	static SceneManager* GetInstance();

	//次のシーン予約
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetSceneFactory(AbstractSceneFactory* factory);

private:

	SceneManager() = default;
	~SceneManager();
};

