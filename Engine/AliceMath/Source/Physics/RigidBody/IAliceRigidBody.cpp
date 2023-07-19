#include "IAliceRigidBody.h"

physx::PxPhysics* IAliceRigidBody::physics = nullptr;
physx::PxScene* IAliceRigidBody::scene = nullptr;
AliceRigidBodyManager* IAliceRigidBody::manager = nullptr;
physx::PxCooking* IAliceRigidBody::cooking = nullptr;

void IAliceRigidBody::CreateRigidBody(RigidBodyType type)
{
	if (type == RigidBodyType::DYNAMIC)
	{
		shape->userData = name.data();
		dynamicBody = PxCreateDynamic(*physics, transform, *shape, density);
		dynamicBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, isGravity);
		scene->addActor(*dynamicBody);
		manager->AddRigidBody(this);

		rigidBodyType = RigidBodyType::DYNAMIC;
	}
	else
	{
		shape->userData = name.data();
		staticBody = PxCreateStatic(*physics, transform, *shape);
		scene->addActor(*staticBody);
		manager->AddRigidBody(this);

		rigidBodyType = RigidBodyType::STATIC;
	}
}

void IAliceRigidBody::CreateShape(float radius_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	shape = physics->createShape(physx::PxSphereGeometry(radius_), *material);

	if (isTrigger)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	//フィルタリング
	physx::PxFilterData lFilterData;
	lFilterData.word0 = filterGroup_;
	lFilterData.word1 = filterMask_;
	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::SPHERE;
}

void IAliceRigidBody::CreateShape(float radius_, float halfHeight_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	shape = physics->createShape(physx::PxCapsuleGeometry(radius_, halfHeight_), *material);

	if (isTrigger)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	//フィルタリング
	physx::PxFilterData lFilterData;
	lFilterData.word0 = filterGroup_;
	lFilterData.word1 = filterMask_;
	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::CAPSULE;
}

void IAliceRigidBody::CreateShape(const std::vector<AliceMathF::Vector3>& points_, const std::vector<uint32_t>& triangles_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	physx::PxTriangleMeshDesc lDesc;

	lDesc.setToDefault(); // 初期化

	lDesc.points.data = &points_[0];
	lDesc.points.count = static_cast<physx::PxU32>(points_.size());
	lDesc.points.stride = sizeof(points_[0]);

	lDesc.triangles.data = &triangles_[0];
	lDesc.triangles.count = static_cast<physx::PxU32>(triangles_.size());
	lDesc.triangles.stride = 3 * sizeof(triangles_[0]);
	lDesc.flags = physx::PxMeshFlags();

	assert(lDesc.isValid());

	physx::PxDefaultMemoryOutputStream lWriteBuffer{};

	assert(cooking->cookTriangleMesh(lDesc, lWriteBuffer));

	physx::PxDefaultMemoryInputData read_buffer(lWriteBuffer.getData(), lWriteBuffer.getSize());
	physx::PxTriangleMesh* lTriangleMesh{ physics->createTriangleMesh(read_buffer) };
	assert(lTriangleMesh);

	physx::PxTriangleMeshGeometry mesh{};

	mesh.triangleMesh = lTriangleMesh;

	shape = physics->createShape(mesh, *material);

	if (isTrigger)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	//フィルタリング
	physx::PxFilterData lFilterData;
	lFilterData.word0 = filterGroup_;
	lFilterData.word1 = filterMask_;
	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::MESH;
}

void IAliceRigidBody::CreateShape(const AliceMathF::Vector3& halfExtent, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	shape = physics->createShape(physx::PxBoxGeometry(halfExtent), *material);

	if (isTrigger)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
	}

	//フィルタリング
	physx::PxFilterData lFilterData;
	lFilterData.word0 = filterGroup_;
	lFilterData.word1 = filterMask_;
	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::SPHERE;
}

void IAliceRigidBody::CreateMaterial(float staticFriction_, float dynamicFriction_, float restitution_)
{
	material = physics->createMaterial(staticFriction_, dynamicFriction_, restitution_);

}

void IAliceRigidBody::SetPhysics(physx::PxPhysics* physics_)
{
	physics = physics_;
}

void IAliceRigidBody::SetScene(physx::PxScene* scene_)
{
	scene = scene_;
}

void IAliceRigidBody::SetManager(AliceRigidBodyManager* manager_)
{
	manager = manager_;
}

void IAliceRigidBody::SetCooking(physx::PxCooking* cooking_)
{
	cooking = cooking_;
}

void IAliceRigidBody::SetPos(const AliceMathF::Vector3 pos_)
{
	transform.p = pos_;
}

void IAliceRigidBody::SetRot(const AliceMathF::Vector3 rot_)
{
	AliceMathF::Quaternion lQuaternion;
	lQuaternion.SeTEuler(rot_);
	transform.q = lQuaternion;
}

void IAliceRigidBody::SetRot(const AliceMathF::Quaternion quaternion_)
{
	transform.q = quaternion_;
}

const std::string& IAliceRigidBody::GetName()const
{
	return name;
}

IAliceRigidBody::~IAliceRigidBody()
{
	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		if (dynamicBody)
		{
			dynamicBody->detachShape(*shape);
			dynamicBody->release();
		}
	}
	else
	{
		if (staticBody)
		{
			staticBody->detachShape(*shape);
			staticBody->release();
		}
	}

	if (material)
	{
		material->release();

	}
}
