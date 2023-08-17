#include "FailureScene.h"

#include<FadeOutTransition.h>
#include<SceneManager.h>

void FailureScene::Initialize(const std::string& previewSceneName_)
{
	failureHanlde = TextureManager::SLoad("Resources/UI/Failure.png");
	failure = CreateUniqueSprite2D(failureHanlde);
	transform.Initialize();
	transform.translation = { sWinApp->GetWindowSize().width / 2,sWinApp->GetWindowSize().height / 2 - 150,0u };


	inTransition = std::make_unique<FadeInTransition>();
	inTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	inTransition->Start();
	inTransition->SetIncrement(0.008f);

	outTransition = std::make_unique<FadeOutTransition>();
	outTransition->Initilize(static_cast<float>(sWinApp->GetWindowSize().height), static_cast<float>(sWinApp->GetWindowSize().width));
	outTransition->SetIncrement(0.008f);

}

void FailureScene::Update()
{

	if (sInput->TriggerButton(ControllerButton::A) || sInput->TriggerKey(Keys::A))
	{
		if (!sceneChange && inTransition->IsEnd())
		{
			sceneChange = true;
			outTransition->Start();
		}
	}

	inTransition->Update();
	outTransition->Update();

	if (outTransition->IsEnd())
	{
		sceneManager->ChangeScene("TITLE");
	}
}

void FailureScene::Draw()
{
	failure->Draw(transform);

	inTransition->Draw();
	outTransition->Draw(true);

}

void FailureScene::Finalize()
{
}
