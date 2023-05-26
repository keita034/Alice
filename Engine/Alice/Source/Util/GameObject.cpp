#include<GameObject.h>

void GameObject::SetCollider(BaseCollider* coll)
{
	SetColl(coll);

	collider->SetObject(this);
}

void GameObject::OnCollision()
{
}

const Transform* GameObject::GetTransformPtr() const
{
	return &transform;
}

void GameObject::SetName(const std::string& objectName)
{
	name = objectName;
}
