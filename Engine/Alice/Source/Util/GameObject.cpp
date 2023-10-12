#include "GameObject.h"
#include<GameObject.h>

void GameObject::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_);
}

const Transform* GameObject::GetTransformPtr() const
{
	return &transform;
}

void GameObject::SetName(const std::string& objectName_)
{
	name = objectName_;
}
