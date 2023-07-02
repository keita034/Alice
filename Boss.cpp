#include "Boss.h"

#include<OBBCollider.h>
#include<GameCollisionConflg.h>

void Boss::Initialize()
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Player");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	transform.translation = { 0.0f,0.0f,10.0f };

	transform.Initialize();


	{
		OBBCollider* lObb = new OBBCollider();

		const std::vector<PosNormUvTangeColSkin>& ver = model->GetMeshs()[0]->GetVertices();

		lObb->CreateOBB(ver, transform.translation, transform.rotation, transform.scale);

		lObb->SetCollsionName("Boss");
		//衝突属性を設定
		lObb->SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
		//衝突対象を自分以外に設定
		lObb->SetCollisionMask(~COLLISION_ATTRIBUTE_ENEMY);


		SetCollider(lObb);
	}

}

void Boss::Update()
{

}

void Boss::Draw()
{
	model->Draw(transform);
}

void Boss::Finalize()
{
}

void Boss::TransUpdate(Camera* camera_)
{
	{
		OBBCollider* lObb = dynamic_cast<OBBCollider*>(collider.get());
		lObb->UpdateOBB(transform.matWorld, transform.rotation);
	}

	transform.TransUpdate(camera_);
}
