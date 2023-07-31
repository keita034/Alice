#include "TitleScene.h"

#include<SceneManager.h>

void TitleScene::Initialize()
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

	//camera_ = std::make_unique<GameCamera>();
	//camera_->Initialize();

	transition = std::make_unique<FadeOutTransition>();
	transition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	transition->SetIncrement(0.02f);
}

void TitleScene::Update()
{
	if (sInput->TriggerButton(ControllerButton::A)|| sInput->TriggerKey(Keys::A))
	{
		if (!sceneChange)
		{
			sceneChange = true;
			transition->Start();
		}
	}

	transition->Update();

	if (transition->IsEnd())
	{
		sceneManager->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	title->Draw(transform);
	startA->Draw(startATransform);

	transition->Draw();

}

void TitleScene::Finalize()
{
}
