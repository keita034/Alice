#include "TitleScene.h"

void TitleScene::Initialize()
{
	input = Input::GetInstance();

	camera_ = std::make_unique<CinemaCamera>();
	camera_->Initialize();
}

void TitleScene::Update()
{
	
}

void TitleScene::Draw()
{
	
}

void TitleScene::Finalize()
{
}
