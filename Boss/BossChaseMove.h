#pragma once
#include<AliceMathF.h>

class BossChaseMove
{
private:

	AliceMathF::Vector3 playerPos;

	AliceMathF::Vector3 BossPos;

	AliceMathF::Vector3 direction;

	AliceMathF::Vector3 distanceTraveled;

	float maxDistance = 0.0f;

	float speed = 0.0f;

	bool isFinish = false;

public:

	BossChaseMove() = default;
	 ~BossChaseMove() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	 void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	 void Update();

	/// <summary>
	/// 後始末
	/// </summary>
	 void Finalize();

	 void SetPlayerPos(const AliceMathF::Vector3& pos_);
	 void SetBossPos(const AliceMathF::Vector3& pos_);
	 void SetMaxDistance(float distance_);
	 void SetSpeed(float speed_);

	 bool IsFinish();

	 const AliceMathF::Vector3& GetDistanceTraveled()const;

	 const AliceMathF::Vector3& GetDirection()const;
};