#include "GameMain.h"
#include"SceneFactrory.h"

void GameMain::Initialize()
{
	AliceFramework::Initialize();

	//ƒV[ƒ“‚Ì‰Šú‰»
	sceneFactory = SceneFactrory::GetInstance();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	SceneManager::GetInstance()->ChangeScene("GAME");
}

void GameMain::Finalize()
{
	AliceFramework::Finalize();
}

void GameMain::Update()
{
	AliceFramework::Update();
}

void GameMain::Draw()
{
	AliceFramework::Draw();
}

GameMain::GameMain()
{
}

GameMain::~GameMain()
{
}
