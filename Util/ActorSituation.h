#pragma once

enum ActorSituation
{
	ATTACK = ( 1 << 0 ),
	ROWLING = ( 1 << 1 ),
	DAMAGE = ( 1 << 2 ),
	WALKING = ( 1 << 3 ),
	HEALING = ( 1 << 4 ),
	SHOCKWAVE_DAMAGE = ( 1 << 5 ),
	BEAM_DAMAGE = ( 1 << 6 ),
};