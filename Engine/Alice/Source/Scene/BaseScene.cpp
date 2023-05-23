#include<BaseScene.h>

AliceInput::IInput* BaseScene::input = nullptr;

void BaseScene::SetSceneManager(SceneManager* manager)
{
	sceneManager = manager;
}

void  BaseScene::SetInput(AliceInput::IInput* iInput)
{
	input = iInput;
}