#include "SceneFactrory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"

SceneFactrory* SceneFactrory::GetInstance()
{
	static SceneFactrory instance;
	return &instance;
}

BaseScene* SceneFactrory::CreateScene(const std::string& sceneName)
{
	//éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if(sceneName == "GAME")
	{
		newScene = new GameScene();
	}
	else if (sceneName == "RESULT")
	{
		newScene = new ResultScene();
	}
	return newScene;
}
