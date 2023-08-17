#pragma once
enum CollisionAttribute
{
	PLAYER = (1 << 0),
	ENEMY = (1 << 1),
	BOSS = (1 << 2),
	FIELD = (1 << 3),
	PLAYERWEAPON = (1 << 4),
	BOSSHAND = (1<<5),
};