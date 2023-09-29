#include "BaseScene.h"
#include "BaseScene.h"
#include<BaseScene.h>


AliceInput::IInput* BaseScene::sInput = nullptr;
IAudioManager* BaseScene::sAudioManager = nullptr;
IWindowsApp* BaseScene::sWinApp = nullptr;
AlicePhysics::AlicePhysicsSystem* BaseScene::sPhysicsSystem = nullptr;

void BaseScene::SetSceneManager(SceneManager* manager_)
{
	sceneManager = manager_;
}

void BaseScene::SetSceneName(const std::string& sceneName_)
{
	sceneName = sceneName_;
}

void  BaseScene::SSetInput(AliceInput::IInput* input_)
{
	sInput = input_;
}

const std::string& BaseScene::GetSceneName()const
{
	return sceneName;
}

void BaseScene::SSetAudioManager(IAudioManager* audioManager_)
{
	sAudioManager = audioManager_;
}

void BaseScene::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWinApp = windowsApp_;
}

void BaseScene::SSetPhysicsSystem(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	sPhysicsSystem = physicsSystem_;
}