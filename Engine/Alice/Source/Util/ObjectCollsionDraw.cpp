#include "ObjectCollsionDraw.h"

void ObjectCollsionDraw::DebugDraw(CollisionShape shape_,physx::PxShape* pxShape_,int32_t& index_,const physx::PxTransform& transform_,Camera* camera_)
{
	static_cast< void >( shape_ );
	static_cast< void >( pxShape_ );
	static_cast< void >( index_ );
	static_cast< void >( transform_ );
	static_cast< void >( camera_ );

#ifdef _DEBUG

	if ( index_ < 0 )
	{
		index_ = static_cast<int32_t>(transforms.size());

		std::unique_ptr<Transform> transform = std::make_unique<Transform>();
		transform->Initialize();
		transforms.push_back(std::move(transform));
	}

	std::unique_ptr<Transform>& transform = transforms[ static_cast< size_t >(index_)];

	transform->translation = transform_.p;

	switch ( shape_ )
	{
	case CollisionShape::BOX:
		break;
	case CollisionShape::SPHERE:
		break;
	case CollisionShape::CAPSULE:

	{
		physx::PxCapsuleGeometry capsule;
		pxShape_->getCapsuleGeometry(capsule);

		transform->scale.x =  capsule.radius/ 1.0f;
		transform->scale.y =  capsule.halfHeight/ 2.0f;
		transform->scale.z = capsule.radius / 1.0f;

		if ( !capsuleModel.model )
		{
			capsuleModel.hanlde = AliceModel::SCreateModel("Resources/Default/CollisionShape");
			capsuleModel.model = std::make_unique<AliceModel>();
			capsuleModel.model->SetModel(capsuleModel.hanlde);
		}

		transform->TransUpdate(camera_);
		capsuleModel.model->Draw(*transform);
	}

		break;
	case CollisionShape::MESH:
		break;
	}

#endif // _DEBUG

}

ObjectCollsionDraw::ObjectCollsionDraw()
{

}

ObjectCollsionDraw::~ObjectCollsionDraw()
{
	transforms.clear();
}