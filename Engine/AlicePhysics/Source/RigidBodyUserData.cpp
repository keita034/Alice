#include "RigidBodyUserData.h"

#include<IRigidBody.h>

void AlicePhysics::RigidBodyUserData::SetName(const std::string& name_)
{
	name = name_;
}

void AlicePhysics::RigidBodyUserData::SetAttribute(CollisionAttribute attribute_)
{
	attribute = attribute_;
}

void AlicePhysics::RigidBodyUserData::SetGroup(CollisionGroup group_)
{
	group= group_;
}

void AlicePhysics::RigidBodyUserData::SetUserData(void* userData_)
{
	userData = userData_;
}

void AlicePhysics::RigidBodyUserData::SetRigidBody(IRigidBody* rigidBody_)
{
	rigidBody = rigidBody_;
}

AlicePhysics::IRigidBody* AlicePhysics::RigidBodyUserData::GetRigidBody()
{
	return rigidBody;
}

const std::string& AlicePhysics::RigidBodyUserData::GetName() const
{
	return name;
}

CollisionAttribute AlicePhysics::RigidBodyUserData::GetAttribute() const
{
	return attribute;
}

CollisionGroup AlicePhysics::RigidBodyUserData::GetGroup() const
{
	return group;
}

void* AlicePhysics::RigidBodyUserData::GetUserData()
{
    return userData;
}

void* AlicePhysics::RigidBodyUserData::GetRigidBodyUserData()
{
	return static_cast<void*>(this);
}
