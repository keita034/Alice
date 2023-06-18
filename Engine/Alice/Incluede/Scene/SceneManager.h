#pragma once
#include<BaseScene.h>
#include<AbstractSceneFactory.h>

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
private:

	//今のシーン(実行中のシーン)
	std::unique_ptr<BaseScene> scene;

	//次のシーン
	std::unique_ptr<BaseScene> nextScene;

	AbstractSceneFactory* sceneFactory = nullptr;

public:

	static SceneManager* SGetInstance();

	//次のシーン予約
	void ChangeScene(const std::string& sceneName_);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetSceneFactory(AbstractSceneFactory* factory_);

private:

	SceneManager();
	~SceneManager();

	//コピーコンストラクタ・代入演算子削除
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
};

