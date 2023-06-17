#include "SceneFactrory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

SceneFactrory* SceneFactrory::GetInstance()
{
	static SceneFactrory instance;
	return &instance;
}

std::unique_ptr<BaseScene> SceneFactrory::CreateScene(const std::string& sceneName)
{
	//éüÇÃÉVÅ[ÉìÇê∂ê¨
	std::unique_ptr<BaseScene> lNewScene = nullptr;

	if (sceneName == "TITLE")
	{
		lNewScene = std::make_unique<TitleScene>();
	}
	else if(sceneName == "GAME")
	{
		lNewScene = std::make_unique<GameScene>();
	}
	else if (sceneName == "RESULT")
	{
		lNewScene = std::make_unique<ResultScene>();
	}
	return std::move(lNewScene);
}
