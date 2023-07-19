#include "GameObject.h"
#include<GameObject.h>

void GameObject::SetCollider(BaseCollider* coll_)
{
	SetColl(coll_);

	collider->SetObject(this);
}

void GameObject::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_);
}

void GameObject::OnCollision()
{
}

const Transform* GameObject::GetTransformPtr() const
{
	return &transform;
}

void GameObject::SetName(const std::string& objectName_)
{
	name = objectName_;
}

void GameObject::OnTrigger()
{
}

void GameObject::OnContact()
{
}
