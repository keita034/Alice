#include "Collision2DManager.h"
#include<BoxCollider2D.h>
#include<CapsuleCllider2D.h>
#include<Collision.h>

Collision2DManager* Collision2DManager::GetInstance()
{
	static Collision2DManager instance;
	return &instance;
}

void Collision2DManager::AddCollider(Base2DCollider* collider)
{
	colliders.push_front(collider);
}

void Collision2DManager::RemoveCollider(Base2DCollider* collide)
{
	colliders.remove(collide);
}

void Collision2DManager::ChangeCollider(UINT colliderIndex, Base2DCollider* collider)
{

	std::forward_list<Base2DCollider*>::iterator itr = colliders.begin();

	for (UINT i = 0; i < colliderIndex - 1; i++)
	{
		itr++;
	}

	colliders.emplace_after(itr);
	colliders.insert_after(itr, collider);

}

void Collision2DManager::CheckAllCollisions()
{
	std::forward_list<Base2DCollider*>::iterator itA;
	std::forward_list<Base2DCollider*>::iterator itB;


	// 全ての組み合わせについて総当りチェック
	
	for (itA = colliders.begin(); itA != colliders.end(); itA++)
	{
		if (!itA._Ptr->_Myval->IsValid())
		{
			continue;
		}

		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB)
		{

			if (!itB._Ptr->_Myval->IsValid())
			{
				continue;
			}

			Base2DCollider* colA = *itA;
			Base2DCollider* colB = *itB;

			// カプセルと矩形
			if (colA->GetShapeType() == COLLISIONSHAPE_BOX &&
				colB->GetShapeType() == COLLISIONSHAPE_CAPSULE)
			{
				BoxCollider2D* box = dynamic_cast<BoxCollider2D*>(colA);
				CapsuleCllider2D* capsule = dynamic_cast<CapsuleCllider2D*>(colB);

				if (Collision::CheckBox2Capsule(*box, *capsule))
				{
					box->SetOpponentCollsionName(capsule->GetCollsionName());
					box->SetOpponentCollider(capsule);
					box->OnCollision();
					capsule->SetOpponentCollsionName(box->GetCollsionName());
					capsule->SetOpponentCollider(box);
					capsule->OnCollision();
				}
			}
		}
	}
}
