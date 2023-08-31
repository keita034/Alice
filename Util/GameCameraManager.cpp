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
	if (cameraIndex == PLAYER_CAMERA)
	{
		PPlayerCameraUpdate();
	}

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

void GameCameraManager::PPlayerCameraUpdate()
{
	//std::unique_ptr<GameCamera>& lCamera = cameras[CameraIndex::PLAYER_CAMERA];

	//AliceMathF::Vector3 lCameraToPos = lCamera->GetEye() - lCamera->GetTarget();

	//float lHeight = lCameraToPos.y;
	//lCameraToPos.y = 0.0f;

	//float lCameraToPosLen = lCameraToPos.Length();
	//lCameraToPos = lCameraToPos.Normal();

	//AliceMathF::Vector3 lTarget = player->GetPosition();
	//lTarget.y += 50.0f;

	//AliceMathF::Vector3 lNewCameraPos = lCamera->GetEye() - lTarget;
	//lNewCameraPos.y = 0.0f; 
	//lNewCameraPos = lNewCameraPos.Normal();  

	//float lWeight = 1.0f;
	//lNewCameraPos = lNewCameraPos * lWeight + lCameraToPos * (1.0f - lWeight);
	//lNewCameraPos.Normal();
	//lNewCameraPos *= lCameraToPosLen;
	//lNewCameraPos.y = lHeight;

	//AliceMathF::Vector3 lPos = lTarget + lNewCameraPos;

	//lCamera->SetEye(lPos);
	//lCamera->SetTarget(lTarget);
}
