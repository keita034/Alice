#include "GameMain.h"
#include"SceneFactrory.h"


void GameMain::Initialize()
{
	AliceFramework::Initialize();

	//シーンの初期化
	sceneFactory = SceneFactrory::SGetInstance();
	SceneManager::SGetInstance()->SetSceneFactory(sceneFactory);

	SceneManager::SGetInstance()->ChangeScene("GAME");
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
