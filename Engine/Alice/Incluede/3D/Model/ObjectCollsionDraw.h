#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4711)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5220)

#include<vector>
#include<PxPhysicsAPI.h>
#pragma warning(pop)

#include<Transform.h>
#include<AliceModel.h>

#include<IAliceRigidBody.h>

class ObjectCollsionDraw
{
private:

public:

	void DebugDraw(CollisionShape shape_,physx::PxShape* pxShape_,int32_t& index_,const physx::PxTransform& transform_,Camera* camera_);

	ObjectCollsionDraw();
	~ObjectCollsionDraw();

private:

	//コピーコンストラクタ・代入演算子削除
	ObjectCollsionDraw& operator=(const ObjectCollsionDraw&) = delete;
	ObjectCollsionDraw(const ObjectCollsionDraw&) = delete;
};