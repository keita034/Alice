﻿#include "BaseScene.h"
#include<BaseScene.h>


AliceInput::IInput* BaseScene::sInput = nullptr;
IAudioManager* BaseScene::sAudioManager = nullptr;
IWindowsApp* BaseScene::sWinApp = nullptr;

void BaseScene::SetSceneManager(SceneManager* manager_)
{
	sceneManager = manager_;
}

void  BaseScene::SSetInput(AliceInput::IInput* input_)
{
	sInput = input_;
}

void BaseScene::SSetAudioManager(IAudioManager* audioManager_)
{
	sAudioManager = audioManager_;
}

void BaseScene::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWinApp = windowsApp_;
}
