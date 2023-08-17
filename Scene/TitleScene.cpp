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

	//camera_ = std::make_unique<GameCamera>();
	//camera_->Initialize();

	inTransition = std::make_unique<FadeInTransition>();
	inTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	inTransition->SetIncrement(0.008f);

	if (previewSceneName_ != "")
	{
		inTransition->Start();
	}


	outTransition = std::make_unique<FadeOutTransition>();
	outTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	outTransition->SetIncrement(0.008f);
}

void TitleScene::Update()
{
	if (sInput->TriggerButton(ControllerButton::A)|| sInput->TriggerKey(Keys::A))
	{
		if (!sceneChange)
		{
			sceneChange = true;
			outTransition->Start();
		}
	}

	outTransition->Update();
	inTransition->Update();

	if (outTransition->IsEnd())
	{
		sceneManager->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	title->Draw(transform);
	startA->Draw(startATransform);

	inTransition->Draw();
	outTransition->Draw(true);
}

void TitleScene::Finalize()
{
}
