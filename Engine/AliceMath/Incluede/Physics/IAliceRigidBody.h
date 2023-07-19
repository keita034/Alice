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

#include<PxPhysicsAPI.h>
#include<memory>
#include<string>
#include<unordered_map>
#pragma warning(pop)

#include<AliceMathUtility.h>
#include<AliceRigidBodyManager.h>

enum class CollisionShape
{
	BOX,
	SPHERE,
	CAPSULE,
	MESH
};

enum class RigidBodyType
{
	DYNAMIC,
	STATIC
};

class IAliceRigidBody
{
protected:

	static physx::PxPhysics* physics;
	static physx::PxScene* scene;
	static AliceRigidBodyManager* manager;
	static physx::PxCooking* cooking;

	physx::PxRigidDynamic* dynamicBody;

	physx::PxRigidStatic* staticBody;

	physx::PxMaterial* material;

	physx::PxShape* shape;

	std::vector<AliceMathF::Vector3>* points;
	std::vector<uint32_t>* triangles;

	std::string name;

	physx::PxTransform transform;

	uint32_t filterGroup;
	uint32_t filterMask;

	float density = 10.0f;

	CollisionShape collisionShape;
	RigidBodyType rigidBodyType;

	bool isTrigger = false;
	bool isGravity = true;

	int8_t PADING[6];

public:

	void CreateRigidBody(RigidBodyType type);

	void CreateShape(float radius_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_ = false, bool gravity_ = true);
	void CreateShape(float radius_, float halfHeight_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_ = false, bool gravity_ = true);
	void CreateShape(const std::vector<AliceMathF::Vector3>& points_, const std::vector<uint32_t>& triangles_, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_ = false, bool gravity_ = true);
	void CreateShape(const AliceMathF::Vector3& halfExtent, uint32_t filterGroup_, uint32_t filterMask_, bool trigger_ = false, bool gravity_ = true);

	void CreateMaterial(float staticFriction_ = 0.5f, float dynamicFriction_ = 0.5f, float restitution_ = 0.6f);

	static void SetPhysics(physx::PxPhysics* physics_);
	static void SetScene(physx::PxScene* scene_);
	static void SetManager(AliceRigidBodyManager* manager_);
	static void SetCooking(physx::PxCooking* cooking_);

	void SetPos(const AliceMathF::Vector3 pos_);
	void SetRot(const AliceMathF::Vector3 rot_);
	void SetRot(const AliceMathF::Quaternion quaternion_);

	/// <summary>
	/// 名前取得
	/// </summary>
	/// <returns></returns>
	const std::string& GetName()const;

	//すり抜ける当たり判定
	virtual void OnTrigger() = 0;

	//すり抜けない当たり判定
	virtual void OnContact() = 0;

	IAliceRigidBody() = default;
	virtual ~IAliceRigidBody();


};

