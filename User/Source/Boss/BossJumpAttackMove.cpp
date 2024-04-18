#include "BossJumpAttackMove.h"
#include<Easing.h>
#include<SphereShape.h>
#include<Player.h>

void BossJumpAttackMove::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_,BossAnimation* bossAnimation_,ShockWaveGPUParticle* shockWaveGPUParticle_)
{
	bossAnimation = bossAnimation_;

	transform.translation = { 0.0f,1000.0f,0.0f };
	transform.Initialize();

	shape.reset(AlicePhysics::CreateSphereShape(250.0f));

	{
		AlicePhysics::IRigidBodyCreationSettings lSetting;
		lSetting.name = "BossShockWave";
		lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
		lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
		lSetting.allowSleeping = false;
		lSetting.collisionGroup = CollisionGroup::BOSS;
		lSetting.collisionAttribute = CollisionAttribute::SHOCKWAVE;
		lSetting.position = transform.translation;
		lSetting.userData = static_cast< void* >( &usData );
		lSetting.shape = shape.get();
		lSetting.trigger = true;
		physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
		physicsSystem_->AddRigidBody(rigidBody);
		rigidBody->SetRigidBodyCollision(this);
		physicsSystem = physicsSystem_;
	}
}

void BossJumpAttackMove::Update()
{
	if ( !animationFinish )
	{

		if ( bossAnimation->GetRatio() >= 0.45f )
		{
			animationFinish = true;
			ShockWave();
		}
	}

	isFinish = ShockWaveUpdate() && animationFinish;

#ifndef _DEBUG
	transform.MakeWorldMatrix();
	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
#endif // _RELEASE

}

void BossJumpAttackMove::TransUpdate(Camera* camera_)
{
	transform.TransUpdate(camera_);

	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
}

const AliceMathF::Vector3& BossJumpAttackMove::GetBossPosition() const
{
	return bossPosition;
}

void BossJumpAttackMove::SetBossPosition(const AliceMathF::Vector3& position_)
{
	bossPosition = position_;
}

void BossJumpAttackMove::SetShockWaveTime(float time_)
{
	shockMaxWaveTime = time_;
}

void BossJumpAttackMove::SetShockWaveRadius(float radius_)
{
	shockWaveMaxRadius = radius_;
	shockWaveMaxRadius = 250.0f;
}

void BossJumpAttackMove::SetShockWaveCollisionRadius(float radius_)
{
	shockWaveMaxRadius = radius_;
	shockWaveMaxRadius = 250.0f;
}

void BossJumpAttackMove::ShockWave()
{
	transform.translation = bossPosition;
	usData.pos = bossPosition;
}

bool BossJumpAttackMove::ShockWaveUpdate()
{
	if ( animationFinish )
	{
		shockWaveTime++;

		if ( time < 2.5f )
		{
			usData.radius = AliceMathF::Lerp(0.0f,250.0f,AliceMathF::Clamp01(time * 1.5f / 2.5f));
		}
		else
		{
			usData.isFinish = true;
		}


		if ( shockWaveTime >= shockMaxWaveTime )
		{
			shockWaveFinish = true;
		}
	}

	return shockWaveFinish;
}

bool BossJumpAttackMove::IsFinish()
{
	return isFinish;
}

void BossJumpAttackMove::End()
{
	animationFinish = false;
	shockWaveFinish = false;
	isFinish = false;
	usData.isFinish = false;
	shockWaveTime = 0;
	time = 0.0f;

	transform.translation = { 0.0f,1000.0f,0.0f };
	transform.MakeWorldMatrix();
	rigidBody->SetMatrix(transform.rigidBodyMatWorld,transform.matWorld);
}

void BossJumpAttackMove::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	physicsSystem_->RemoveRigidBody(rigidBody);
	bossAnimation = nullptr;
}

void BossJumpAttackMove::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
}

void BossJumpAttackMove::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{

}

void BossJumpAttackMove::OnCollisionExit()
{
}

void BossJumpAttackMove::Draw()
{
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 0.0f ,1.0f ,0.0f ,1.0f },true);
}