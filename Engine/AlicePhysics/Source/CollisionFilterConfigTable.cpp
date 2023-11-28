#include "CollisionFilterConfigTable.h"
#include<Bit.h>

CollisionFilterConfigTable::CollisionFilterConfigTable()
{
	for ( GroupMask& groupMask : groupMasks )
	{
		groupMask.mask = 0xffff;
	}
}

CollisionFilterConfigTable::~CollisionFilterConfigTable()
{
}

void CollisionFilterConfigTable::SetGroupName(uint32_t group_,const char* name_)
{
	groupMasks[ group_ ].name = name_;
}

const char* CollisionFilterConfigTable::GetGroupName(uint32_t group_) const
{
	return groupMasks[ group_ ].name.c_str();
}

void CollisionFilterConfigTable::EnableCollision(uint32_t groupA_,uint32_t groupB_,bool enable_)
{
	if ( enable_ )
	{
		//有効
		groupMasks[ groupA_ ].mask |= ALICE_BIT(groupB_);
		groupMasks[ groupB_ ].mask |= ALICE_BIT(groupA_);
	}
	else
	{
		//無効
		groupMasks[ groupA_ ].mask &= ~ALICE_BIT(groupB_);
		groupMasks[ groupB_ ].mask &= ~ALICE_BIT(groupA_);
	}
}

bool CollisionFilterConfigTable::IsCollisionEnabled(uint32_t groupA_,uint32_t groupB_) const
{
	return ( groupMasks[ groupA_ ].mask & ALICE_BIT(groupB_) );
}

uint32_t CollisionFilterConfigTable::GetFilterMask(uint32_t group_) const
{
	return groupMasks[ group_ ].mask;
}

uint32_t CollisionFilterConfigTable::GetNumNamedGroups() const
{
	uint32_t count = 0;

	for ( size_t i = 0; i < 32; ++i )
	{
		if ( !groupMasks[ i ].name.empty() )
		{
			++count;
		}
	}

	return count;
}

uint32_t CollisionFilterConfigTable::GetFilterGroupByName(const std::string& name_) const
{
	for ( uint32_t i = 0; i < 32; ++i )
	{
		if ( groupMasks[ i ].name == name_ )
		{
			return i;
		}
	}

	return static_cast< uint32_t >( -1 );
}

uint32_t CollisionFilterConfigTable::FindUnnamedGroup() const
{
	for ( uint32_t i = 0; i < 32; ++i )
	{
		if ( groupMasks[ i ].name.empty() )
		{
			return i;
		}
	}

	return static_cast< uint32_t >( -1 );
}
