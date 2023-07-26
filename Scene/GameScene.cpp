#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

#include<Collision.h>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	fieldObjData = SceneLoader::SLoadFile("Resources/Field.json");

	player = std::make_unique<Player>();
	player->Initialize(sInput);

	gameCameraManager = std::make_unique<GameCameraManager>();
	gameCameraManager->Initialize(player.get(),sInput);

	boss = std::make_unique<Boss>();
	boss->Initialize();
}

void GameScene::Update()
{
	gameCameraManager->BeginUpdate();

	player->Update(gameCameraManager->GetGameCamera(), gameCameraManager->GetCameraIndex());
	boss->Update();

	gameCameraManager->Update();

	player->TransUpdate(gameCameraManager->GetCamera());
	boss->TransUpdate(gameCameraManager->GetCamera());

	fieldObjData->Update(gameCameraManager->GetCamera());


}

void GameScene::Draw()
{
	fieldObjData->Draw();

	player->Draw();
	boss->Draw();

	//player->UIDraw();
}

void GameScene::Finalize()
{
}

