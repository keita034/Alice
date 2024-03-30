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

	std::string lParticleBossPath = "Resources/Model/Boss/Particle";
	std::unique_ptr<AliceModel>lBossModel = std::make_unique<AliceModel>();
	lBossModel->SetModel(AliceModel::SCreateModel(lParticleBossPath));

	//{
	//	gpuParticleEmitter->FireParticleCreate(1000000,"BossHandParticle");
	//	gpuParticleEmitter->FireParticleSetTex("BossHandParticle",fireEffectNoiseHandle);
	//}

	//{
	//	gpuParticleEmitter->ShockWaveParticleCreate(100000,"BossShockWaveParticle");
	//	gpuParticleEmitter->ShockWaveParticleSetTex("BossShockWaveParticle",effect1Handle);
	//}

	{
		gpuParticleEmitter->LaserParticleCreate(10000,"BossLaserParticle");
		gpuParticleEmitter->LaserParticleSetMainTex("BossLaserParticle",effect1Handle);
		gpuParticleEmitter->LaserParticleSetSubTex("BossLaserParticle",effect2Handle);
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
		gpuParticleEmitter->AnimationModelGPUParticleSetModel("BossModelParticle",lBossModel.get());

		gpuParticleEmitter->AnimationModelGPUParticleSetTex("BossModelParticle",effect1Handle);
		gpuParticleEmitter->AnimationModelGPUParticleEmit("BossModelParticle",lAnimationSetting);

		gpuParticleEmitter->AnimationModelGPUParticleCreate(1000000,"BossLeftHandModelParticle");
		gpuParticleEmitter->AnimationModelGPUParticleSetModel("BossLeftHandModelParticle",lBossModel.get());

		gpuParticleEmitter->AnimationModelGPUParticleSetTex("BossLeftHandModelParticle",effect1Handle);
		gpuParticleEmitter->AnimationModelGPUParticleEmit("BossLeftHandModelParticle",lAnimationSetting);

		gpuParticleEmitter->AnimationModelGPUParticleCreate(1000000,"BossRightHandModelParticle");
		gpuParticleEmitter->AnimationModelGPUParticleSetModel("BossRightHandModelParticle",lBossModel.get());

		gpuParticleEmitter->AnimationModelGPUParticleSetTex("BossRightHandModelParticle",effect1Handle);
		gpuParticleEmitter->AnimationModelGPUParticleEmit("BossRightHandModelParticle",lAnimationSetting);

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
		std::string lBossLeftHandParticle = "BossLeftHandParticle";
		std::string lBossRightHandParticle = "BossRightHandParticle";

		gpuParticleEmitter->AnimationMeshGPUParticleCreate(100000,lBossLeftHandParticle);
		gpuParticleEmitter->AnimationMeshGPUParticleSetModel(lBossLeftHandParticle,lBossModel.get());
		gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex(lBossLeftHandParticle,white1x1Handle);
		gpuParticleEmitter->AnimationMeshGPUParticleSetTex(lBossLeftHandParticle,effect1Handle);

		gpuParticleEmitter->AnimationMeshGPUParticleCreate(100000,lBossRightHandParticle);
		gpuParticleEmitter->AnimationMeshGPUParticleSetModel(lBossRightHandParticle,lBossModel.get());
		gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex(lBossRightHandParticle,white1x1Handle);
		gpuParticleEmitter->AnimationMeshGPUParticleSetTex(lBossRightHandParticle,effect1Handle);

		AnimationMeshGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.velocity = { 0,0,0 };
		lSetting.startColor = { 0.0f,0.0f,1.0f,1 };
		lSetting.endColor = { 0.0f,0.0f,1.0f,1 };
		lSetting.lifeTime = 0.3f;
		lSetting.maxParticles = 100000.0f;
		lSetting.timeBetweenEmit = 0.001f;
		lSetting.emitLifeTime = 0.001f;
		lSetting.size = 2.0f;
		lSetting.speed = 15;
		lSetting.isPlay = false;

		gpuParticleEmitter->AnimationMeshGPUParticleEmit(lBossLeftHandParticle,lSetting);
		gpuParticleEmitter->GetAnimationMeshGPUParticle(lBossLeftHandParticle)->SetBoneMesh("elemental_element1mesh.003","mixamorig:LeftHand");

		gpuParticleEmitter->AnimationMeshGPUParticleEmit(lBossRightHandParticle,lSetting);
		gpuParticleEmitter->GetAnimationMeshGPUParticle(lBossRightHandParticle)->SetBoneMesh("elemental_element1mesh.003","mixamorig:RightHand");

	}

	{
		gpuParticleEmitter->MeshGPUParticleCreate(500000,"BossWeaponParticle");
		std::unique_ptr<AliceModel>lWeaponModel = std::make_unique<AliceModel>();
		lWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss/Weapon/Sword"));
		gpuParticleEmitter->MeshGPUParticleSetModel("BossWeaponParticle",lWeaponModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("BossWeaponParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("BossWeaponParticle",fireEffectNoiseHandle);
	}

	{
		gpuParticleEmitter->MeshGPUParticleCreate(500000,"PiayerWeaponScatteringParticle");
		std::unique_ptr<AliceModel>lWeaponModel = std::make_unique<AliceModel>();
		lWeaponModel->SetModel(AliceModel::SCreateModel("Resources/Model/Player/Weapon/Sword"));
		gpuParticleEmitter->MeshGPUParticleSetModel("PiayerWeaponScatteringParticle",lWeaponModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("PiayerWeaponScatteringParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("PiayerWeaponScatteringParticle",fireEffectNoiseHandle);

		gpuParticleEmitter->MeshGPUParticleCreate(500000,"PiayerGreatWeaponScatteringParticle");
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
		gpuParticleEmitter->AnimationMeshGPUParticleCreate(500000,"BossHandScatteringParticle");
		gpuParticleEmitter->AnimationMeshGPUParticleSetModel("BossHandScatteringParticle",lBossModel.get());
		gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex("BossHandScatteringParticle",white1x1Handle);
		gpuParticleEmitter->AnimationMeshGPUParticleSetTex("BossHandScatteringParticle",fireEffectNoiseHandle);

		AnimationMeshGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.velocity = { 0,0,0 };
		lSetting.startColor = { 0.9f,0.1f,0.0f,1 };
		lSetting.endColor = { 0.9f,0.1f,0.0f,1 };
		lSetting.size = 2.0f;
		lSetting.lifeTime = 0.5f;
		lSetting.maxParticles = 100000.0f;
		lSetting.timeBetweenEmit = 0.001f;
		lSetting.emitLifeTime = 0.001f;
		lSetting.speed = 15;
		lSetting.isPlay = false;

		gpuParticleEmitter->AnimationMeshGPUParticleEmit("BossHandScatteringParticle",lSetting);
	}

	{
		gpuParticleEmitter->BloodGushGPUParticleCreate(100000,"BossBloodGushParticle");
		gpuParticleEmitter->BloodGushGPUParticleSetTex("BossBloodGushParticle",fireEffectNoiseHandle);
	}

	{
		MeshGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.velocity = { 0,1,0 };
		lSetting.startColor = { 0.9f,0.1f,0.0f,1 };
		lSetting.endColor = { 0.9f,0.1f,0.0f,1 };
		lSetting.lifeTime = 1.0f;
		lSetting.maxParticles = 100000.0f;
		lSetting.timeBetweenEmit = 0.001f;
		lSetting.emitLifeTime = 0.001f;
		lSetting.size = 2.0f;
		lSetting.speed = 15;
		lSetting.isPlay = false;

		std::unique_ptr<AliceModel>lSphereModel = std::make_unique<AliceModel>();
		lSphereModel->SetModel(AliceModel::SCreateModel("Resources/Default/Particle/"));

		gpuParticleEmitter->MeshGPUParticleCreate(500000,"EffectSphereParticle");
		gpuParticleEmitter->MeshGPUParticleSetModel("EffectSphereParticle",lSphereModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("EffectSphereParticle",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("EffectSphereParticle",fireEffectNoiseHandle);
		gpuParticleEmitter->MeshGPUParticleEmit("EffectSphereParticle",lSetting);

		lSphereModel->SetModel(AliceModel::SCreateModel("Resources/Default/sphere"));
		lSetting.startColor = { 0.0f,0.9f,0.1f,1 };
		lSetting.endColor = { 0.0f,0.9f,0.1f,1 };
		lSetting.lifeTime = 0.55f;
		gpuParticleEmitter->MeshGPUParticleCreate(500000,"playerHealingEffect");
		gpuParticleEmitter->MeshGPUParticleSetModel("playerHealingEffect",lSphereModel.get());
		gpuParticleEmitter->MeshGPUParticleSetDetermineTex("playerHealingEffect",white1x1Handle);
		gpuParticleEmitter->MeshGPUParticleSetTex("playerHealingEffect",effect1Handle);
		gpuParticleEmitter->MeshGPUParticleEmit("playerHealingEffect",lSetting);
	}

	{
		gpuParticleEmitter->AnimationMeshGPUParticleCreate(800000,"BossDeathEffectParticle");
		gpuParticleEmitter->AnimationMeshGPUParticleSetModel("BossDeathEffectParticle",lBossModel.get());
		gpuParticleEmitter->AnimationMeshGPUParticleSetDetermineTex("BossDeathEffectParticle",white1x1Handle);
		gpuParticleEmitter->AnimationMeshGPUParticleSetTex("BossDeathEffectParticle",fireEffectNoiseHandle);

		AnimationMeshGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.velocity = { 0,1,0 };
		lSetting.startColor = { 1.0f,1.0f,1.0f,1 };
		lSetting.endColor = { 1.0f,1.0f,1.0f,1 };
		lSetting.lifeTime = 1.0f;
		lSetting.maxParticles = 100000.0f;
		lSetting.timeBetweenEmit = 0.001f;
		lSetting.emitLifeTime = 0.001f;
		lSetting.size = 2.0f;
		lSetting.speed = 15;
		lSetting.isPlay = false;

		gpuParticleEmitter->AnimationMeshGPUParticleEmit("BossDeathEffectParticle",lSetting);
		gpuParticleEmitter->GetAnimationMeshGPUParticle("BossDeathEffectParticle")->SetBoneMesh("elemental_element1mesh.003","mixamorig:Hips");
	}

	{
		gpuParticleEmitter->AnimationModelSuctionGPUParticleCreate(1000000,"BossLeftHandScatterinParticle");
		gpuParticleEmitter->AnimationModelSuctionGPUParticleSetModel("BossLeftHandScatterinParticle",lBossModel.get());
		gpuParticleEmitter->AnimationModelSuctionGPUParticleSetTex("BossLeftHandScatterinParticle",fireEffectNoiseHandle);

		AnimationModelSuctionGPUParticleSetting lSetting;

		lSetting.matWorld = AliceMathF::MakeIdentity();
		lSetting.size = 0.8f;
		lSetting.startColor = { 0.0f,1.0f,1.0f,1 };
		lSetting.endColor = { 0.0f,1.0f,1.0f,1 };
		lSetting.distance = 10.0f;
		lSetting.lifeTime = 200.0f;
		lSetting.invisibleTime = 0.2f;
		lSetting.moveTime = 0.2f;
		lSetting.timeBetweenEmit = 0.1f;
		lSetting.emitLifeTime = 0.001f;
		lSetting.isInfinityEmit = true;
		lSetting.isPlay = false;

		gpuParticleEmitter->AnimationModelSuctionGPUParticleEmit("BossLeftHandScatterinParticle",lSetting);
		gpuParticleEmitter->GetAnimationModelSuctionGPUParticle("BossLeftHandScatterinParticle")->SetBoneMesh("elemental_element1mesh.003","mixamorig:LeftHand");
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
