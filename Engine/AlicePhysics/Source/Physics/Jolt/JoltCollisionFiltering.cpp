#include "JoltCollisionFiltering.h"

#include<Bit.h>
#include<AliceAssert.h>

uint32_t AlicePhysics::GetBroadphaseCollisionMask(PhysicsRigidBodyType broadphase_)
{
    switch (broadphase_)
    {
    case AlicePhysics::PhysicsRigidBodyType::STATIC:
        return ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::DYNAMIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::CHARACTER)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::RAGDOLL));

    case AlicePhysics::PhysicsRigidBodyType::DYNAMIC:
        return ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::STATIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::DYNAMIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::TRIGGER)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::CHARACTER)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::RAGDOLL)) ;

    case AlicePhysics::PhysicsRigidBodyType::QUERY:
        // クエリ形状は、シミュレーションの中で何かと相互作用することはない
        return 0;

    case AlicePhysics::PhysicsRigidBodyType::TRIGGER:
        // トリガーはパフォーマンス上の理由からラグドールなどの詳細オブジェクトを除外します。
        return ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::DYNAMIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::CHARACTER));

    case AlicePhysics::PhysicsRigidBodyType::CHARACTER:
        return ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::STATIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::DYNAMIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::TRIGGER)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::CHARACTER))|
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::RAGDOLL));

    case AlicePhysics::PhysicsRigidBodyType::RAGDOLL:
        return ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::STATIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::DYNAMIC)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::CHARACTER)) |
               ALICE_BIT(static_cast<uint32_t>(PhysicsRigidBodyType::RAGDOLL));

    case AlicePhysics::PhysicsRigidBodyType::PHYSICSRIGIDBODYTYPE_COUNT:
        ALICE_ASSERT_NOT_IMPLEMENTED;
        break;

        ALICE_DEFAULT_CASE_NOT_IMPLEMENTED;
    }

    return 0;
}

uint16_t AlicePhysics::ConstructObjectLayer(PhysicsRigidBodyType broadphase_, uint8_t collisionGroup_)
{
    return static_cast<uint16_t>(static_cast<uint16_t>(broadphase_) << 8 | static_cast<uint16_t>(collisionGroup_));
}

uint32_t AlicePhysics::JoltObjectToBroadphaseLayer::GetNumBroadPhaseLayers() const
{
    return static_cast<uint32_t>(PhysicsRigidBodyType::PHYSICSRIGIDBODYTYPE_COUNT);
}

JPH::BroadPhaseLayer AlicePhysics::JoltObjectToBroadphaseLayer::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
{
    //uint16からuint8にビット演算で変換
	return JPH::BroadPhaseLayer(static_cast< JPH::BroadPhaseLayer::Type >( inLayer >> 8 ));
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)

const char* AlicePhysics::JoltObjectToBroadphaseLayer::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
{
		switch ( inLayer.operator JPH::BroadPhaseLayer::Type() )
		{
		case AlicePhysics::PhysicsRigidBodyType::STATIC:
			return "STATIC";

		case AlicePhysics::PhysicsRigidBodyType::DYNAMIC:
			return "DYNAMIC";

		case AlicePhysics::PhysicsRigidBodyType::QUERY:
			return "QUERY";

		case AlicePhysics::PhysicsRigidBodyType::TRIGGER:
			return "TRIGGER";

		case AlicePhysics::PhysicsRigidBodyType::CHARACTER:
			return "CHARACTER";

		case AlicePhysics::PhysicsRigidBodyType::RAGDOLL:
			return "RAGDOLL";

		case AlicePhysics::PhysicsRigidBodyType::PHYSICSRIGIDBODYTYPE_COUNT:
			ALICE_ASSERT_NOT_IMPLEMENTED;
			break;

			ALICE_DEFAULT_CASE_NOT_IMPLEMENTED;
		}

	return "nullptr";
}

#endif

bool AlicePhysics::JoltObjectVsBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
{
    const uint32_t mask1 = ALICE_BIT(inLayer1 >> 8);
    const uint32_t mask2 = GetBroadphaseCollisionMask(static_cast<PhysicsRigidBodyType>(static_cast<uint8_t>(inLayer2)));

    return (mask1 & mask2) != 0;
}

bool AlicePhysics::JoltObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer inLayer1,JPH::ObjectLayer inLayer2) const
{
	return collisionFilterConfigTable->IsCollisionEnabled(static_cast< uint32_t >( inLayer1 ) & 0xFF,static_cast< uint32_t >( inLayer2 ) & 0xFF);
}

void AlicePhysics::JoltObjectLayerPairFilter::SetCollisionFilterConfigTable(CollisionFilterConfigTable* collisionFilterConfigTable_)
{
	collisionFilterConfigTable = collisionFilterConfigTable_;
}