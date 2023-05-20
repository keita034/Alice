#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{

	mesh2D = Mesh::GetInstance();
	audioManager = AudioManager::GetInstance();

	//ライト初期化
	light.reset(Light::Create());
	light->SetLightColor(lightColor);
	light->SetLightDir({ 0.5f,0.0f,0.5f });
	//モデルにライトをセット
	AliceModel::SetLight(light.get());

	camera = std::make_unique<GameCamera>();
	camera->Initialize(UpdateProjMatrixFunc_Perspective);
	camera->SetEye({ 0,0,-100 });
	camera->SetTarget({ 0,0,0 });
	//camera->SetTarget({ 0.0f,70.0f,0.0f});

	//playerModelHandle = AliceModel::CreateModel("Resources/Default/Test");
	playerModelHandle2 = AliceModel::CreateToonModel("Resources/Default/Test");
	//playerModel = std::make_unique<AliceModel>();
//	playerModel->SetModel(playerModelHandle);
	playerModel2 = std::make_unique<AliceToonModel>();
	playerModel2->SetModel(playerModelHandle2);
	playerTransform.translation = { 0.0f,0.0f,0.0f };
	playerTransform.scale = { 0.1f,0.1f,0.1f };
	playerTransform.Initialize();

	//PostEffectManager::GetInstance()->AddPostEffect("CHROMATICABERRATION");
}

void GameScene::Update()
{
	light->Update();
	camera->Update();

	if (input->CheckKey(Keys::LEFT))
	{
		angle++;
		playerTransform.rotation = { 0.0f,angle * AliceMathF::Deg2Rad,0.0f };

	}

	if (input->CheckKey(Keys::RIGHT))
	{
		angle--;
		playerTransform.rotation = { 0.0f,angle * AliceMathF::Deg2Rad,0.0f };

	}
	playerTransform.TransUpdate(camera.get());
}

void GameScene::Draw()
{
	playerModel2->Draw(playerTransform);
}

void GameScene::Finalize()
{
}
