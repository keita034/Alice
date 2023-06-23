#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	//ライト初期化
	light = CreateUniqueLight();
	light->SetLightColor(lightColor);
	light->SetLightDir({ 0.0f,0.0f,1.0f });
	//モデルにライトをセット
	AliceModel::SSetLight(light.get());

	camera = std::make_unique<GameCamera>();
	camera->Initialize(UPDATE_PROJMATRIXFUNC_PERSPECTIVE);
	camera->SetEye({0,10,-50});
	camera->SetTarget({0,10,0});

	skinningModelHandle = AliceModel::SCreateModel("Resources/boneTest");
	skinningModel = std::make_unique<AliceModel>();
	skinningModel->SetModel(skinningModelHandle);
	skinningModelTransform.Initialize();
	skinningModelTransform.scale = {0.025f,0.025f,0.025f};

	modelAnimationHandle = AliceMotionData::SCreateMotion("Resources/boneTest");
	modelAnimation = std::make_unique<AliceMotionData>();
	modelAnimation->SetMotion(modelAnimationHandle);

	sphereModelHandle = AliceModel::SCreateModel("Resources/Default/sphere");
	sphereModel = std::make_unique<AliceModel>();
	sphereModel->SetModel(sphereModelHandle);
	sphereModelTransform.Initialize();
	sphereModelTransform.translation = {0,10 ,-45};

	PostEffectManager::SGetInstance()->AddPostEffect("STRIPED");

	//PostEffectManager::SGetInstance()->AddPostEffect("OILPAINTING");
}

void GameScene::Update()
{
	light->Update();
	camera->Update();

	switch (demoIndex)
	{
	case GameScene::DemoIndex::SKINNING_ANIMATION:
		light->SetLightDir({ 0.0f,0.0f,1.0f });
		skinningModelTransform.TransUpdate(camera.get());
		modelAnimationFrame += 0.01f;
		break;
	case GameScene::DemoIndex::PHONG_SHADER:
		light->SetLightDir({ 1.0f,-1.0f,1.0f });
		sphereModelTransform.TransUpdate(camera.get());
		break;
	case GameScene::DemoIndex::POSTEFFECT:


		light->SetLightDir({ 1.0f,-1.0f,1.0f });
		sphereModelTransform.TransUpdate(camera.get());
		break;
	default:
		break;
	}

}

void GameScene::Draw()
{
	switch (demoIndex)
	{
	case GameScene::DemoIndex::SKINNING_ANIMATION:
		skinningModel->Draw(skinningModelTransform, modelAnimation.get(), modelAnimationFrame);
		break;
	case GameScene::DemoIndex::PHONG_SHADER:
		sphereModel->Draw(sphereModelTransform,nullptr,0.0f,MaterialManager::SGetInstance()->GetMaterialData("DefaultPhong"));
		break;
	case GameScene::DemoIndex::POSTEFFECT:
		sphereModel->Draw(sphereModelTransform);
		break;
	default:
		break;
	}
}

void GameScene::Finalize()
{
}
