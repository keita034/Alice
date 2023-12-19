#include "GameMain.h"
#include"SceneFactrory.h"


void GameMain::Initialize()
{
	AliceFramework::Initialize();
	directX12Core->SetBackScreenColor(11 / 255.f, 15 / 255.f, 20 / 255.f, 1.0f);

	int32_t emmisionFireElemental = TextureManager::SLoad("Resources/Model/Boss/emmision fire_elemental.png",AdaptersIndex::SUB);
	int32_t fireEffectNoiseHandle = TextureManager::SLoad("Resources/Default/Particle/FireEffectNoise.png");
	int32_t effect1Handle = TextureManager::SLoad("Resources/Default/Particle/effect1.png");
	int32_t effect2Handle = TextureManager::SLoad("Resources/Default/Particle/effect2.png");
	int32_t fireEffectNoiseInvHandle = TextureManager::SLoad("Resources/Default/Particle/FireEffectNoiseInv.png");
	int32_t white1x1Handle = TextureManager::SLoad("Resources/Default/white1x1.png");

	gpuParticleEmitter->FireParticleCreate(10000,"BossHandParticle");
	gpuParticleEmitter->FireParticleSetTex("BossHandParticle",fireEffectNoiseHandle);

	gpuParticleEmitter->ShockWaveParticleCreate(10000,"BossShockWaveParticle");
	gpuParticleEmitter->ShockWaveParticleSetTex("BossShockWaveParticle",effect1Handle);

	gpuParticleEmitter->LaserParticleCreate(10000,"BossLaserParticle");
	gpuParticleEmitter->LaserParticleSetMainTex("BossLaserParticle",effect1Handle);
	gpuParticleEmitter->LaserParticleSetSubTex("BossLaserParticle",effect2Handle);

	gpuParticleEmitter->AnimationMeshGPUParticleCreate(1000000,"BossModelParticle");
	std::unique_ptr<AliceModel>lModel = std::make_unique<AliceModel>();
	lModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss"));
	gpuParticleEmitter->AnimationMeshGPUParticleSetModel("BossModelParticle",lModel.get());
	gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex("BossModelParticle",emmisionFireElemental);
	gpuParticleEmitter->AnimationMeshGPUParticleSetTex("BossModelParticle",fireEffectNoiseHandle);

	gpuParticleEmitter->AnimationMeshGPUParticleCreate(1000000,"BossWeaponParticle");
	std::unique_ptr<AliceModel>lWeaponModel = std::make_unique<AliceModel>();
	lModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss/Weapon/Sword"));
	gpuParticleEmitter->AnimationMeshGPUParticleSetModel("BossWeaponParticle",lModel.get());
	gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex("BossWeaponParticle",white1x1Handle);
	gpuParticleEmitter->AnimationMeshGPUParticleSetTex("BossWeaponParticle",fireEffectNoiseHandle);


	gpuParticleEmitter->BloodGushGPUParticleCreate(100000,"BossBloodGushParticle");
	gpuParticleEmitter->BloodGushGPUParticleSetTex("BossBloodGushParticle",fireEffectNoiseHandle);

	//シーンの初期化
	sceneFactory = SceneFactrory::SGetInstance();
	SceneManager::SGetInstance()->SetSceneFactory(sceneFactory);

	SceneManager::SGetInstance()->ChangeScene("TITLE");
}

void GameMain::Finalize()
{
	AliceFramework::Finalize();
}

void GameMain::Update()
{
	AliceFramework::Update();
}

void GameMain::Draw()
{
	AliceFramework::Draw();
}

GameMain::GameMain()
{
}

GameMain::~GameMain()
{
}
