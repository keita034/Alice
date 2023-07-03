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
	transform.TransUpdate(camera_);
}
