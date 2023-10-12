#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<stdint.h>

ALICE_SUPPRESS_WARNINGS_END

enum class CollisionAttribute : uint8_t
{
	DEFAULT,
	BODY,
	HAND,
	WEAPON,
};

enum class CollisionGroup : uint8_t
{
	DEFAULT,
	PLAYER,
	ENEMY,
	BOSS,
	FIELD,
};