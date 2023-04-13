#include "Game2DObject.h"

void Game2DObject::SetCollider(Base2DCollider* coll)
{
	SetColl(coll);

	collider->SetObject(this);
}

void Game2DObject::OnCollision()
{
}

void Game2DObject::CollisionUpdate()
{

}