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
		AnimationModelGPUParticleSetting lAnimationSetting;

		lAnimationSetting.matWorld = AliceMathF::MakeIdentity();
		lAnimationSetting.velocity = { 0,0,0 };
		lAnimationSetting.startColor = { 1,0,0,1 };
		lAnimationSetting.endColor = { 1,0,0,1 };
		lAnimationSetting.lifeTime = 0.05f;
		lAnimationSetting.maxParticles = 1000000.0f;
		lAnimationSetting.timeBetweenEmit = 0.1f;
		lAnimationSetting.emitLifeTime = -1;
		lAnimationSetting.size = 0.2f;
		lAnimationSetting.speed = 15;
		lAnimationSetting.isPlay = false;

		ModelGPUParticleSetting lModelSetting;

		lModelSetting.matWorld = AliceMathF::MakeIdentity();
		lModelSetting.velocity = { 0,0,0 };
		lModelSetting.startColor = { 1,0,0,1 };
		lModelSetting.endColor = { 1,0,0,1 };
		lModelSetting.lifeTime = 0.05f;
		lModelSetting.maxParticles = 1000000.0f;
		lModelSetting.timeBetweenEmit = 0.1f;
		lModelSetting.emitLifeTime = -1;
		lModelSetting.size = 0.05f;
		lModelSetting.speed = 15;
		lModelSetting.isPlay = false;

		gpuParticleEmitter->AnimationModelGPUParticleCreate(1000000,"BossModelParticle");
		std::unique_ptr<AliceModel>lBossModel = std::make_unique<AliceModel>();
		lBossModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss/Particle"));
		gpuParticleEmitter->AnimationModelGPUParticleSetModel("BossModelParticle",lBossModel.get());

		gpuParticleEmitter->AnimationModelGPUParticleSetTex("BossModelParticle",effect1Handle);
		gpuParticleEmitter->AnimationModelGPUParticleEmit("BossModelParticle",lAnimationSetting);

		gpuParticleEmitter->AnimationModelGPUParticleCreate(1000000,"PlayerModelParticle");
		std::unique_ptr<AliceModel>lPlayerModel = std::make_unique<AliceModel>();
		lPlayerModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Particle"));
		gpuParticleEmitter->AnimationModelGPUParticleSetModel("PlayerModelParticle",lPlayerModel.get());

		lAnimationSetting.size = 0.05f;

		gpuParticleEmitter->AnimationModelGPUParticleSetTex("PlayerModelParticle",effect1Handle);
		gpuParticleEmitter->AnimationModelGPUParticleEmit("PlayerModelParticle",lAnimationSetting);

		gpuParticleEmitter->ModelGPUParticleCreate(1000000,"PiayerWeaponParticle");
		std::unique_ptr<AliceModel>lPlayerWeaponModel = std::make_unique<AliceModel>();
		lPlayerWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/Sword"));
		gpuParticleEmitter->ModelGPUParticleSetModel("PiayerWeaponParticle",lPlayerWeaponModel.get());
		gpuParticleEmitter->ModelGPUParticleSetTex("PiayerWeaponParticle",effect1Handle);

		gpuParticleEmitter->ModelGPUParticleEmit("PiayerWeaponParticle",lModelSetting);

		gpuParticleEmitter->ModelGPUParticleCreate(1000000,"PiayerGreatWeaponParticle");
		std::unique_ptr<AliceModel>lPlayerGreatWeaponModel = std::make_unique<AliceModel>();
		lPlayerGreatWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/GreatSword"));
		gpuParticleEmitter->ModelGPUParticleSetModel("PiayerGreatWeaponParticle",lPlayerGreatWeaponModel.get());
		gpuParticleEmitter->ModelGPUParticleSetTex("PiayerGreatWeaponParticle",effect1Handle);

		gpuParticleEmitter->ModelGPUParticleEmit("PiayerGreatWeaponParticle",lModelSetting);
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
