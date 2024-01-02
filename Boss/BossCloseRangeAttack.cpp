#include "BossCloseRangeAttack.h"

void BossCloseRangeAttack::Initialize(GPUParticleEmitter* particleEmitter_,Transform* bossTrans_,AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_)
{
	physicsSystem = physicsSystem_;
	bossAnimation = bossAnimation_;
	phase = Phase::NONE;

	bossSword = std::make_unique<BossSword>();
	bossSword->Initialize(bossTrans_,physicsSystem_);

	meshParticle = particleEmitter_->GetMeshGPUParticle("BossWeaponParticle");

	MeshGPUParticleSetting lSetting;

	lSetting.matWorld = AliceMathF::MakeIdentity();
	lSetting.velocity = { 0,1,0 };
	lSetting.startColor = { 1,1,1,1 };
	lSetting.endColor = { 1,1,1,1 };
	lSetting.lifeTime = 2.0f;
	lSetting.maxParticles = 100000.0f;
	lSetting.timeBetweenEmit = 0.001f;
	lSetting.emitLifeTime = -1;
	lSetting.size = 2;
	lSetting.speed = 15;
	lSetting.isPlay = false;

	particleIndex = meshParticle->Emit(lSetting);
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
			bossSword->SetIsUpdate(true);
			meshParticle->EmitPlay(particleIndex);
		}
		break;
	case BossCloseRangeAttack::Phase::APPROACH:
	{
		float lDistance = AliceMathF::Vector3(playerPos - BossPos).Length();

		if ( distanceFrame >= lDistance )
		{
			phase = Phase::ATTACK;
			distanceTraveled = { 0.0f,0.0f,0.0f };
	
			bossAnimation->SetAddFrame(0.015f);
		}
		else
		{
			distanceTraveled = direction * frameDistance;
		}
	}
		break;
	case BossCloseRangeAttack::Phase::ATTACK:

		if ( bossAnimation->GetRatio() >= 0.6f )
		{
			bossSword->SetIsUpdate(false);
			meshParticle->EmitStop(particleIndex);
		}
		if ( !bossAnimation->IsInsert() )
		{
			isFinish = true;
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
	meshParticle->EmitStop(particleIndex);
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
	meshParticle->SetMat(bossSword->GetWorldMat(),particleIndex);
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
