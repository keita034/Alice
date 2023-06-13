#include "Collider2DObject.h"

void Collider2DObject::SetColl(Base2DCollider* collider_)
{
	collider.reset(collider_);
}

Base2DCollider* Collider2DObject::GetCollider()
{
	return collider.get();
}
