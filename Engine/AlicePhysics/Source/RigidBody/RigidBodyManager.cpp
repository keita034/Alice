#include "RigidBodyManager.h"

void AlicePhysics::RigidBodyManager::AddRigidBody(std::unique_ptr<IRigidBody>& rigidBody_,uint32_t id_)
{
	if ( rigidBodys.find(id_) == rigidBodys.end() )
	{
		rigidBodys.insert(std::pair<uint32_t,std::unique_ptr<IRigidBody>>(id_,std::move(rigidBody_)));
	}
	else
	{
		printf("ID:%dは追加済みです\n",id_);
	}
}

void AlicePhysics::RigidBodyManager::RemoveRigidBody(uint32_t id_)
{
	if ( rigidBodys.find(id_) != rigidBodys.end() )
	{
		rigidBodys.erase(id_);
	}
	else
	{
		printf("ID:%dは存在しません\n",id_);
	}
}

AlicePhysics::IRigidBody* AlicePhysics::RigidBodyManager::GetRigidBody(uint32_t id_)
{
	if ( rigidBodys.find(id_) != rigidBodys.end() )
	{
		return rigidBodys[ id_ ].get();
	}
	else
	{
		printf("ID:%dは存在しません\n",id_);

		return nullptr;
	}
}
