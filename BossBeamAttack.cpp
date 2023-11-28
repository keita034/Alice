#include "BossBeamAttack.h"

#include<CapsuleShape.h>

void BossBeamAttack::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_,LaserGPUParticle* laserGPUParticle_,Transform* parent_)
{
	bossAnimation = bossAnimation_;
	laserGPUParticle = laserGPUParticle_;
	firePosition.parent = parent_;
	rigidBodyTransform.parent = &firePosition;
	rigidBodyTransform.Initialize();

	firePosition.translation = {0,100*70,-100 * 70 };

	shape.reset(AlicePhysics::CreateCapsuleShape(rigidHalfHeight,rigidRadius));

	{
		AlicePhysics::IRigidBodyCreationSettings lSetting;
		lSetting.name = "BossLaserAttack";
		lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
		lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
		lSetting.allowSleeping = false;
		lSetting.collisionGroup = CollisionGroup::BOSS;
		lSetting.collisionAttribute = CollisionAttribute::BEAM;
		lSetting.position = transform.translation;
		lSetting.userData = static_cast< void* >( &usData );
		lSetting.shape = shape.get();
		lSetting.trigger = true;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
		rigidBody->SetRigidBodyCollision(this);
		physicsSystem = physicsSystem_;
	}

	LaserGPUParticleSetting lSetting;
	lSetting.position = { -10,0,0 };
	lSetting.lifeTime = 100.0f;
	lSetting.velocity = { 10,0,0 };
	lSetting.startColor = { 1,0.2f,0.2f,1 };
	lSetting.endColor = { 1,0.2f,0.2f,1 };
	lSetting.timeBetweenEmit = 0.00005f;
	lSetting.maxSize = 800;
	lSetting.maxSizeTime = 0.15f;
	lSetting.speed = 700;
	lSetting.isPlay = false;

	particleIndex = laserGPUParticle->Emit(lSetting);

	firePosition.MakeWorldMatrix();
}

void BossBeamAttack::Update()
{
	if ( bossAnimation->GetRatio() >= 0.25f )
	{
		if ( !isFire )
		{
			firePosition.MakeWorldMatrix();

			laserGPUParticle->EmitPlay(particleIndex);

			AliceMathF::Vector3 lFirePos = AliceMathF::GetWorldPosition(firePosition);
			laserGPUParticle->SetPos(lFirePos,particleIndex);
			lFirePos.y = 0;

			AliceMathF::Vector3 lBossPos = firePosition.parent->translation;
			lBossPos.y = 0;

			AliceMathF::Vector3 lVelocity;
			lVelocity = lFirePos- lBossPos;
			lVelocity = lVelocity.Normal();
			laserGPUParticle->SetVelocity(lVelocity,particleIndex);
			usData.pos = lBossPos;

			rigidBodyTransform.rotation = { AliceMathF::AX_PI / 2.0f,0.0f,0.0f };
			rigidBodyTransform.translation = { 0.0f,0.0f,-( rigidHalfHeight + rigidRadius * 2 ) * 100 };

		}

		time += laserGPUParticle->GetDeltaTime();

		if (time> 0.15f&& time < 2.2f )
		{
			usData.distance = AliceMathF::Lerp(0.0f,10000.0f,AliceMathF::Clamp01(time*0.5f - 0.15f));

		}

		if ( time >= 2.2f )
		{
			usData.isFinish = true;
		}

		isFire = true;

	}

	if ( isFire )
	{
		Fire();
	}

	if ( !bossAnimation->IsInsert() )
	{
		isFinish = true;
	}

#ifndef _DEBUG
	transform.MakeWorldMatrix();
	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
#endif // _RELEASE
}

bool BossBeamAttack::IsFinish()
{
	return isFinish;
}

void BossBeamAttack::End()
{
	isFinish = false;
	isFire = false;
	usData.isFinish = false;
	time = 0.0f;
	usData.distance = 0.0f;

	rigidBodyTransform.translation = { 0.0f,100000.0f,0.0f };
	rigidBodyTransform.MakeWorldMatrix();
	rigidBody->SetMatrix(rigidBodyTransform.rigidBodyMatWorld,rigidBodyTransform.matWorld);
}

void BossBeamAttack::Fire()
{
	if ( bossAnimation->GetRatio() >= 0.7f )
	{
		isFire = false;
		laserGPUParticle->EmitStop(particleIndex);
	}
}

const AliceMathF::Vector3& BossBeamAttack::GetDirection() const
{
	return direction;
}

void BossBeamAttack::TransUpdate(Camera* camera_)
{
	rigidBodyTransform.TransUpdate(camera_);

	rigidBody->SetMatrix(rigidBodyTransform.rigidBodyMatWorld,rigidBodyTransform.matWorld);
}

void BossBeamAttack::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	laserGPUParticle->EmitStop(particleIndex);
	physicsSystem_->RemoveRigidBody(rigidBody);
	bossAnimation = nullptr;
	laserGPUParticle = nullptr;
}

void BossBeamAttack::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_)
{
}

void BossBeamAttack::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_)
{
}

void BossBeamAttack::OnCollisionExit()
{
}

void BossBeamAttack::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 0.0f ,1.0f ,0.0f ,1.0f },true);
}
