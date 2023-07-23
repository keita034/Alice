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

void GameObject::OnTrigger(uint32_t attribute_)
{
	static_cast< void >( attribute_ );
}

void GameObject::OnContact(uint32_t attribute_)
{
	static_cast< void >( attribute_ );
}

void GameObject::SetObjectCollsionDraw(ObjectCollsionDraw* objectCollsionDraw_)
{
	objectCollsionDraw = objectCollsionDraw_;
}
