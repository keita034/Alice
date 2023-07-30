#include "TitleScene.h"

#include<SceneManager.h>

void TitleScene::Initialize()
{
	//titleHanlde = 
	title = std::make_unique<Sprite2D>();
	title->SetTex(titleHanlde);
	transform.Initialize();
	transform.translation = { sWinApp->GetWindowSize().width,sWinApp->GetWindowSize().height,0u };

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
	//title->Draw(transform);

	transition->Draw();
}

void TitleScene::Finalize()
{
}
