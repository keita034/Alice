#pragma once

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)
#pragma warning(disable: 5220)

#include<PxPhysicsAPI.h>

#pragma warning(pop)

#include<AliceRigidBodyManager.h>

class AlicePhysics final
{
private:
	physx::PxDefaultAllocator defaultAllocator;

	physx::PxDefaultErrorCallback defaultErrorCallback;

	physx::PxFoundation* foundation;

	physx::PxPhysics* physics = nullptr;

	physx::PxDefaultCpuDispatcher* dispatcher = nullptr;

	physx::PxScene* scene = nullptr;

	physx::PxPvd* pvd = nullptr;

	physx::PxCooking* cooking = nullptr;

public:

	AlicePhysics() = default;
	~AlicePhysics();

	void Finalize();

	void Initialize(AliceRigidBodyManager* aliceRigidBodyManager);

	void SimulateTime(float time_);
};

