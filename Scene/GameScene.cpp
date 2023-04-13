#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	input = Input::GetInstance();
	mesh2D = Mesh::GetInstance();
	audioManager = AudioManager::GetInstance();

	//ライト初期化
	light.reset(Light::Create());
	light->SetLightColor(lightColor);
	light->SetLightDir({ 0,-1,0 });
	//モデルにライトをセット
	AliceModel::SetLight(light.get());

	camera = std::make_unique<GameCamera>();
	camera->Initialize(UpdateProjMatrixFunc_Perspective);
	camera->SetEye({ 0,70,-180.0f });
	camera->SetTarget({ 0.0f,70.0f,0.0f});

	playerModelHandle = AliceModel::CreateModel("Resources/Default/cube");
	playerModel = std::make_unique<AliceModel>();
	playerModel->SetModel(playerModelHandle);
	playerTransform.translation = { 0.0f,0.0f,0.0f };
	playerTransform.Initialize();
}

void GameScene::Update()
{
	camera->Update();
	playerTransform.TransUpdate(camera.get());
}

void GameScene::Draw()
{
	playerModel->Draw(playerTransform);
}

void GameScene::Finalize()
{
}
