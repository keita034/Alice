#include "GameMain.h"
#include"SceneFactrory.h"

void GameMain::Initialize()
{
	AliceFramework::Initialize();
	directX12Core->SetBackScreenColor(5 / 255.f, 5 / 255.f, 5 / 255.f, 1.0f);

	int32_t emmisionFireElemental = TextureManager::SLoad("Resources/Model/Boss/emmision fire_elemental.png",AdaptersIndex::SUB);
	int32_t fireEffectNoiseHandle = TextureManager::SLoad("Resources/Default/Particle/FireEffectNoise.png");
	int32_t effect1Handle = TextureManager::SLoad("Resources/Default/Particle/effect1.png");
	int32_t effect2Handle = TextureManager::SLoad("Resources/Default/Particle/effect2.png");
	int32_t fireEffectNoiseInvHandle = TextureManager::SLoad("Resources/Default/Particle/FireEffectNoiseInv.png");
	int32_t white1x1Handle = TextureManager::SLoad("Resources/Default/white1x1.png",AdaptersIndex::SUB);

	{
		gpuParticleEmitter->FireParticleCreate(100000,"BossHandParticle");
		gpuParticleEmitter->FireParticleSetTex("BossHandParticle",fireEffectNoiseHandle);
	}

	{
		gpuParticleEmitter->ShockWaveParticleCreate(100000,"BossShockWaveParticle");
		gpuParticleEmitter->ShockWaveParticleSetTex("BossShockWaveParticle",effect1Handle);
	}

	{
		gpuParticleEmitter->LaserParticleCreate(10000,"BossLaserParticle");
		gpuParticleEmitter->LaserParticleSetMainTex("BossLaserParticle",effect1Handle);
		gpuParticleEmitter->LaserParticleSetSubTex("BossLaserParticle",effect2Handle);
	}

	{
		//gpuParticleEmitter->AnimationMeshGPUParticleCreate(100000,"BossParticle");
		//std::unique_ptr<AliceModel>lModel = std::make_unique<AliceModel>();
		//lModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss"));
		//gpuParticleEmitter->AnimationMeshGPUParticleSetModel("BossParticle",lModel.get());
		//gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex("BossParticle",emmisionFireElemental);
		//gpuParticleEmitter->AnimationMeshGPUParticleSetTex("BossParticle",fireEffectNoiseHandle);
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

		lModelSetting.startColor = { 1,1,1,1 };
		lModelSetting.size = 1.5f;
		{
			gpuParticleEmitter->ModelGPUParticleCreate(1000000,"FieldModelParticle");
			std::unique_ptr<AliceModel>lFieldModel = std::make_unique<AliceModel>();
			lFieldModel->SetModel(AliceModel::SCreateModel("Resources/Model/Wall/Particle"));
			gpuParticleEmitter->ModelGPUParticleSetModel("FieldModelParticle",lFieldModel.get());
			gpuParticleEmitter->ModelGPUParticleSetTex("FieldModelParticle",effect1Handle);

			gpuParticleEmitter->ModelGPUParticleEmit("FieldModelParticle",lModelSetting);
		}

		{
			gpuParticleEmitter->ModelGPUParticleCreate(1000000,"Field2ModelParticle");
			std::unique_ptr<AliceModel>lFieldModel = std::make_unique<AliceModel>();
			lFieldModel->SetModel(AliceModel::SCreateModel("Resources/Model/Wall/Particle2"));
			gpuParticleEmitter->ModelGPUParticleSetModel("Field2ModelParticle",lFieldModel.get());
			gpuParticleEmitter->ModelGPUParticleSetTex("Field2ModelParticle",effect1Handle);

			gpuParticleEmitter->ModelGPUParticleEmit("Field2ModelParticle",lModelSetting);
		}

		{
			gpuParticleEmitter->ModelGPUParticleCreate(1000000,"GroundModelParticle");
			std::unique_ptr<AliceModel>lFieldModel = std::make_unique<AliceModel>();
			lFieldModel->SetModel(AliceModel::SCreateModel("Resources/Model/Ground/Particle"));
			gpuParticleEmitter->ModelGPUParticleSetModel("GroundModelParticle",lFieldModel.get());
			gpuParticleEmitter->ModelGPUParticleSetTex("GroundModelParticle",effect1Handle);

			gpuParticleEmitter->ModelGPUParticleEmit("GroundModelParticle",lModelSetting);
		}

		lModelSetting.startColor = { 0.0f,0.0f,1.0f,1 };
		lModelSetting.size = 0.05f;
		{
			gpuParticleEmitter->ModelGPUParticleCreate(1000000,"PiayerWeaponParticle");
			std::unique_ptr<AliceModel>lPlayerWeaponModel = std::make_unique<AliceModel>();
			lPlayerWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/Sword"));
			gpuParticleEmitter->ModelGPUParticleSetModel("PiayerWeaponParticle",lPlayerWeaponModel.get());
			gpuParticleEmitter->ModelGPUParticleSetTex("PiayerWeaponParticle",effect1Handle);

			gpuParticleEmitter->ModelGPUParticleEmit("PiayerWeaponParticle",lModelSetting);
		}

		{
			gpuParticleEmitter->ModelGPUParticleCreate(1000000,"PiayerGreatWeaponParticle");
			std::unique_ptr<AliceModel>lPlayerGreatWeaponModel = std::make_unique<AliceModel>();
			lPlayerGreatWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/GreatSword"));
			gpuParticleEmitter->ModelGPUParticleSetModel("PiayerGreatWeaponParticle",lPlayerGreatWeaponModel.get());
			gpuParticleEmitter->ModelGPUParticleSetTex("PiayerGreatWeaponParticle",effect1Handle);

			gpuParticleEmitter->ModelGPUParticleEmit("PiayerGreatWeaponParticle",lModelSetting);
		}
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
		gpuParticleEmitter->MeshGPUParticleCreate(1000000,"PiayerWeaponScatteringParticle");
		std::unique_ptr<AliceModel>lWeaponModel = std::make_unique<AliceModel>();
		lWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/Sword"));
		gpuParticleEmitter->MeshGPUParticleSetModel("PiayerWeaponScatteringParticle",lWeaponModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("PiayerWeaponScatteringParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("PiayerWeaponScatteringParticle",fireEffectNoiseHandle);

		gpuParticleEmitter->MeshGPUParticleCreate(1000000,"PiayerGreatWeaponScatteringParticle");
		lWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/GreatSword"));
		gpuParticleEmitter->MeshGPUParticleSetModel("PiayerGreatWeaponScatteringParticle",lWeaponModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("PiayerGreatWeaponScatteringParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("PiayerGreatWeaponScatteringParticle",fireEffectNoiseHandle);

		MeshGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.velocity = { 0,1,0 };
		lSetting.startColor = { 0.0f,0.0f,1.0f,1 };
		lSetting.endColor = { 0.0f,0.0f,1.0f,1 };
		lSetting.lifeTime = 1.0f;
		lSetting.maxParticles = 100000.0f;
		lSetting.timeBetweenEmit = 0.001f;
		lSetting.emitLifeTime = 0.001f;
		lSetting.size = 0.05f;
		lSetting.speed = 15;
		lSetting.isPlay = false;

		gpuParticleEmitter->MeshGPUParticleEmit("PiayerWeaponScatteringParticle",lSetting);
		gpuParticleEmitter->MeshGPUParticleEmit("PiayerGreatWeaponScatteringParticle",lSetting);
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
