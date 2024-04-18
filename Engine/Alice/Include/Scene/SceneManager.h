#pragma once
#include<BaseScene.h>
#include<AbstractSceneFactory.h>

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
private:

	static std::unique_ptr<SceneManager> sSceneManager;

	//今のシーン(実行中のシーン)
	std::unique_ptr<BaseScene> scene;

	//次のシーン
	std::unique_ptr<BaseScene> nextScene;

	AbstractSceneFactory* sceneFactory = nullptr;

	std::string previewSceneName;
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

	void SpriteDraw();

	void SetSceneFactory(AbstractSceneFactory* factory_);

	void Finalize();

	~SceneManager();

	Camera* GetSceneCamera();

private:

	SceneManager();

	//コピーコンストラクタ・代入演算子削除
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
};

