#include "ColliderObject.h"

void ColliderObject::SetColl(BaseCollider* collider_)
{
	collider.reset(collider_);
}

BaseCollider* ColliderObject::GetCollider()
{
	return collider.get();
}