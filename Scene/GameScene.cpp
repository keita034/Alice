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
	light->SetLightDir({ 1.0f,0.0f,1.0f });
	//モデルにライトをセット
	AliceModel::SetLight(light.get());

	camera = std::make_unique<GameCamera>();
	camera->Initialize(UpdateProjMatrixFunc_Perspective);
	//camera->SetEye({ 0,0,-5 });
	camera->SetEye({ 0,90,-800 });
	camera->SetTarget({ 0,90,0 });

	modelHanle = AliceModel::CreateToonModel("Resources/Neru");
	model = std::make_unique<AliceToonModel>();
	model->SetModel(modelHanle);

	trans.Initialize();
}

void GameScene::Update()
{
	if (input->CheckKey(Keys::UP))
	{
		AliceMathF::Vector3 move = { 0.0f,1.0 ,0.0f };
		camera->Move(move);
	}

	if (input->CheckKey(Keys::LEFT))
	{
		AliceMathF::Vector3 move = { -1.0,0.0f,0.0f };
		camera->Move(move);
	}

	if (input->CheckKey(Keys::RIGHT))
	{
		AliceMathF::Vector3 move = { 1.0f,0.0f,0.0f };
		camera->Move(move);
	}
	if (input->CheckKey(Keys::DOWN))
	{
		AliceMathF::Vector3 move = { 0.0f,-1.0, 0.0f };
		camera->Move(move);
	}

	if (input->CheckKey(Keys::W))
	{
		AliceMathF::Vector3 move = { 0.0f,0.0 ,1.0f };
		camera->Move(move);
	}

	if (input->CheckKey(Keys::S))
	{
		AliceMathF::Vector3 move = { 0.0,0.0f,-1.0f };
		camera->Move(move);
	}

	light->Update();
	camera->Update();

	trans.TransUpdate(camera.get());
}

void GameScene::Draw()
{

	model->ZeldaDraw(trans);
}

void GameScene::Finalize()
{
}
