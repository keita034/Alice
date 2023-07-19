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

		if (curContactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{

			for (size_t j = 0; j < 2;j++)
			{
				std::string lRigidBodyName =  static_cast<const char*>(curContactPair.shapes[j]->userData);

				IAliceRigidBody* lRigidBody = rigidBodyMap.at(lRigidBodyName);
				lRigidBody->OnContact();
			}
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

		{
			physx::PxRigidActor* triggerActor = curTriggerPair.triggerActor;
			std::string lRigidBodyName = static_cast<const char*>(triggerActor->userData);

			IAliceRigidBody* lRigidBody = rigidBodyMap.at(lRigidBodyName);
			lRigidBody->OnContact();
		}

		{
			physx::PxRigidActor* otherActor = curTriggerPair.otherActor;
			std::string lRigidBodyName = static_cast<const char*>(otherActor->userData);

			IAliceRigidBody* lRigidBody = rigidBodyMap.at(lRigidBodyName);
			lRigidBody->OnContact();
		}
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
