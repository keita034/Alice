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
}

void ClearScene::Update()
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

void ClearScene::Draw()
{
	clear->Draw(transform);

	inTransition->Draw();
	outTransition->Draw(true);

}

void ClearScene::Finalize()
{
}
