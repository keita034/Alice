#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{

	sceneData = SceneLoader::SLoadFile("Resources/test.json");
}

void GameScene::Update()
{
	sceneData->Update();
}

void GameScene::Draw()
{
	sceneData->Draw();
}

void GameScene::Finalize()
{
}
