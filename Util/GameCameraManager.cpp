#include "GameCameraManager.h"

#include<PlayerGameCamera.h>
#include<Player.h>

GameCameraManager::GameCameraManager() = default;

GameCameraManager::~GameCameraManager() = default;

void GameCameraManager::Initialize(Player* player_,AliceInput::IInput* input_)
{
	{
		std::unique_ptr<PlayerGameCamera>lCamera = std::make_unique<PlayerGameCamera>();
		lCamera->SetPlayer(player_, input_);

		cameras.push_back(std::move(lCamera));
	}

	for (std::unique_ptr<BaseGameCamera>& camera : cameras)
	{
		camera->Initialize();
	}
}

void GameCameraManager::Update()
{
	for (std::unique_ptr<BaseGameCamera>& camera :cameras)
	{
		camera->Update();
	}
}

void GameCameraManager::BeginUpdate()
{
	for (std::unique_ptr<BaseGameCamera>& camera : cameras)
	{
		camera->BeginUpdate();
	}
}

void GameCameraManager::Draw()
{
	for (std::unique_ptr<BaseGameCamera>& camera : cameras)
	{
		camera->Draw();
	}
}

void GameCameraManager::Finalize()
{

}

Camera* GameCameraManager::GetCamera()
{
	return cameras[cameraIndex]->GetGameCamera();
}

BaseGameCamera* GameCameraManager::GetGameCamera()
{
	return cameras[cameraIndex].get();
}

GameCameraManager::CameraIndex GameCameraManager::GetCameraIndex()
{
	return cameraIndex;
}

