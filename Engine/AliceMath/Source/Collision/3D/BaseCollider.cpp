#include "BaseCollider.h"
#include<ColliderObject.h>

void BaseCollider::OnCollision()
{
	object->OnCollision();
}

uint32_t BaseCollider::GetCollisionAttribute()
{
	return collisionAttribute;
}

void BaseCollider::SetCollisionAttribute(uint32_t collisionAttribute_)
{
	collisionAttribute = collisionAttribute_;
}

uint32_t BaseCollider::GetCollisionMask()
{
	return CollisionMask;
}

void BaseCollider::SetCollisionMask(uint32_t CollisionMask_)
{
	CollisionMask = CollisionMask_;
}

const std::string& BaseCollider::GetCollsionName()const
{
	return collsionName;
}

const std::string& BaseCollider::GetOpponentCollsionName()const
{
	return opponentCollsionName;
}

void BaseCollider::SetObject(ColliderObject* object_)
{
	object = object_;
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

void BaseCollider::SetCollsionName(const std::string& name_)
{
	collsionName = name_;
}

void BaseCollider::SetOpponentCollsionName(const std::string& name_)
{
	opponentCollsionName = name_;
}
