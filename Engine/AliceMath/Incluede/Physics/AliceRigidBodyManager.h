#pragma once

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5220)

#include<PxPhysicsAPI.h>
#include<memory>
#include<string>
#include<unordered_map>
#pragma warning(pop)

class IAliceRigidBody;

class AliceRigidBodyManager : public physx::PxSimulationEventCallback
{
private:

	//リジットボディ群
	std::unordered_map<std::string, IAliceRigidBody*> rigidBodyMap;

public:
	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbpairs)override;

	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)override;

	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)override;
	virtual void onWake(physx::PxActor** actors, physx::PxU32 count)override;
	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count)override;
	virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)override;

	void AddRigidBody(IAliceRigidBody* AliceRigidBody);
	void RemoveRigidBody(IAliceRigidBody* AliceRigidBody);
};