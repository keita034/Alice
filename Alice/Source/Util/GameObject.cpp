#include "GameObject.h"

void GameObject::SetCollider(BaseCollider* coll)
{
	SetColl(coll);

	collider->SetObject(this);
}

void GameObject::OnCollision()
{
}
