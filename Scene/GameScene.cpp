#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

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
	camera->SetEye({ 0,10,-50 });
	camera->SetTarget({ 0,10,0 });

	posteffectModelHandle = AliceModel::SCreateModel("Resources/Default/cube");
	posteffectModel = std::make_unique<AliceModel>();
	posteffectModel->SetModel(posteffectModelHandle);
	posteffectModelTransform.Initialize();
	posteffectModelTransform.scale = { 0.01f,0.01f ,0.01f };
	posteffectModelTransform.translation = { 0,10 ,-45 };
}

void GameScene::Update()
{
	light->Update();
	camera->Update();

	ImGui::Begin("Demo");

	PostEffectGui();

	ImGui::End();

	light->SetLightDir({ 1.0f,-1.0f,1.0f });
	posteffectModelTransform.TransUpdate(camera.get());

}

void GameScene::Draw()
{
	posteffectModel->Draw(posteffectModelTransform);
}

void GameScene::Finalize()
{
}

void GameScene::PostEffectGui()
{

	if (ImGui::Checkbox("Gaussianblur", &gaussianBlurPosteffect[0]))
	{
		if (!gaussianBlurPosteffect[1])
		{
			gaussianBlurPosteffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("GAUSSIANBLUR");
		}
		else
		{
			gaussianBlurPosteffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("GAUSSIANBLUR");
		}
	}

	if (ImGui::Checkbox("Bloom", &bloomPostEffect[0]))
	{
		if (!bloomPostEffect[1])
		{
			bloomPostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("BLOOM");
		}
		else
		{
			bloomPostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("BLOOM");
		}
	}
}
