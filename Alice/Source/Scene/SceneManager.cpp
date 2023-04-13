#include "SceneManager.h"
#include"PostEffectManager.h"
SceneManager::~SceneManager()
{
	scene->Finalize();
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	nextScene = sceneFactory->CreateScene(sceneName);
}

void SceneManager::Update()
{

	//次のシーンの予約があるなら
	if (nextScene)
	{
		//旧シーンの終了
		if (scene)
		{
			scene->Finalize();
			PostEffectManager::GetInstance()->Finalize();
			delete scene;
		}

		//シーン切り替え
		scene = nextScene;
		nextScene = nullptr;

		//シーンマネージャーをセット
		scene->SetSceneManager(this);

		//次のシーンを初期化する
		scene->Initialize();
		PostEffectManager::GetInstance()->PostInitialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* factory)
{
	sceneFactory = factory;
}
