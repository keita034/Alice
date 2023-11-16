#include "GameMain.h"
#include"SceneFactrory.h"


void GameMain::Initialize()
{
	AliceFramework::Initialize();
	directX12Core->SetBackScreenColor(11 / 255.f, 15 / 255.f, 20 / 255.f, 1.0f);

	gpuParticleEmitter->FireParticleCreate(10000,"BossHandParticle");
	gpuParticleEmitter->FireParticleSetTex("BossHandParticle",TextureManager::SLoad("Resources/Default/Particle/FireEffectNoise.png"));

	gpuParticleEmitter->ShockWaveParticleCreate(10000,"BossShockWaveParticle");
	gpuParticleEmitter->ShockWaveParticleSetTex("BossShockWaveParticle",TextureManager::SLoad("Resources/Default/Particle/effect1.png"));

	gpuParticleEmitter->LaserParticleCreate(10000,"BossLaserParticle");
	gpuParticleEmitter->LaserParticleSetMainTex("BossLaserParticle",TextureManager::SLoad("Resources/Default/Particle/Laser1.png"));
	gpuParticleEmitter->LaserParticleSetSubTex("BossLaserParticle",TextureManager::SLoad("Resources/Default/Particle/effect2.png"));

	gpuParticleEmitter->MeshGPUParticleCreate(1000000,"BossModelParticle");

	std::unique_ptr<AliceModel>lModel = std::make_unique<AliceModel>();
	lModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss"));
	gpuParticleEmitter->MeshGPUParticleSetModel("BossModelParticle",lModel.get());
	gpuParticleEmitter->MeshGPUParticleSetDetermineTex("BossModelParticle",TextureManager::SLoad("Resources/Model/Boss/emmision fire_elemental.png"));

	MeshGPUParticleSetting lSetting;

	lSetting.matWorld = AliceMathF::MakeIdentity();
	lSetting.velocity = { 0,1,0 };
	lSetting.startColor = { 1,0,0,1 };
	lSetting.endColor = { 1,0,0,1 };
	lSetting.lifeTime = 1.0f;
	lSetting.maxParticles = 10000.0f;
	lSetting.timeBetweenEmit = 0.0001f;
	lSetting.emitLifeTime = -1;
	lSetting.size = 10;
	lSetting.speed = 10;
	gpuParticleEmitter->MeshGPUParticleEmit("BossModelParticle",lSetting);

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
