#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include <Jolt/Jolt.h>
#include<Jolt/Physics/Collision/GroupFilter.h>
#include<Jolt/Physics/Collision/CollisionGroup.h>
#include<map>
ALICE_SUPPRESS_WARNINGS_END;

namespace AlicePhysics
{
    class JoltGroupFilter : public JPH::GroupFilter
    {
    private:

        std::map<uint64_t, uint64_t> ignoreCollisions;

    public:

		JoltGroupFilter() = default;
		~JoltGroupFilter() = default;

        /// <summary>
        /// グループ同士が当たるかどうか
        /// </summary>
        /// <param name="group1">グループ1</param>
        /// <param name="group2">グループ2</param>
        virtual bool CanCollide(const JPH::CollisionGroup& group1, const JPH::CollisionGroup& group2) const override;

        /// <summary>
        /// グループ同士の当たり判定を有効化・無効化
        /// </summary>
        /// <param name="groupA_">グループA</param>
        /// <param name="groupB_">グループA</param>
        /// <param name="enable_">trueなら有効falseなら無効</param>
        void EnableCollisionGroups(uint32_t group1_, uint32_t group2_, bool enable_ = true);
    };
}

