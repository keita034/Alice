#pragma once
#include<stdint.h>

namespace AlicePhysics
{
	enum PhysicsRigidBodyType : uint8_t
	{
		STATIC,
		DYNAMIC,
		QUERY,
		TRIGGER,
		CHARACTER,
		RAGDOLL,

		PHYSICSRIGIDBODYTYPE_COUNT
	};
}