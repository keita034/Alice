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
