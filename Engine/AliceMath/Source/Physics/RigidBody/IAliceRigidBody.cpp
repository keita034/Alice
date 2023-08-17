#include "IAliceRigidBody.h"

physx::PxPhysics* IAliceRigidBody::physics = nullptr;
physx::PxScene* IAliceRigidBody::scene = nullptr;
AliceRigidBodyManager* IAliceRigidBody::manager = nullptr;
physx::PxCooking* IAliceRigidBody::cooking = nullptr;

std::string GuidToString(const GUID& guid)
{
	char guid_cstr[37];
	snprintf(guid_cstr, sizeof(guid_cstr),
		"%lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return std::string(guid_cstr);
}

void IAliceRigidBody::CreateRigidBody(RigidBodyType type)
{
	if (type == RigidBodyType::DYNAMIC)
	{
		GUID guid;
		CoCreateGuid(&guid);
		userData.id = GuidToString(guid);
		shape->userData = &userData;
		userData.attribute = filterGroup << 16;

		dynamicBody = PxCreateDynamic(*physics, pxTransform, *shape, density);
		dynamicBody->setMass(1.0f);
		dynamicBody->userData = &userData;
		//dynamicBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, isGravity);
		scene->addActor(*dynamicBody);
		manager->AddRigidBody(this);

		rigidBodyType = RigidBodyType::DYNAMIC;
	}
	else
	{
		GUID guid;
		CoCreateGuid(&guid);
		userData.id = GuidToString(guid);
		shape->userData = &userData;
		userData.attribute = filterGroup << 16;

		staticBody = PxCreateStatic(*physics, pxTransform, *shape);
		staticBody->userData = &userData;

		scene->addActor(*staticBody);
		manager->AddRigidBody(this);

		rigidBodyType = RigidBodyType::STATIC;
	}
}

void IAliceRigidBody::CreateShape(float radius_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	shape = physics->createShape(physx::PxSphereGeometry(radius_), *pxMaterial);

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
	filterGroup = filterGroup_;

	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::SPHERE;
}

void IAliceRigidBody::CreateShape(float radius_, float halfHeight_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	shape = physics->createShape(physx::PxCapsuleGeometry(radius_, halfHeight_), *pxMaterial);

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
	filterGroup = filterGroup_;

	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::CAPSULE;
}

void IAliceRigidBody::CreateShape(const std::vector<AliceMathF::Vector3>& points_, const std::vector<uint32_t>& triangles_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	physx::PxTriangleMeshDesc lDesc{};

	lDesc.setToDefault(); // 初期化

	lDesc.points.data = points_.data();
	lDesc.points.count = static_cast<physx::PxU32>(points_.size());
	lDesc.points.stride = sizeof(points_[0]);

	lDesc.triangles.data = triangles_.data();
	lDesc.triangles.count = static_cast<physx::PxU32>(triangles_.size());
	lDesc.triangles.stride = 3 * sizeof(triangles_[0]);
	lDesc.flags = physx::PxMeshFlags();

	assert(lDesc.isValid());

	physx::PxTriangleMeshCookingResult::Enum condition;
	physx::PxDefaultMemoryOutputStream lWriteBuffer{};
	bool status{ cooking->cookTriangleMesh(lDesc, lWriteBuffer,&condition) };
	assert(status);
	status = false;

	static_cast<void>(condition);

	physx::PxDefaultMemoryInputData read_buffer(lWriteBuffer.getData(), lWriteBuffer.getSize());
	physx::PxTriangleMesh* lTriangleMesh{ physics->createTriangleMesh(read_buffer) };
	assert(lTriangleMesh);

	physx::PxTriangleMeshGeometry mesh{};

	mesh.triangleMesh = lTriangleMesh;

	shape = physics->createShape(mesh, *pxMaterial);

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
	filterGroup = filterGroup_;

	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::MESH;
}

void IAliceRigidBody::CreateShape(const AliceMathF::Vector3& halfExtent, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_, bool gravity_)
{
	isTrigger = trigger_;
	isGravity = gravity_;

	shape = physics->createShape(physx::PxBoxGeometry(halfExtent), *pxMaterial);

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
	filterGroup = filterGroup_;

	shape->setSimulationFilterData(lFilterData);

	collisionShape = CollisionShape::SPHERE;
}

void IAliceRigidBody::CreateMaterial(float staticFriction_, float dynamicFriction_, float restitution_)
{
	pxMaterial = physics->createMaterial(staticFriction_, dynamicFriction_, restitution_);

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

void IAliceRigidBody::SetInitializePos(const AliceMathF::Vector3& pos_)
{
	pxTransform.p = pos_;
}

void IAliceRigidBody::SetInitializeRot(const AliceMathF::Vector3& rot_, const AliceMathF::Quaternion* quaternion_)
{
	AliceMathF::Quaternion lQuaternion;
	if (quaternion_)
	{
		AliceMathF::Quaternion lQuaternion2 = *quaternion_;
		lQuaternion.SeTEuler(rot_);
		lQuaternion2 *= lQuaternion;
		pxTransform.q = lQuaternion2;
	}
	else
	{
		lQuaternion.SeTEuler(rot_);
		pxTransform.q = lQuaternion;
	}

}

void IAliceRigidBody::SetInitializeRot(const AliceMathF::Quaternion& quaternion_)
{
	pxTransform.q = quaternion_;
}

void IAliceRigidBody::AddForce(const AliceMathF::Vector3& force_, ForceMode mode_)
{
	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		dynamicBody->addForce(force_, static_cast<physx::PxForceMode::Enum>(mode_));
	}
}

void IAliceRigidBody::SetPos(const AliceMathF::Vector3& pos_)
{
	pxTransform.p = pos_;

	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		dynamicBody->setGlobalPose(pxTransform);
	}
	else
	{
		staticBody->setGlobalPose(pxTransform);
	}
}

void IAliceRigidBody::SetRot(const AliceMathF::Vector3& rot_)
{
	AliceMathF::Quaternion lQuaternion;
	lQuaternion.SeTEuler(rot_);
	pxTransform.q = lQuaternion;

	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		dynamicBody->setGlobalPose(pxTransform);
	}
	else
	{
		staticBody->setGlobalPose(pxTransform);
	}
}

void IAliceRigidBody::SetRot(const AliceMathF::Quaternion& quaternion_)
{
	pxTransform.q = quaternion_;

	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		dynamicBody->setGlobalPose(pxTransform);
	}
	else
	{
		staticBody->setGlobalPose(pxTransform);
	}
}

void IAliceRigidBody::SetMat(const AliceMathF::Matrix4& mat_)
{
	pxTransform = physx::PxTransform(mat_);

	//pxTransform.q = { -0.6964f,0.0457f,0.6800f,0.2242f};
	//pxTransform.p = { -10.9301f,59.8267f,3.2187f };

	if (pxTransform.isFinite())
	{
 		int b =0;
		b++;
	}

	if (pxTransform.isValid())
	{
		int b = 0;
		b++;
	}

	if (pxTransform.isSane())
	{
		int b = 0;
		b++;
	}

	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		dynamicBody->setGlobalPose(pxTransform);
	}
	else
	{
		staticBody->setGlobalPose(pxTransform);
	}
}

const std::string& IAliceRigidBody::GetName()const
{
	return userData.id;
}

const AliceMathF::Vector3& IAliceRigidBody::GetGlobalPos()
{
	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		globalPos = dynamicBody->getGlobalPose().p;
		return globalPos;
	}
	else
	{
		globalPos = staticBody->getGlobalPose().p;
		return globalPos;
	}
}

RigidBodyUserData* IAliceRigidBody::GetUserData()
{
	return &userData;
}

IAliceRigidBody::~IAliceRigidBody()
{
	if (rigidBodyType == RigidBodyType::DYNAMIC)
	{
		if (dynamicBody)
		{
			if (shape)
			{
				dynamicBody->detachShape(*shape);
			}

			dynamicBody->release();
		}
	}
	else
	{
		if (staticBody)
		{
			if (shape)
			{
				staticBody->detachShape(*shape);
			}

			staticBody->release();
		}
	}

	if (pxMaterial)
	{
		pxMaterial->release();

	}

	manager->RemoveRigidBody(this);
}
