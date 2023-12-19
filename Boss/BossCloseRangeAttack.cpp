#include "BossCloseRangeAttack.h"

void BossCloseRangeAttack::Initialize(Transform* bossTrans_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_)
{
	physicsSystem = physicsSystem_;
	bossAnimation = bossAnimation_;
	phase = Phase::NONE;

	bossSword = std::make_unique<BossSword>();
	bossSword->Initialize(bossTrans_,physicsSystem_);

}

void BossCloseRangeAttack::Update()
{
	switch ( phase )
	{
	case BossCloseRangeAttack::Phase::NONE:
		if ( bossAnimation->GetRatio() >= 0.35f )
		{
			bossAnimation->SetAddFrame(0.0f);
			phase = Phase::APPROACH;
		}
		break;
	case BossCloseRangeAttack::Phase::APPROACH:
	{
		float lDistance = AliceMathF::Vector3(playerPos - BossPos).Length();

		if ( distanceFrame >= lDistance )
		{
			phase = Phase::ATTACK;
			distanceTraveled = { 0.0f,0.0f,0.0f };
			bossSword->SetIsUpdate(true);
			bossAnimation->SetAddFrame();
		}
		else
		{
			distanceTraveled = direction * frameDistance;
		}
	}
		break;
	case BossCloseRangeAttack::Phase::ATTACK:

		if ( !bossAnimation->IsInsert() )
		{
			isFinish = true;
			bossSword->SetIsUpdate(false);
		}

		break;
	default:
		break;
	}

}

void BossCloseRangeAttack::Finalize()
{
	physicsSystem = nullptr;
	bossAnimation = nullptr;
}

void BossCloseRangeAttack::End()
{
	isFinish = false;
	phase = Phase::NONE;
}

void BossCloseRangeAttack::SetPlayerPos(const AliceMathF::Vector3& pos_)
{
	playerPos = pos_;
	playerPos.y = 0.0f;

	direction = playerPos - BossPos;

	direction = direction.Normal();
}

void BossCloseRangeAttack::SetBossPos(const AliceMathF::Vector3& pos_)
{
	BossPos = pos_;
	BossPos.y = 0.0f;
}

void BossCloseRangeAttack::SetFrameDistance(float frameDistance_)
{
	frameDistance = frameDistance_;
}

void BossCloseRangeAttack::SetDistanceFrame(float distanceFrame_)
{
	distanceFrame = distanceFrame_;
}

void BossCloseRangeAttack::TransUpdate(Camera* camera_)
{
	bossSword->TransUpdate(camera_);
}

bool BossCloseRangeAttack::IsFinish()
{
	return isFinish;
}

void BossCloseRangeAttack::Draw()
{
	bossSword->Draw();
}

void BossCloseRangeAttack::SwordUpdate(const std::string& boneName_,AliceBlendTree* tree_,AliceModel* bossModel_)
{
	bossSword->Update(boneName_,tree_,bossModel_);
}

const AliceMathF::Vector3& BossCloseRangeAttack::GetDistanceTraveled() const
{
	return distanceTraveled;
}

const AliceMathF::Vector3& BossCloseRangeAttack::GetDirection()const
{
	return direction;
}
