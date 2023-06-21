#include "SceneFactrory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

SceneFactrory* SceneFactrory::SGetInstance()
{
	static SceneFactrory lInstance;
	return &lInstance;
}

std::unique_ptr<BaseScene> SceneFactrory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	std::unique_ptr<BaseScene> lNewScene = nullptr;

	if (sceneName == "TITLE")
	{
		lNewScene = std::make_unique<TitleScene>();
	}
	else if (sceneName == "GAME")
	{
		lNewScene = std::make_unique<GameScene>();
	}
	else if (sceneName == "RESULT")
	{
		lNewScene = std::make_unique<ResultScene>();
	}
	return std::move(lNewScene);
}
