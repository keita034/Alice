#include "ColliderObject.h"

void ColliderObject::SetColl(BaseCollider* coll)
{
	collider.reset(coll);
}

BaseCollider* ColliderObject::GetCollider()
{
	return collider.get();
}