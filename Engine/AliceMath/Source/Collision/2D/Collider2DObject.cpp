#include "Collider2DObject.h"

void Collider2DObject::SetColl(Base2DCollider* coll)
{
	collider.reset(coll);
}

Base2DCollider* Collider2DObject::GetCollider()
{
	return collider.get();
}
