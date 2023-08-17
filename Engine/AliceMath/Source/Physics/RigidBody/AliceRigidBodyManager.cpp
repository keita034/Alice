#include "AliceRigidBodyManager.h"
#include<IAliceRigidBody.h>

void AliceRigidBodyManager::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbpairs)
{
	static_cast<void>(pairHeader);

	//PhysXシミュレーションのすべてのコンタクトペアをループする
	for (physx::PxU32 i = 0; i < nbpairs; i++)
	{
		//現在のコンタクトペアからコンタントの情報を抽出 
		const physx::PxContactPair& curContactPair = pairs[i];

		if (curContactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND || curContactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			physx::PxRigidActor* triggerActor = pairHeader.actors[0];
			physx::PxRigidActor* otherActor = pairHeader.actors[1];

			RigidBodyUserData* lTriggerRigidBodyData = static_cast<RigidBodyUserData*>(triggerActor->userData);
			RigidBodyUserData* lOtherRigidBodyData = static_cast<RigidBodyUserData*>(otherActor->userData);

			IAliceRigidBody* lTriggerRigidBody = rigidBodyMap.at(lTriggerRigidBodyData->id);
			IAliceRigidBody* lOtherRigidBody = rigidBodyMap.at(lOtherRigidBodyData->id);

			lTriggerRigidBody->OnContact(lOtherRigidBody->GetUserData()->attribute);
			lOtherRigidBody->OnContact(lTriggerRigidBody->GetUserData()->attribute);
		}
	}

}

void AliceRigidBodyManager::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		// 図形が削除されたペアを無視する
		if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const physx::PxTriggerPair& curTriggerPair = pairs[i];

		physx::PxRigidActor* triggerActor = curTriggerPair.triggerActor;
		physx::PxRigidActor* otherActor = curTriggerPair.otherActor;

		RigidBodyUserData* lTriggerRigidBodyData = static_cast<RigidBodyUserData*>(triggerActor->userData);
		RigidBodyUserData* lOtherRigidBodyData = static_cast<RigidBodyUserData*>(otherActor->userData);

		IAliceRigidBody* lTriggerRigidBody = rigidBodyMap.at(lTriggerRigidBodyData->id);
		IAliceRigidBody* lOtherRigidBody = rigidBodyMap.at(lOtherRigidBodyData->id);

		lTriggerRigidBody->OnTrigger(lOtherRigidBody->GetUserData()->attribute);
		lOtherRigidBody->OnTrigger(lTriggerRigidBody->GetUserData()->attribute);
		int b = 0;
		b++;
	}
}

void AliceRigidBodyManager::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
	static_cast<void>(constraints);
	static_cast<void>(count);
}

void AliceRigidBodyManager::onWake(physx::PxActor** actors, physx::PxU32 count)
{
	static_cast<void>(actors);
	static_cast<void>(count);
}

void AliceRigidBodyManager::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
	static_cast<void>(count);
	static_cast<void>(actors);
}

void AliceRigidBodyManager::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
	static_cast<void>(bodyBuffer);
	static_cast<void>(poseBuffer);
	static_cast<void>(count);
}

void AliceRigidBodyManager::AddRigidBody(IAliceRigidBody* AliceRigidBody)
{
	rigidBodyMap[AliceRigidBody->GetName()] = AliceRigidBody;
}

void AliceRigidBodyManager::RemoveRigidBody(IAliceRigidBody* AliceRigidBody)
{
	rigidBodyMap.erase(AliceRigidBody->GetName());
}
