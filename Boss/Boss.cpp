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
	transform.scale = { 0.01f,0.01f,0.01f };
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

			//meshParticleIndex = particleEmitter->AnimationMeshGPUParticleEmit("BossParticle",lMeshSetting);
			//bossGPUParticle = particleEmitter->GetAnimationMeshGPUParticle("BossParticle");
		}

		{
			bossModelGPUParticle = particleEmitter->GetAnimationModelGPUParticle("BossModelParticle");
		}

		{
			deathGPUParticle = particleEmitter->GetAnimationMeshGPUParticle("BossDeathEffectParticle");
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
		particleEmitter->BloodGushGPUParticleEmit("BossBloodGushParticle",lBloodGushSetting);
		bloodGushGPUParticle = particleEmitter->GetBloodGushGPUParticle("BossBloodGushParticle");

		//bossGPUParticle->EmitPlay();
		bossModelGPUParticle->EmitPlay();

	}

	damageSE = audioManager->LoadAudio("Resources/SE/Damage.mp3",0.3f);
	deathSE = audioManager->LoadAudio("Resources/SE/BossDeath.mp3");

	animation = std::make_unique<BossAnimation>();
	animation->Initialize();

	actionManager = std::make_unique<BossActionManager>();
	actionManager->SetParticleEmitter(particleEmitter);
	actionManager->SetPlayer(player);
	actionManager->SetBoss(this);
	actionManager->Initialize(animation.get(),physicsSystem_,&transform);


	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ] = std::make_unique<BossHand>();
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ] = std::make_unique<BossHand>();

	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->SetFireGPUParticle(particleEmitter->GetFireParticle("BossHandParticle"));
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->SetFireGPUParticle(particleEmitter->GetFireParticle("BossHandParticle"));

	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Initialize(&transform,physicsSystem_,BossHandIndex::LEFT,this,particleEmitter);
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Initialize(&transform,physicsSystem_,BossHandIndex::RIGHT,this,particleEmitter);

	bossUI = std::make_unique<BossUI>();
	bossUI->Initialize();

	Reset();



}

void Boss::Update()
{
	if ( !isMove && player->GetPosition().z >= -230 )
	{
		isMove = true;
	}
	isMove = false;

	oldTrans = transform.translation;

	if ( hp > 0 )
	{
		actionManager->Update("mixamorig:RightHand",animation->GetAnimation(),model.get(),isMove,{ hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->GetCenterOfMassTransform(),hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->GetCenterOfMassTransform() });
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

		if ( situation & ActorSituation::ATTACK )
		{
			if ( animation->GetAnimation()->GetRatio() >= 0.22f && animation->GetAnimation()->GetRatio() <= 0.6f )
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
		//
		if ( hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->GetCutting() && !( situation & ActorSituation::LEFT_CUTTING ) ||
			hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->GetDeath() && !( situation & ActorSituation::LEFT_CUTTING ) ||
			hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->GetCutting() && !( situation & ActorSituation::RIGHT_CUTTING ) ||
			hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->GetDeath() && !( situation & ActorSituation::RIGHT_CUTTING ) )
		{
			reconstruction = true;

			if ( hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->GetCutting() || hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->GetDeath() )
			{
				situation |= ActorSituation::LEFT_CUTTING;
			}

			if ( hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->GetCutting() || hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->GetDeath() )
			{
				situation |= ActorSituation::RIGHT_CUTTING;
			}


		}

		if ( reconstruction && !actionManager->IsReconstruction() )
		{
			reconstructionTime--;

			if ( reconstructionTime <= 0 || situation & ActorSituation::LEFT_CUTTING && situation & ActorSituation::RIGHT_CUTTING )
			{
				if ( !hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->GetDeath() && !hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->GetDeath() )
				{
					actionManager->StartReconstruction();
					hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Reset();
					hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Reset();
					reconstruction = false;
				}
			}
		}

		if ( situation & ActorSituation::DAMAGE && !player->IsAttack() )
		{
			situation &= ~ActorSituation::DAMAGE;
		}
	}

	animation->Update();
	hands[ static_cast< size_t >( BossHandIndex::RIGHT ) ]->Update("mixamorig:RightHandMiddle1",animation->GetAnimation(),model.get());
	hands[ static_cast< size_t >( BossHandIndex::LEFT ) ]->Update("mixamorig:LeftHandMiddle1",animation->GetAnimation(),model.get());
	bossUI->Update();

	if ( hp <= 0 && !animation->IsInsert() )
	{
		death = true;
		bossModelGPUParticle->EmitStop();
		deathGPUParticle->EmitPlay();
		particleEmitter->ScatteringSetSpeed(600.0f);
		//gpuParticleEmitter->ScatteringSetAccel({ 0,10,0 });
		AliceMathF::Vector3 centerPos = AliceMathF::GetWorldPosition(transform.matWorld);
		centerPos += direction * rigidBodyoffset.y;
		centerPos.y += rigidBodyoffset.x;
		particleEmitter->ScatteringSetCenterPos(centerPos);
		particleEmitter->AnimationMeshGPUParticleScattering("BossDeathEffectParticle");
		deathGPUParticle->EmitStop();
	}

	if ( death )
	{
		deathTime--;

		if ( deathTime <= 60 )
		{
			deathGPUParticle->DrawStop();
		}

		if ( !deathTime )
		{
			deathTime = 100;
			end = true;
		}

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
	bloodGushGPUParticle->EmitStop();
	//bossGPUParticle->EmitStop(0);
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

	//bossGPUParticle->SetMat(transform.matWorld,0);

	bossModelGPUParticle->SetMat(transform.matWorld);
	deathGPUParticle->SetMat(transform.matWorld);
	particleEmitter->GetAnimationMeshGPUParticle("BossLeftHandParticle")->SetMat(transform.matWorld);
	particleEmitter->GetAnimationMeshGPUParticle("BossRightHandParticle")->SetMat(transform.matWorld);
	particleEmitter->GetMeshGPUParticle("EffectSphereParticle")->SetMat(rigidBody->GetCenterOfMassTransform());
#ifdef _DEBUG
	actionManager->GetBossJumpAttackMove()->TransUpdate(camera_);

#endif

	camera = camera_;
}

void Boss::OnCollisionEnter(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_)
{
	DamageEnterCalculation(BodyData_,hitPosdition_);
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

int32_t Boss::GetHp() const
{
	return hp;
}

AnimationModelGPUParticle* Boss::GetModelParticle()
{
	return bossModelGPUParticle;
}

bool Boss::IsEnd() const
{
	return end;// hp <= 0 && !animation->IsInsert();
}

void Boss::CuttingReset()
{
	if ( situation & ActorSituation::LEFT_CUTTING )
	{
		situation &= ~ActorSituation::LEFT_CUTTING;
	}

	if ( situation & ActorSituation::RIGHT_CUTTING )
	{
		situation &= ~ActorSituation::RIGHT_CUTTING;
	}
	reconstruction = false;
	reconstructionTime = 100;
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

AliceMathF::Vector3 Boss::GetPos() const
{
	return transform.translation;
}

AliceMathF::Vector3 Boss::GetCenterPos() const
{
	return AliceMathF::GetWorldPosition(rigidBody->GetCenterOfMassTransform());
}

AliceMathF::Matrix4 Boss::GetCenterOfMassTransform()
{
	return rigidBody->GetCenterOfMassTransform();
}

int32_t Boss::GetSituation() const
{
	return situation;
}

void Boss::DamageEnterCalculation(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_,float power_)
{
	if ( BodyData_->GetGroup() == CollisionGroup::PLAYER && BodyData_->GetAttribute() == CollisionAttribute::WEAPON && player->IsAttack() && !( situation & ActorSituation::DAMAGE ) )
	{
		if ( BodyData_->GetName() == "PlayerGreatWeapon" )
		{
			PlayerGreatWeaponUsData* lUsData = static_cast< PlayerGreatWeaponUsData* > ( BodyData_->GetUserData() );

			int32_t lLength = static_cast< int32_t >( player->GetGreatDamage() * power_ );

			if ( hp > 0 )
			{
				for ( int32_t i = 0; i < lLength; i++ )
				{
					if ( hp <= 0 )
					{
						break;
					}

					hp--;
				}

				situation |= ActorSituation::DAMAGE;
				audioManager->PlayWave(damageSE);
				bloodGushGPUParticle->EmitPlay(hitPosdition_,lUsData->velocity);

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
				int32_t lLength = static_cast< int32_t >( player->GetDamage() * power_ );

				for ( int32_t i = 0; i < lLength; i++ )
				{
					if ( hp <= 0 )
					{
						break;
					}

					hp--;
				}

				situation |= ActorSituation::DAMAGE;
				audioManager->PlayWave(damageSE);
				bloodGushGPUParticle->EmitPlay(hitPosdition_,lUsData->velocity);

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

bool Boss::DamageEnterCalculation(AlicePhysics::RigidBodyUserData* BodyData_,const AliceMathF::Vector3& hitPosdition_,int32_t& hp_,float power_,bool sound_,bool particle_)
{
	if ( BodyData_->GetGroup() == CollisionGroup::PLAYER && BodyData_->GetAttribute() == CollisionAttribute::WEAPON && player->IsAttack() )
	{
		if ( BodyData_->GetName() == "PlayerGreatWeapon" )
		{
			PlayerGreatWeaponUsData* lUsData = static_cast< PlayerGreatWeaponUsData* > ( BodyData_->GetUserData() );

			int32_t lLength = static_cast< int32_t >( player->GetGreatDamage() * power_ );

			if ( hp_ > 0 )
			{
				for ( int32_t i = 0; i < lLength; i++ )
				{
					if ( hp_ <= 0 )
					{
						break;
					}

					hp_--;
				}

				if ( sound_ )
				{
					audioManager->PlayWave(damageSE);
				}

				if ( particle_ )
				{
					bloodGushGPUParticle->EmitPlay(hitPosdition_,lUsData->velocity);
				}
			}

			return true;
		}
		else
		{
			PlayerWeaponUsData* lUsData = static_cast< PlayerWeaponUsData* > ( BodyData_->GetUserData() );

			if ( hp_ > 0 )
			{
				int32_t lLength = static_cast< int32_t >( player->GetDamage() * power_ );

				for ( int32_t i = 0; i < lLength; i++ )
				{
					if ( hp_ <= 0 )
					{
						break;
					}

					hp_--;
				}

				if ( sound_ )
				{
					audioManager->PlayWave(damageSE);
				}

				if ( particle_ )
				{
					bloodGushGPUParticle->EmitPlay(hitPosdition_,lUsData->velocity);
				}
			}

			return true;
		}
	}

	return false;
}
