#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)

#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#pragma warning(pop)

#include<SceneManager.h>

#include<FadeOutTransition.h>

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::Initialize(const std::string& previewSceneName_)
{
	sphere = AlicePhysics::CreateSphereShape(0.5f);
	
	AlicePhysics::IRigidBodyCreationSettings sphereBodySettings;

	sphereBodySettings.name = "sphere";
	sphereBodySettings.shape = sphere;
	sphereBodySettings.position = { 0.0f,20.0f,0.0f };
	sphereBodySettings.motionType = AlicePhysics::MotionType::DYNAMIC;
	sphereBodySettings.collisionAttribute = CollisionAttribute::BODY;
	sphereBodySettings.collisionGroup = CollisionGroup::PLAYER;
	sphereBodySettings.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
	sphereBodySettings.restitution = 0.5f;
	sphereBodySettings.allowSleeping = false;
	sPhysicsSystem->CreateRigidBody(sphereBody,&sphereBodySettings);
	sPhysicsSystem->AddRigidBody(sphereBody);
	sphereBody->SetLinearVelocity({ 0.0f, -5.0f, 0.0f });
	sphereBody->SetRigidBodyCollision(&testRigidBody);

	box = AlicePhysics::CreateBoxShape({ 100.0f, 1.0f, 100.0f });

	AlicePhysics::IRigidBodyCreationSettings boxBodySettings;

	boxBodySettings.name = "box";
	boxBodySettings.shape = box;
	boxBodySettings.position = { 0.0f,-1.0f,0.0f };
	boxBodySettings.motionType = AlicePhysics::MotionType::STATIC;
	boxBodySettings.collisionAttribute = CollisionAttribute::BODY;
	boxBodySettings.collisionGroup = CollisionGroup::ENEMY;
	boxBodySettings.type = AlicePhysics::PhysicsRigidBodyType::STATIC;
	boxBodySettings.isActive = false;
	sPhysicsSystem->CreateRigidBody(boxBody,&boxBodySettings);
	sPhysicsSystem->AddRigidBody(boxBody);

	camera = std::make_unique<GameCamera>();
	camera->SetEye(0.0f,2.0f,-15.0f);
	camera->Initialize(UPDATE_PROJMATRIXFUNC_PERSPECTIVE);

	light = std::make_unique<Light>();
	light->Initialize();

}

void GameScene::Update()
{
	if ( sInput->TriggerKey(Keys::ENTER) )
	{
		sphereBody->SetLinearVelocity({ 0.0f, 10.0f, 0.0f });
	}
}

void GameScene::Draw()
{
	box->Draw(boxBody->GetCenterOfMassTransform(),{ 1.0f,1.0f,1.0f },{ 1.0f,1.0f,1.0f,1.0f },true);
	sphere->Draw(sphereBody->GetCenterOfMassTransform(),{ 1.0f,1.0f,1.0f },{0.0f,0.0f,1.0f,1.0f},true);
}

void GameScene::Finalize()
{
	
}

