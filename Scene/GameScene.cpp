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

	skinningModelHandle = AliceModel::SCreateModel("Resources/boneTest");
	skinningModel = std::make_unique<AliceModel>();
	skinningModel->SetModel(skinningModelHandle);
	skinningModelTransform.Initialize();
	skinningModelTransform.scale = { 0.025f,0.025f,0.025f };

	modelAnimationHandle = AliceMotionData::SCreateMotion("Resources/boneTest");
	modelAnimation = std::make_unique<AliceMotionData>();
	modelAnimation->SetMotion(modelAnimationHandle);

	sphereModelHandle = AliceModel::SCreateModel("Resources/Default/sphere");
	sphereModel = std::make_unique<AliceModel>();
	sphereModel->SetModel(sphereModelHandle);
	sphereToonModelHandle = AliceModel::SCreateToonModel("Resources/Default/toonSphere");
	sphereToonModel = std::make_unique<AliceToonModel>();
	sphereToonModel->SetModel(sphereToonModelHandle);
	sphereModelTransform.Initialize();
	sphereModelTransform.translation = { 0,10 ,-45 };

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

	ImGui::Combo("DemoSelect", &demoIndex, demoList.data(), static_cast<int32_t>(demoList.size()));

	if (static_cast<DemoIndex>(demoIndex) == GameScene::DemoIndex::POSTEFFECT)
	{
		PostEffectGui();
	}

	ImGui::End();

	switch (static_cast<DemoIndex>(demoIndex))
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
	case GameScene::DemoIndex::TOON_SHADER:
		light->SetLightDir({ 1.0f,-1.0f,1.0f });
		sphereModelTransform.TransUpdate(camera.get());
		break;
	case GameScene::DemoIndex::POSTEFFECT:
		light->SetLightDir({ 1.0f,-1.0f,1.0f });
		posteffectModelTransform.TransUpdate(camera.get());
		break;
	default:
		break;
	}

}

void GameScene::Draw()
{
	switch (static_cast<DemoIndex>(demoIndex))
	{
	case GameScene::DemoIndex::SKINNING_ANIMATION:
		skinningModel->Draw(skinningModelTransform, modelAnimation.get(), modelAnimationFrame);
		break;
	case GameScene::DemoIndex::PHONG_SHADER:
		sphereModel->Draw(sphereModelTransform, nullptr, 0.0f, MaterialManager::SGetInstance()->GetMaterialData("DefaultPhong"));
		break;
	case GameScene::DemoIndex::TOON_SHADER:
		sphereToonModel->Draw(sphereModelTransform);
		break;
	case GameScene::DemoIndex::POSTEFFECT:
		posteffectModel->Draw(posteffectModelTransform);
		break;
	default:
		break;
	}
}

void GameScene::Finalize()
{
}

void GameScene::PostEffectGui()
{
	if (ImGui::Checkbox("Task", &stripedPosteffect[0]))
	{
		if (!stripedPosteffect[1])
		{
			stripedPosteffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("STRIPED");
		}
		else
		{
			stripedPosteffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("STRIPED");
		}
	}

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

	if (ImGui::Checkbox("Oilpainting", &oilPaintingPostEffect[0]))
	{
		if (!oilPaintingPostEffect[1])
		{
			oilPaintingPostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("OILPAINTING");
		}
		else
		{
			oilPaintingPostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("OILPAINTING");
		}
	}

	if (ImGui::Checkbox("Chromaticaberration", &chromaticAberrationPostEffect[0]))
	{
		if (!chromaticAberrationPostEffect[1])
		{
			chromaticAberrationPostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("CHROMATICABERRATION");
		}
		else
		{
			chromaticAberrationPostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("CHROMATICABERRATION");
		}
	}

	if (ImGui::Checkbox("Vignette", &vignettePostEffect[0]))
	{
		if (!vignettePostEffect[1])
		{
			vignettePostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("VIGNETTE");
		}
		else
		{
			vignettePostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("VIGNETTE");
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

	if (ImGui::Checkbox("SandStorm", &sandStormPostEffect[0]))
	{
		if (!sandStormPostEffect[1])
		{
			sandStormPostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("SANDSTORM");
		}
		else
		{
			sandStormPostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("SANDSTORM");
		}
	}

	if (ImGui::Checkbox("CRT", &crtPostEffect[0]))
	{
		if (!crtPostEffect[1])
		{
			crtPostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("CRT");
		}
		else
		{
			crtPostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("CRT");
		}
	}

	if (ImGui::Checkbox("Monochrome", &monochromePostEffect[0]))
	{
		if (!monochromePostEffect[1])
		{
			monochromePostEffect[1] = true;
			PostEffectManager::SGetInstance()->AddPostEffect("MONOCHROME");
		}
		else
		{
			monochromePostEffect[1] = false;
			PostEffectManager::SGetInstance()->SubPostEffect("MONOCHROME");
		}
	}
}
