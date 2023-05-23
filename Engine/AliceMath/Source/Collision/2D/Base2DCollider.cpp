#include "Base2DCollider.h"
#include<Collider2DObject.h>
#include<Collision2DManager.h>

void Base2DCollider::OnCollision()
{
	object->OnCollision();
}

uint32_t Base2DCollider::GetCollisionAttribute()
{
	return collisionAttribute;
}

void Base2DCollider::SetCollisionAttribute(uint32_t collisionAttribute_)
{
	collisionAttribute = collisionAttribute_;
}

uint32_t Base2DCollider::GetCollisionMask()
{
	return CollisionMask;
}

void Base2DCollider::SetCollisionMask(uint32_t CollisionMask_)
{
	CollisionMask = CollisionMask_;
}

const std::string& Base2DCollider::GetCollsionName()
{
	return collsionName;
}

void Base2DCollider::SetOpponentCollider(Base2DCollider* col)
{
	opponentCollider = col;
}

Base2DCollider* Base2DCollider::GetOpponentCollider()
{
	return opponentCollider;
}

const std::string& Base2DCollider::GetOpponentCollsionName()
{
	return opponentCollsionName;
}

void Base2DCollider::SetObject(Collider2DObject* obj)
{
	object = obj;
}

Collider2DObject* Base2DCollider::GetColliderObject()
{
	return object;
}

void Base2DCollider::Valid()
{
	Collision2DManager::GetInstance()->AddCollider(this);
	isValid = true;
}

void Base2DCollider::Invalid()
{
	Collision2DManager::GetInstance()->RemoveCollider(this);
	isValid = false;
}

bool Base2DCollider::IsValid()
{
	return isValid;
}

CollisionShapeType Base2DCollider::GetShapeType()
{
	return shapeType;
}

void Base2DCollider::CollisionUpdate()
{
	object->CollisionUpdate();
}

void Base2DCollider::SetCollsionName(const std::string& name)
{
	collsionName = name;
}

void Base2DCollider::SetOpponentCollsionName(const std::string& name)
{
	opponentCollsionName = name;
}
