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
	camera->SetEye({ 0,10,-10 });
	camera->SetTarget({ 0,0,0 });

	sceneData.reset(SceneLoader::LoadFile("Resources/test.json"));
}

void GameScene::Update()
{
	light->Update();
	camera->Update();

	sceneData->Update(camera.get());
}

void GameScene::Draw()
{
	sceneData->Draw();
}

void GameScene::Finalize()
{
}
