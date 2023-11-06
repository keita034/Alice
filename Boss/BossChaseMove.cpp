#include "BossChaseMove.h"

void BossChaseMove::Initialize()
{
}

void BossChaseMove::Update()
{
	float lDistance = AliceMathF::Vector3(playerPos - BossPos).Length();

	if (maxDistance >= lDistance)
	{
		isFinish = true;
		distanceTraveled = { 0.0f,0.0f,0.0f };
		return;
	}
	else
	{
		distanceTraveled = direction * speed;
	}
}

void BossChaseMove::Finalize()
{
	distanceTraveled = { 0.0f,0.0f,0.0f };
	isFinish = false;
}

void BossChaseMove::SetPlayerPos(const AliceMathF::Vector3& pos_)
{
	playerPos = pos_;
	playerPos.y = 0.0f;

	direction =  playerPos- BossPos;

	direction =  direction.Normal();
}

void BossChaseMove::SetBossPos(const AliceMathF::Vector3& pos_)
{
	BossPos = pos_;
	BossPos.y = 0.0f;
}

void BossChaseMove::SetMaxDistance(float distance_)
{
	maxDistance = distance_;
}

void BossChaseMove::SetSpeed(float speed_)
{
	speed = speed_;
}

bool BossChaseMove::IsFinish()
{
	return isFinish;
}

const AliceMathF::Vector3& BossChaseMove::GetDistanceTraveled()const
{
	return distanceTraveled;
}
