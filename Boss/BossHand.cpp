#include "BossHand.h"

#include<CollisionAttribute.h>

void BossHand::Initialize(Transform* parent_)
{
	transform.parent = parent_;
	transform.Initialize();

	CreateMaterial(0.6f, 0.6f, 0.0f);
	CreateShape(4.0f, CollisionAttribute::BOSSHAND, CollisionAttribute::PLAYER , true);
	SetInitializePos(transform.translation);
	SetInitializeRot(transform.rotation);
	CreateRigidBody(RigidBodyType::DYNAMIC);
}

void BossHand::Update(const std::string& boneName_, AliceBlendTree* tree_, AliceModel* playerModel_)
{
	animationTransform = playerModel_->GetAnimationTransform(tree_, boneName_);
}

void BossHand::Finalize()
{
}

void BossHand::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_, &animationTransform.boneMatrix);

	SetMat(transform.rigidBodyMatWorld);
	SetPos(AliceMathF::GetWorldPosition(transform.matWorld));
}

void BossHand::OnTrigger(uint32_t attribute_)
{

}

void BossHand::SetSituation(uint32_t situation_)
{
	uint32_t lBitTmp = userData.attribute & 0xfff;
	userData.attribute &= ~lBitTmp;
	userData.attribute |= situation_;
}
