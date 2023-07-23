#include "AlicePhysics.h"
#include <FilterShader.h>
#include<IAliceRigidBody.h>

AlicePhysics::~AlicePhysics()
{
	cooking->release();
	PxCloseExtensions();
	scene->release();
	dispatcher->release();
	physics->release();
	if (pvd)
	{
		pvd->disconnect();
		physx::PxPvdTransport* transport = pvd->getTransport();
		pvd->release();
		transport->release();
	}

	foundation->release();
}

void AlicePhysics::Initialize(AliceRigidBodyManager* aliceRigidBodyManager)
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator, defaultErrorCallback);
		
	pvd = physx::PxCreatePvd(*foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale(), true, pvd);
	PxInitExtensions(*physics, pvd);
	// Scene の作成
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	dispatcher = physx::PxDefaultCpuDispatcherCreate(8);
	sceneDesc.cpuDispatcher = dispatcher;
	sceneDesc.filterShader = FilterShader;
	sceneDesc.simulationEventCallback = aliceRigidBodyManager;
	scene = physics->createScene(sceneDesc);
	// PVD の設定
	physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();

	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, physx::PxCookingParams(physics->getTolerancesScale()));

	IAliceRigidBody::SetPhysics(physics);
	IAliceRigidBody::SetScene(scene);
	IAliceRigidBody::SetCooking(cooking);
}

void AlicePhysics::SimulateTime(float time_)
{
	static_cast<void>(time_);
	const physx::PxReal kElapsedTime = 1.0f / 60.0f; // 60Hz
	scene->simulate(kElapsedTime);
	scene->fetchResults(true);
}
