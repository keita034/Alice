#include "BaseCollider.h"
#include<ColliderObject.h>

void BaseCollider::OnCollision()
{
	object->OnCollision();
}

const uint32_t BaseCollider::GetCollisionAttribute()
{
	return collisionAttribute;
}

void BaseCollider::SetCollisionAttribute(uint32_t collisionAttribute_)
{
	collisionAttribute = collisionAttribute_;
}

const uint32_t BaseCollider::GetCollisionMask()
{
	return CollisionMask;
}

void BaseCollider::SetCollisionMask(uint32_t CollisionMask_)
{
	CollisionMask = CollisionMask_;
}

const std::string& BaseCollider::GetCollsionName()
{
	return collsionName;
}

const std::string& BaseCollider::GetOpponentCollsionName()
{
	return opponentCollsionName;
}

void BaseCollider::SetObject(ColliderObject* obj)
{
	object = obj;
}

ColliderObject* BaseCollider::GetColliderObject()
{
	return object;
}

void BaseCollider::Valid()
{
	isValid = true;
}

void BaseCollider::Invalid()
{
	isValid = false;
}

bool BaseCollider::IsValid()
{
	return isValid;
}

void BaseCollider::SetCollsionName(const std::string& name)
{
	collsionName = name;
}

void BaseCollider::SetOpponentCollsionName(const std::string& name)
{
	opponentCollsionName = name;
}
