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
	int32_t white1x1Handle = TextureManager::SLoad("Resources/Default/white1x1.png",AdaptersIndex::SUB);

	{
		gpuParticleEmitter->FireParticleCreate(10000,"BossHandParticle");
		gpuParticleEmitter->FireParticleSetTex("BossHandParticle",fireEffectNoiseHandle);
	}

	{
		gpuParticleEmitter->ShockWaveParticleCreate(10000,"BossShockWaveParticle");
		gpuParticleEmitter->ShockWaveParticleSetTex("BossShockWaveParticle",effect1Handle);
	}

	{
		gpuParticleEmitter->LaserParticleCreate(10000,"BossLaserParticle");
		gpuParticleEmitter->LaserParticleSetMainTex("BossLaserParticle",effect1Handle);
		gpuParticleEmitter->LaserParticleSetSubTex("BossLaserParticle",effect2Handle);
	}

	{
		gpuParticleEmitter->AnimationMeshGPUParticleCreate(100000,"BossParticle");
		std::unique_ptr<AliceModel>lModel = std::make_unique<AliceModel>();
		lModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss"));
		gpuParticleEmitter->AnimationMeshGPUParticleSetModel("BossParticle",lModel.get());
		gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex("BossParticle",emmisionFireElemental);
		gpuParticleEmitter->AnimationMeshGPUParticleSetTex("BossParticle",fireEffectNoiseHandle);
	}

	{
		gpuParticleEmitter->AnimationModelGPUParticleCreate(1000000,"BossModelParticle");
		std::unique_ptr<AliceModel>lModel = std::make_unique<AliceModel>();
		lModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss/Particle"));
		gpuParticleEmitter->AnimationModelGPUParticleSetModel("BossModelParticle",lModel.get());
		gpuParticleEmitter->AnimationModelGPUParticleSetTex("BossModelParticle",effect1Handle);

		ModelGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.velocity = { 0,0,0 };
		lSetting.startColor = { 1,0,0,1 };
		lSetting.endColor = { 1,0,0,1 };
		lSetting.lifeTime = 0.05f;
		lSetting.maxParticles = 1000000.0f;
		lSetting.timeBetweenEmit = 0.1f;
		lSetting.emitLifeTime = -1;
		lSetting.size = 0.2f;
		lSetting.speed = 15;
		lSetting.isPlay = false;

		gpuParticleEmitter->AnimationModelGPUParticleEmit("BossModelParticle",lSetting);
	}

	{
		gpuParticleEmitter->MeshGPUParticleCreate(1000000,"BossWeaponParticle");
		std::unique_ptr<AliceModel>lWeaponModel = std::make_unique<AliceModel>();
		lWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss/Weapon/Sword"));
		gpuParticleEmitter->MeshGPUParticleSetModel("BossWeaponParticle",lWeaponModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("BossWeaponParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("BossWeaponParticle",fireEffectNoiseHandle);
	}

	{
		gpuParticleEmitter->MeshGPUParticleCreate(1000000,"PiayerWeaponParticle");
		std::unique_ptr<AliceModel>lPlayerWeaponModel = std::make_unique<AliceModel>();
		lPlayerWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/katana"));
		gpuParticleEmitter->MeshGPUParticleSetModel("PiayerWeaponParticle",lPlayerWeaponModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("PiayerWeaponParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("PiayerWeaponParticle",fireEffectNoiseHandle);
	}

	{
		gpuParticleEmitter->BloodGushGPUParticleCreate(100000,"BossBloodGushParticle");
		gpuParticleEmitter->BloodGushGPUParticleSetTex("BossBloodGushParticle",fireEffectNoiseHandle);
	}

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
