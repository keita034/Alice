#include "ClearScene.h"

#include<FadeOutTransition.h>
#include<SceneManager.h>

void ClearScene::Initialize(const std::string& previewSceneName_)
{
	clearHanlde = TextureManager::SLoad("Resources/UI/Clear.png");
	clear = CreateUniqueSprite2D(clearHanlde);
	transform.Initialize();
	transform.translation = { sWinApp->GetWindowSize().width / 2,sWinApp->GetWindowSize().height / 2 - 150,0u };


	inTransition = std::make_unique<FadeInTransition>();
	inTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	inTransition->Start();
	inTransition->SetIncrement(0.008f);

	outTransition = std::make_unique<FadeOutTransition>();
	outTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	outTransition->SetIncrement(0.008f);


	bgmHandle = sAudioManager->LoadAudio("Resources/BGM/ResultBGB.mp3");
	sAudioManager->PlayWave(bgmHandle,true);
	sAudioManager->ChangeVolume(bgmHandle,0);

	seHandle = sAudioManager->LoadAudio("Resources/SE/UISelect.mp3");

	gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize(UpdateProjMatrixFunc::UPDATE_PROJMATRIXFUNC_PERSPECTIVE);
}

void ClearScene::Update()
{

	if ( inTransition->IsStart() && !inTransition->IsEnd() )
	{
		float lVolume = 1.0f * ( inTransition->GetCoefficient() * 1.05f );
		lVolume = bgmVolume * AliceMathF::Clamp01(lVolume);
		sAudioManager->ChangeVolume(bgmHandle,lVolume);
	}

	if ( outTransition->IsStart() )
	{
		float lVolume = 1.0f - ( outTransition->GetCoefficient() * 1.05f );
		lVolume = bgmVolume * AliceMathF::Clamp01(lVolume);
		sAudioManager->ChangeVolume(bgmHandle,lVolume);
	}

	if (sInput->TriggerButton(ControllerButton::A) || sInput->TriggerKey(Keys::A))
	{
		if (!sceneChange && inTransition->IsEnd())
		{
			sceneChange = true;
			outTransition->Start();
			sAudioManager->PlayWave(seHandle);
			sAudioManager->ChangeVolume(seHandle,seVolume);
		}
	}

	inTransition->Update();
	outTransition->Update();

	if (outTransition->IsEnd())
	{
		sceneManager->ChangeScene("TITLE");
	}
}

void ClearScene::Draw()
{
	clear->Draw(transform);

	inTransition->Draw();
	outTransition->Draw(true);

}

void ClearScene::Finalize()
{
}

Camera* ClearScene::GetSceneCamera()
{
	return gameCamera.get();
}