#include "Collision2DManager.h"
#include<BoxCollider2D.h>
#include<CapsuleCllider2D.h>
#include<Collision.h>

Collision2DManager* Collision2DManager::GetInstance()
{
	static Collision2DManager lInstance;
	return &lInstance;
}

void Collision2DManager::AddCollider(Base2DCollider* collider_)
{
	colliders.push_front(collider_);
}

void Collision2DManager::RemoveCollider(Base2DCollider* collider_)
{
	colliders.remove(collider_);
}

void Collision2DManager::ChangeCollider(uint32_t colliderIndex_, Base2DCollider* collider_)
{

	std::forward_list<Base2DCollider*>::iterator literator = colliders.begin();

	for (uint32_t i = 0; i < colliderIndex_ - 1; i++)
	{
		literator++;
	}

	colliders.emplace_after(literator);
	colliders.insert_after(literator, collider_);

}

void Collision2DManager::CheckAllCollisions()
{
	std::forward_list<Base2DCollider*>::iterator literatorA;
	std::forward_list<Base2DCollider*>::iterator literatorB;


	// 全ての組み合わせについて総当りチェック
	
	for (literatorA = colliders.begin(); literatorA != colliders.end(); literatorA++)
	{
		if (!literatorA._Ptr->_Myval->IsValid())
		{
			continue;
		}

		literatorB = literatorA;
		++literatorB;
		for (; literatorB != colliders.end(); ++literatorB)
		{

			if (!literatorB._Ptr->_Myval->IsValid())
			{
				continue;
			}

			Base2DCollider* lColliderA = *literatorA;
			Base2DCollider* lColliderB = *literatorB;

			// カプセルと矩形
			if (lColliderA->GetShapeType() == COLLISIONSHAPE_BOX && lColliderB->GetShapeType() == COLLISIONSHAPE_CAPSULE)
			{
				BoxCollider2D* lBoxCollider = dynamic_cast<BoxCollider2D*>(lColliderA);
				CapsuleCllider2D* lCapsuleCllider = dynamic_cast<CapsuleCllider2D*>(lColliderB);

				if (Collision::SCheckBox2Capsule(*lBoxCollider, *lCapsuleCllider))
				{
					lBoxCollider->SetOpponentCollsionName(lCapsuleCllider->GetCollsionName());
					lBoxCollider->SetOpponentCollider(lCapsuleCllider);
					lBoxCollider->OnCollision();
					lCapsuleCllider->SetOpponentCollsionName(lBoxCollider->GetCollsionName());
					lCapsuleCllider->SetOpponentCollider(lBoxCollider);
					lCapsuleCllider->OnCollision();
				}
			}
		}
	}
}
