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

	std::forward_list<Base2DCollider*>::iterator lIterator = colliders.begin();

	for (uint32_t i = 0; i < colliderIndex_ - 1; i++)
	{
		lIterator++;
	}

	colliders.emplace_after(lIterator);
	colliders.insert_after(lIterator, collider_);

}

void Collision2DManager::CheckAllCollisions()
{
	std::forward_list<Base2DCollider*>::iterator lIteratorA;
	std::forward_list<Base2DCollider*>::iterator lIteratorB;


	// 全ての組み合わせについて総当りチェック
	
	for (lIteratorA = colliders.begin(); lIteratorA != colliders.end(); lIteratorA++)
	{
		if (!lIteratorA._Ptr->_Myval->IsValid())
		{
			continue;
		}

		lIteratorB = lIteratorA;
		++lIteratorB;
		for (; lIteratorB != colliders.end(); ++lIteratorB)
		{

			if (!lIteratorB._Ptr->_Myval->IsValid())
			{
				continue;
			}

			Base2DCollider* lColliderA = *lIteratorA;
			Base2DCollider* lColliderB = *lIteratorB;

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
