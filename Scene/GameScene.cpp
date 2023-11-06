#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

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
	sGPUParticleEmitter->FireParticleCreate(10000000,"BossHandParticle");
	sGPUParticleEmitter->FireParticleSetTex("BossHandParticle",TextureManager::SLoad("Resources/Default/Particle/FireEffectNoise.png"));

	fieldObjData = SceneLoader::SLoadFile(sPhysicsSystem,"Resources/Field.json");

	player = std::make_unique<Player>();
	player->Initialize(sInput,sAudioManager,sPhysicsSystem);

	gameCameraManager = std::make_unique<GameCameraManager>();
	gameCameraManager->Initialize(player.get(), sInput);

	boss = std::make_unique<Boss>();
	boss->SetPlayer(player.get());
	boss->SetAudioManager(sAudioManager);
	boss->SetFireGPUParticle(sGPUParticleEmitter->GetFireParticle("BossHandParticle"));
	boss->Initialize(sPhysicsSystem);

	inTransition = std::make_unique<FadeInTransition>();
	inTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	inTransition->Start();
	inTransition->SetIncrement(0.04f);

	outTransition = std::make_unique<FadeOutTransition>();
	outTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));

	bgmHandle = sAudioManager->LoadAudio("Resources/BGM/BossBGM.mp3");
	
	sAudioManager->PlayWave(bgmHandle,true);
	sAudioManager->ChangeVolume(bgmHandle,0);

	camera = std::make_unique<GameCamera>();
	camera->Initialize(UPDATE_PROJMATRIXFUNC_PERSPECTIVE);
}

void GameScene::Update()
{
	if ( inTransition->IsStart() && !inTransition->IsEnd())
	{
		float lVolume = 1.0f *( inTransition->GetCoefficient() * 1.05f );
		lVolume = bgmVolume * AliceMathF::Clamp01(lVolume);
		sAudioManager->ChangeVolume(bgmHandle,lVolume);
	}

	if ( outTransition->IsStart() )
	{
		float lVolume = 1.0f - ( outTransition->GetCoefficient() * 1.05f );
		lVolume = bgmVolume * AliceMathF::Clamp01(lVolume);
		sAudioManager->ChangeVolume(bgmHandle,lVolume);
	}

	if ( boss->IsEnd() )
	{
		boss->DeathSEChangeVolume(AliceMathF::Clamp01(1.0f - ( outTransition->GetCoefficient() * 1.05f )));

		if ( !outTransition->IsStart() )
		{
			outTransition->Start();
			outTransition->SetIncrement(0.02f);
		}

		if ( outTransition->IsEnd() )
		{
			sceneManager->ChangeScene("CLEAR");
			sAudioManager->StopWave(seHandle);
			sAudioManager->StopWave(bgmHandle);
		}
	}

	if (player->IsEnd())
	{
		player->DeathSEChangeVolume(AliceMathF::Clamp01(1.0f - ( outTransition->GetCoefficient() * 1.05f )));

		if (!outTransition->IsStart())
		{
			outTransition->Start();
			outTransition->SetIncrement(0.02f);
		}

		if (outTransition->IsEnd())
		{
			sceneManager->ChangeScene("FAILURE");
			sAudioManager->StopWave(seHandle);
			sAudioManager->StopWave(bgmHandle);
		}
	}

	gameCameraManager->BeginUpdate();

	player->Update(gameCameraManager->GetGameCamera(), gameCameraManager->GetCameraIndex());

	boss->Update();

	gameCameraManager->Update();

	player->TransUpdate(gameCameraManager->GetCamera());
	boss->TransUpdate(gameCameraManager->GetCamera());

	fieldObjData->Update(sPhysicsSystem,gameCameraManager->GetCamera());

	inTransition->Update();
	outTransition->Update();
}

void GameScene::Draw()
{
	fieldObjData->Draw();

	player->Draw();
	boss->Draw();

	player->UIDraw();
	boss->UIDraw();

	inTransition->Draw();
	outTransition->Draw(true);
}

void GameScene::Finalize()
{
	player->Finalize(sPhysicsSystem);
	boss->Finalize(sPhysicsSystem);
	fieldObjData->Finalize(sPhysicsSystem);
}

Camera* GameScene::GetSceneCamera()
{
	return gameCameraManager->GetCamera();
}
