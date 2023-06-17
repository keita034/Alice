#include<Game2DObject.h>

void Game2DObject::SetCollider(Base2DCollider* coll_)
{
	SetColl(coll_);

	collider->SetObject(this);
}

void Game2DObject::OnCollision()
{
}

void Game2DObject::CollisionUpdate()
{

}