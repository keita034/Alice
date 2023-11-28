#include "JoltGroupFilter.h"

bool AlicePhysics::JoltGroupFilter::CanCollide(const JPH::CollisionGroup& group1, const JPH::CollisionGroup& group2) const
{
	uint64_t lId = static_cast<uint64_t>(group1.GetGroupID()) << 32 | group2.GetGroupID();

	return !static_cast<bool>(ignoreCollisions.count(lId));
}

void AlicePhysics::JoltGroupFilter::EnableCollisionGroups(uint32_t group1_, uint32_t group2_, bool enable_)
{
    const uint64_t uiMask1 = static_cast<uint64_t>(group1_) << 32 | group2_;
    const uint64_t uiMask2 = static_cast<uint64_t>(group2_) << 32 | group1_;

    if (enable_)
    {
		ignoreCollisions[ uiMask1 ] = uiMask1;
       ignoreCollisions[ uiMask2 ] = uiMask2;
    }
    else
    {
        ignoreCollisions.erase(uiMask1);
        ignoreCollisions.erase(uiMask2);
    }
}
