#include "AlicePhysics.h"

AlicePhysics::~AlicePhysics()
{
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

void AlicePhysics::Initialize()
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
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);
	// PVD の設定
	physx::PxPvdSceneClient* pvdClient = scene->getScenePvdClient();

	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}
