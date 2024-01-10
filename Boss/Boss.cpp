#include "Boss.h"

#include<CollisionAttribute.h>
#include<CapsuleShape.h>

void Boss::Initialize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	modelHandle = AliceModel::SCreateModel("Resources/Model/Boss");
	model = std::make_unique<AliceModel>();
	model->SetModel(modelHandle);

	bossParticleModel = std::make_unique<AliceModel>();
	bossParticleModel->SetModel(AliceModel::SCreateModel("Resources/Model/Boss/Particle"));

	transform.translation = { 0.0f,0.0f,20.0f };
	transform.scale = {0.01f,0.01f,0.01f };
	transform.Initialize();

	rigidBodyoffset = { 4.0f, 30.0f + 35.0f, 0.0f };

	AliceMathF::Vector3 pos = transform.translation + rigidBodyoffset;
	shape.reset(AlicePhysics::CreateCapsuleShape(35.0f,30.0f));

	AlicePhysics::IRigidBodyCreationSettings lSetting;
	lSetting.name = "Boss";
	lSetting.type = AlicePhysics::PhysicsRigidBodyType::DYNAMIC;
	lSetting.motionType = AlicePhysics::MotionType::DYNAMIC;
	lSetting.allowSleeping = false;
	lSetting.collisionGroup = CollisionGroup::BOSS;
	lSetting.collisionAttribute = CollisionAttribute::BODY;
	lSetting.position = pos;
	lSetting.shape = shape.get();
	lSetting.userData = static_cast< void* >( &situation );
	physicsSystem_->CreateRigidBody(rigidBody,&lSetting);
	physicsSystem_->AddRigidBody(rigidBody);
	rigidBody->SetRigidBodyCollision(this);

	{

		{
			AnimationMeshGPUParticleSetting lMeshSetting;

			lMeshSetting.matWorld = AliceMathF::MakeIdentity();
			lMeshSetting.velocity = { 0,1,0 };
			lMeshSetting.startColor = { 1,0,0,1 };
			lMeshSetting.endColor = { 1,0,0,1 };
			lMeshSetting.lifeTime = 1.0f;
			lMeshSetting.maxParticles = 1000000.0f;
			lMeshSetting.timeBetweenEmit = 0.01f;
			lMeshSetting.emitLifeTime = -1;
			lMeshSetting.size = 2;
			lMeshSetting.speed = 15;
			lMeshSetting.isPlay = false;

			meshParticleIndex = particleEmitter->AnimationMeshGPUParticleEmit("BossParticle",lMeshSetting);
			//bossGPUParticle = particleEmitter->GetAnimationMeshGPUParticle("BossParticle");
		}

		{
			bossModelGPUParticle = particleEmitter->GetAnimationModelGPUParticle("BossModelParticle");
		}

		BloodGushGPUParticleSetting lBloodGushSetting;
		lBloodGushSetting.accel = { 0.0f,-8.5f,0.0f };
		lBloodGushSetting.amount = 1.0f;
		lBloodGushSetting.emitLifeTime = 0.1f;
		lBloodGushSetting.endColor = { 0.0f,1.0f,1.0f,1.0f };
		lBloodGushSetting.startColor = { 0.0f,1.0f,1.0f,1.0f };
		lBloodGushSetting.lifeTime = 0.4f;
		lBloodGushSetting.size = { 20.0f,11.0f };
		lBloodGushSetting.speed = 350.0f;
		lBloodGushSetting.timeBetweenEmit = 0.05f;
		lBloodGushSetting.isPlay = false;
		bloodGushParticleIndex = particleEmitter->BloodGushGPUParticleEmit("BossBloodGushParticle",lBloodGushSetting);
		bloodGushGPUParticle = particleEmitter->GetBloodGushGPUParticle("BossBloodGushParticle");

		//bossGPUParticle->EmitPlay(meshParticleIndex);
		bossModelGPUParticle->EmitPlay();

	}

	damageSE = audioManager->LoadAudio("Resources/SE/Damage.mp3",0.3f);
	deathSE = audioManager->LoadAudio("Resources/SE/BossDeath.mp3");

	animation = std::make_unique<BossAnimation>();
	animation->Initialize();

	actionManager = std::make_unique<BossActionManager>();
	actionManager->SetParticleEmitter(particleEmitter);
	actionManager->Initialize(animation.get(),physicsSystem_,&transform);


	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ] = std::make_unique<BossHand>();
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ] = std::make_unique<BossHand>();

	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->SetFireGPUParticle(particleEmitter->GetFireParticle("BossHandParticle"));
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->SetFireGPUParticle(particleEmitter->GetFireParticle("BossHandParticle"));

	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Initialize(&transform,physicsSystem_,BossHandIndex::LEFT);
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Initialize(&transform,physicsSystem_,BossHandIndex::RIGHT);

	bossUI = std::make_unique<BossUI>();
	bossUI->Initialize();

	Reset();
}

void Boss::Update()
{
	oldTrans = transform.translation;

	if ( hp > 0 )
	{
		actionManager->Update(player->GetPosition(),transform.translation,"mixamorig:RightHand",animation->GetAnimation(),model.get());
	}

	bossUI->SetHp(hp,MAX_HP);

	if ( hp > 0 )
	{
		if ( actionManager->GetinternalAction() == BossInternalAction::ATTACK )
		{
			situation |= ActorSituation::ATTACK;
			animation->SetAddFrame(0.01f);
		}
		else
		{
			if ( situation & ActorSituation::ATTACK )
			{
				situation &= ~ActorSituation::ATTACK;
				animation->SetAddFrame(0.02f);
			}
		}

		//移動


		AliceMathF::Vector3 lMove = actionManager->GetDistanceTraveled();
		rigidBody->SetLinearVelocity(lMove);

		{
			rigidBody->SetRotation(AliceMathF::Quaternion());

			transform.translation = rigidBody->GetPosition() + -rigidBodyoffset;
			actionManager->SetBossPos(rigidBody->GetPosition());
		}

		direction = actionManager->GetDirection();

		if ( AliceMathF::Vector3(0,0,0) != lMove )
		{
			direction = lMove.Normal();
			direction = -direction;
			direction = direction.Normal();
		}
	}

	if ( situation & ActorSituation::DAMAGE && !player->IsAttack() )
	{
		situation &= ~ActorSituation::DAMAGE;
	}

	animation->Update();
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Update("mixamorig:RightHandMiddle1",animation->GetAnimation(),model.get());
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Update("mixamorig:LeftHandMiddle1",animation->GetAnimation(),model.get());
	bossUI->Update();

	if( situation & ActorSituation::ATTACK )
	{
		hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->ParticleEmit();

		if ( animation->GetAnimation()->GetRatio() >= 0.22f && animation->GetAnimation()->GetRatio() <= 0.6f)
		{
			hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->SetSituation(situation);
			hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->SetSituation(situation);

		}
		else
		{
			int32_t lSituation = situation;
			lSituation &= ~ActorSituation::ATTACK;

			hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->SetSituation(lSituation);
			hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->SetSituation(lSituation);

		}
	}
	else
	{

		hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->ParticleStop();
	}

	bossParticleModel->AnimationUpdate(animation->GetAnimation());
	model->AnimationUpdate(animation->GetAnimation());
}

void Boss::Draw()
{
	//model->Draw(transform,animation->GetAnimation());
	shape->Draw(rigidBody->GetCenterOfMassTransform(),{ 1.0f,1.0f ,1.0f },{ 1.0f ,0.0f ,0.0f ,1.0f },true);
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Draw();
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Draw();
	actionManager->GetBossCloseRangeAttack()->Draw();

#ifdef _DEBUG
	actionManager->GetBossJumpAttackMove()->Draw();
	actionManager->GetBossBeamAttackMove()->Draw();

#endif
}

void Boss::UIDraw()
{
	bossUI->Draw();
}

void Boss::Finalize(AlicePhysics::AlicePhysicsSystem* physicsSystem_)
{
	//bossGPUParticle->EmitStop(meshParticleIndex);
	bossModelGPUParticle->EmitStop();

	actionManager->Finalize(physicsSystem_);

	particleEmitter = nullptr;

	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Finalize(physicsSystem_);
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Finalize(physicsSystem_);

	physicsSystem_->RemoveRigidBody(rigidBody);
}

void Boss::Reset()
{
	hp = MAX_HP;
	bossUI->SetHp(hp,MAX_HP);
}

void Boss::TransUpdate(Camera* camera_)
{
	transform.LookAtMatrixAxisFix(direction,{ 0,1,0 },camera_);

	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->TransUpdate(camera_);
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->TransUpdate(camera_);

	actionManager->GetBossBeamAttackMove()->TransUpdate(camera_);
	actionManager->GetBossCloseRangeAttack()->TransUpdate(camera_);

	//bossGPUParticle->SetMat(transform.matWorld,meshParticleIndex);
	bossModelGPUParticle->SetMat(transform.matWorld);

#ifdef _DEBUG
	actionManager->GetBossJumpAttackMove()->TransUpdate(camera_);

#endif

	camera = camera_;
}

void Boss::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
	if ( BodyData_->GetGroup() == CollisionGroup::PLAYER && BodyData_->GetAttribute() == CollisionAttribute::WEAPON && player->IsAttack() && !( situation & ActorSituation::DAMAGE ) )
	{
		if ( BodyData_->GetName() == "PlayerGreatWeapon" )
		{
			PlayerGreatWeaponUsData* lUsData = static_cast< PlayerGreatWeaponUsData* > ( BodyData_->GetUserData() );

			if ( hp > 0 )
			{
				for ( int32_t i = 0; i < player->GetDamage(); i++ )
				{
					if ( hp <= 0 )
					{
						break;
					}

					//hp--;
				}

				situation |= ActorSituation::DAMAGE;
				audioManager->PlayWave(damageSE);
				bloodGushGPUParticle->EmitPlay(hitPosdition_,lUsData->velocity,bloodGushParticleIndex);

				if ( hp <= 0 )
				{
					animation->InsertDeathAnimation();
					animation->AnimationEndStop();
					audioManager->PlayWave(deathSE);
					audioManager->ChangeVolume(deathSE,deathSEVolume);
				}
			}
		}
		else
		{
			PlayerWeaponUsData* lUsData = static_cast< PlayerWeaponUsData* > ( BodyData_->GetUserData() );

			if ( hp > 0 )
			{
				for ( int32_t i = 0; i < player->GetDamage(); i++ )
				{
					if ( hp <= 0 )
					{
						break;
					}

					//hp--;
				}

				situation |= ActorSituation::DAMAGE;
				audioManager->PlayWave(damageSE);
				bloodGushGPUParticle->EmitPlay(hitPosdition_,lUsData->velocity,bloodGushParticleIndex);

				if ( hp <= 0 )
				{
					animation->InsertDeathAnimation();
					animation->AnimationEndStop();
					audioManager->PlayWave(deathSE);
					audioManager->ChangeVolume(deathSE,deathSEVolume);
				}
			}
		}

	}

}

void Boss::OnCollisionStay(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
}

void Boss::SetPlayer(Player* player_)
{
	player = player_;
}

void Boss::SetAudioManager(IAudioManager* audioManager_)
{
	audioManager = audioManager_;
}

void Boss::SetFireGPUParticle(GPUParticleEmitter* particleEmitter_)
{
	particleEmitter = particleEmitter_;
}

int32_t Boss::GetHp()
{
	return hp;
}

bool Boss::IsEnd()
{
	return hp <= 0 && !animation->IsInsert();
}

void Boss::AnimationStop()
{
	animation->AnimationStop();
}

void Boss::AnimationEndStop()
{
	animation->AnimationEndStop();

}

void Boss::DeathSEChangeVolume(float volume_)
{
	audioManager->ChangeVolume(deathSE,deathSEVolume * volume_);
}

void Boss::OnCollisionExit()
{
}
