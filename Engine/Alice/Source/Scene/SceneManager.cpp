#include<SceneManager.h>
#include<PostEffectManager.h>

std::unique_ptr<SceneManager> SceneManager::sSceneManager;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::SGetInstance()
{
	if ( !sSceneManager )
	{
		SceneManager* lInstance = new SceneManager;

		sSceneManager.reset(lInstance);
	}

	return sSceneManager.get();
}

void SceneManager::ChangeScene(const std::string& sceneName_)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	nextScene = sceneFactory->CreateScene(sceneName_);

	if ( scene )
	{
		previewSceneName = scene->GetSceneName();
	}

}

void SceneManager::Update()
{
	//次のシーンの予約があるなら
	if ( nextScene )
	{
		//旧シーンの終了
		if ( scene )
		{
			scene->Finalize();
			PostEffectManager::SGetInstance()->Finalize();
			scene.reset();
		}

		//シーン切り替え
		scene = std::move(nextScene);

		//シーンマネージャーをセット
		scene->SetSceneManager(this);

		//次のシーンを初期化する
		scene->Initialize(previewSceneName);
		PostEffectManager::SGetInstance()->PostInitialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* factory_)
{
	sceneFactory = factory_;
}

void SceneManager::Finalize()
{
	if ( scene )
	{
		scene->Finalize();
		scene.reset();
	}

	sSceneManager.release();
}
