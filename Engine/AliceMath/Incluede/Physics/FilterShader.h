#pragma once

#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)
#pragma warning(disable: 5220)

#include<PxPhysicsAPI.h>

#pragma warning(pop)

physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
