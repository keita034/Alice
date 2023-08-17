#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

#include<Collision.h>
#include<SceneManager.h>

#include<FadeOutTransition.h>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize(const std::string& previewSceneName_)
{
	fieldObjData = SceneLoader::SLoadFile("Resources/Field.json");

	player = std::make_unique<Player>();
	player->Initialize(sInput);

	gameCameraManager = std::make_unique<GameCameraManager>();
	gameCameraManager->Initialize(player.get(), sInput);

	boss = std::make_unique<Boss>();
	boss->SetPlayer(player.get());
	boss->SetAudioManager(sAudioManager);
	boss->Initialize();

	inTransition = std::make_unique<FadeInTransition>();
	inTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	inTransition->Start();
	inTransition->SetIncrement(0.04f);

	outTransition = std::make_unique<FadeOutTransition>();
	outTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
}

void GameScene::Update()
{
	gameCameraManager->BeginUpdate();

	if (boss->GetHp() > 0 && player->GetHp() > 0)
	{

		player->Update(gameCameraManager->GetGameCamera(), gameCameraManager->GetCameraIndex());
		boss->Update();
	}
	else
	{
		if (!outTransition->IsStart())
		{
			outTransition->Start();
			outTransition->SetIncrement(0.02f);
		}
	}

	gameCameraManager->Update();

	player->TransUpdate(gameCameraManager->GetCamera());
	boss->TransUpdate(gameCameraManager->GetCamera());

	fieldObjData->Update(gameCameraManager->GetCamera());

	inTransition->Update();
	outTransition->Update();

	if (boss->GetHp() <= 0)
	{
		if (outTransition->IsEnd())
		{
			sceneManager->ChangeScene("CLEAR");
		}

	}

	if (player->GetHp() <= 0)
	{
		if (outTransition->IsEnd())
		{
			sceneManager->ChangeScene("FAILURE");
		}

	}

}

void GameScene::Draw()
{
	fieldObjData->Draw();

	player->Draw();
	boss->Draw();

	player->UIDraw();

	inTransition->Draw();
	outTransition->Draw(true);
}

void GameScene::Finalize()
{
}

