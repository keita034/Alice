#include "TitleScene.h"

#include<SceneManager.h>
#include<FadeInTransition.h>

void TitleScene::Initialize(const std::string& previewSceneName_)
{
	titleHanlde = TextureManager::SLoad("Resources/UI/Title.png");
	title = CreateUniqueSprite2D(titleHanlde);
	transform.Initialize();
	transform.translation = { sWinApp->GetWindowSize().width / 2,sWinApp->GetWindowSize().height / 2 - 150,0u };

	startAHanlde = TextureManager::SLoad("Resources/UI/Start-A.png");
	startA = CreateUniqueSprite2D(startAHanlde);
	startATransform.Initialize();
	startATransform.translation = { sWinApp->GetWindowSize().width / 2,sWinApp->GetWindowSize().height / 2 + 200,0u };
	startATransform.scale = { 0.5f,0.5f,1.0f };

	inTransition = std::make_unique<FadeInTransition>();
	inTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	inTransition->SetIncrement(0.008f);

	bgmHandle = sAudioManager->LoadAudio("Resources/BGM/TitelBGM.mp3");
	seHandle = sAudioManager->LoadAudio("Resources/SE/UISelect.mp3");

	if (previewSceneName_ != "")
	{
		inTransition->Start();
		sAudioManager->PlayWave(bgmHandle,true);
		sAudioManager->ChangeVolume(bgmHandle,0);
	}
	else
	{
		sAudioManager->PlayWave(bgmHandle,true);
		sAudioManager->ChangeVolume(bgmHandle,bgmVolume);
	}

	outTransition = std::make_unique<FadeOutTransition>();
	outTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	outTransition->SetIncrement(0.008f);

	gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize(UpdateProjMatrixFunc::UPDATE_PROJMATRIXFUNC_PERSPECTIVE);

}

void TitleScene::Update()
{
	if (sInput->TriggerButton(ControllerButton::A)|| sInput->TriggerKey(Keys::A))
	{
		if (!sceneChange)
		{
			sceneChange = true;
			outTransition->Start();
			sAudioManager->PlayWave(seHandle);
			sAudioManager->ChangeVolume(seHandle,seVolume);
		}
	}

	outTransition->Update();
	inTransition->Update();

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

	if (outTransition->IsEnd())
	{
		sceneManager->ChangeScene("GAME");
		sAudioManager->StopWave(seHandle);
		sAudioManager->StopWave(bgmHandle);
	}
}

void TitleScene::Draw()
{
}

void TitleScene::SpriteDraw()
{
	title->Draw(transform);
	startA->Draw(startATransform);

	inTransition->Draw();
	outTransition->Draw(true);
}

void TitleScene::Finalize()
{
}

Camera* TitleScene::GetSceneCamera()
{
	return gameCamera.get();
}