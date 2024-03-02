#include<JoltPhysics.h>
#include<JoltRigidBody.h>
#include<JoltCollisionFiltering.h>
#include<AliceAssert.h>
#include<AliceMathUtility.h>
#include<JoltSphereShape.h>

void AlicePhysics::JoltPhysics::Initialize()
{
	JPH::RegisterDefaultAllocator();

	JPH::Factory::sInstance = new JPH::Factory();

	JPH::RegisterTypes();

	tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(50 * 1024 * 1024);
	jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs,JPH::cMaxPhysicsBarriers,std::thread::hardware_concurrency() - 1);
	physicsSystem = std::make_unique<JPH::PhysicsSystem>();
	joltObjectLayerPairFilter.SetCollisionFilterConfigTable(&collisionFilterConfigTable);

	physicsSystem->Init(MAX_BODIES,NUM_BODY_MUTEXES,MAX_BODY_PAIRS,MAX_CONTACT_CONSTRAINTS,objectToBroadphaseLayer,objectVsBroadPhaseLayerFilter,joltObjectLayerPairFilter);

	groupFilter = new JoltGroupFilter();
	groupFilter->AddRef();

	rigidBodyManager = std::make_unique<RigidBodyManager>();

	contactListener.SetRigidBodyManager(rigidBodyManager.get());

	physicsSystem->SetBodyActivationListener(&bodyActivationListener);
	physicsSystem->SetContactListener(&contactListener);
	physicsSystem->SetGravity(gravity);

#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
	debugRenderer = std::make_unique<JoltDebugRenderer>();
	debugRenderer->SetDevice(device);
	debugRenderer->SetCommandList(commandList);
	debugRenderer->Initialize();
#endif // _DEBUG

}

void AlicePhysics::JoltPhysics::Update(float deltaTime_,float baseDeltaTime_)
{
	int32_t lCollisionSteps = 1;
	const int32_t lIntegrationSubSteps = 1;

	if ( deltaTime_ > baseDeltaTime_ )
	{
		lCollisionSteps = AliceMathUtility::Min<int32_t>(static_cast< int32_t > ( deltaTime_ / baseDeltaTime_ ),10);
		deltaTime_ = 0.016f;
	}

	// ステップを進める
	physicsSystem->Update(deltaTime_,lCollisionSteps,lIntegrationSubSteps,tempAllocator.get(),jobSystem.get());
}

void AlicePhysics::JoltPhysics::SetGravity(const AliceMathF::Vector3& gravity_)
{
	gravity = gravity_;

	if ( physicsSystem )
	{
		physicsSystem->SetGravity(gravity_);
	}
}

void AlicePhysics::JoltPhysics::AddRigidBody(IRigidBody* rigidBody_)
{
	JPH::BodyInterface& lBodyInterface = physicsSystem->GetBodyInterface();
	JoltRigidBody::BodyData* lData = static_cast< JoltRigidBody::BodyData* >( rigidBody_->GetBody() );
	lData->isAdd = true;

	//追加
	lBodyInterface.AddBody(lData->id,lData->isActive ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);
}

void AlicePhysics::JoltPhysics::RemoveRigidBody(IRigidBody* rigidBody_)
{
	JPH::BodyInterface& lBodyInterface = physicsSystem->GetBodyInterface();
	JoltRigidBody::BodyData* lData = static_cast< JoltRigidBody::BodyData* >( rigidBody_->GetBody() );

	// システムから取り除いて削減する
	if ( lData->isAdd )
	{
		lBodyInterface.RemoveBody(lData->id);
	}

	lBodyInterface.DestroyBody(lData->id);

	rigidBodyManager->RemoveRigidBody(lData->id.GetIndex());

}

void AlicePhysics::JoltPhysics::CreateRigidBody(IRigidBody*& rigidBody_,const IRigidBodyCreationSettings* settings_)
{
	std::unique_ptr<IRigidBody>lIRigidBody;
	JPH::BodyInterface& lBodyInterface = physicsSystem->GetBodyInterface();
	JPH::BodyCreationSettings lSetting;
	JoltRigidBody* lRigidBody = new JoltRigidBody();

	JPH::Shape* lShape;
	PGetShape(lShape,settings_->shape);

	if ( settings_->parent )
	{
		JoltRigidBody* lParent = static_cast< JoltRigidBody* >( settings_->parent );
		CollisionGroup lCollisionGroup = lParent->rigidBodyUserData->GetGroup();
		groupFilter->EnableCollisionGroups(static_cast< uint32_t >( lCollisionGroup ),static_cast< uint32_t >( settings_->collisionGroup ),true);
	}

	std::unique_ptr<RigidBodyUserData>& userData = lRigidBody->rigidBodyUserData;

	userData = std::make_unique<RigidBodyUserData>();
	userData->SetName(settings_->name);
	userData->SetUserData(settings_->userData);
	userData->SetAttribute(settings_->collisionAttribute);
	userData->SetGroup(settings_->collisionGroup);
	userData->SetRigidBody(lRigidBody);

	lSetting.mPosition = settings_->position;
	lSetting.mRotation = settings_->rotation;
	lSetting.mMotionType = static_cast< JPH::EMotionType >( settings_->motionType );
	lSetting.mObjectLayer = ConstructObjectLayer(settings_->type,static_cast< uint8_t >( settings_->collisionAttribute ));
	lSetting.mLinearDamping = settings_->linearDamping;
	lSetting.mAngularDamping = settings_->angularDamping;
	lSetting.mMassPropertiesOverride.mMass = settings_->mass;
	lSetting.mOverrideMassProperties = settings_->mass > 0.0f ? JPH::EOverrideMassProperties::CalculateInertia : JPH::EOverrideMassProperties::CalculateMassAndInertia;
	lSetting.mRestitution = settings_->restitution;
	lSetting.mFriction = settings_->friction;
	lSetting.mCollisionGroup.SetGroupID(static_cast< uint32_t >( settings_->collisionGroup ));
	lSetting.mCollisionGroup.SetGroupFilter(groupFilter);
	lSetting.mUserData = reinterpret_cast< uint64_t >( userData->GetRigidBodyUserData() );
	lSetting.mAllowSleeping = settings_->allowSleeping;
	lSetting.mMotionQuality = settings_->linearCast ? JPH::EMotionQuality::LinearCast : JPH::EMotionQuality::Discrete;
	lSetting.mIsSensor = settings_->trigger;
	lSetting.SetShape(lShape);
	lSetting.mMaxLinearVelocity = settings_->maxLinearVelocity;
	lSetting.mMaxAngularVelocity = settings_->maxAngularVelocity;
	//生成
	lRigidBody->body = lBodyInterface.CreateBody(lSetting);

	lRigidBody->bodyData.id = lRigidBody->body->GetID();
	lRigidBody->bodyData.isActive = settings_->isActive;
	lRigidBody->bodyInterface = &physicsSystem->GetBodyInterface();
	lRigidBody->id = lRigidBody->bodyData.id;

	rigidBody_ = lRigidBody;

	lIRigidBody.reset(lRigidBody);

	rigidBodyManager->AddRigidBody(lIRigidBody,lRigidBody->body->GetID().GetIndex());
}

void AlicePhysics::JoltPhysics::Finalize()
{
	groupFilter->Release();
	groupFilter = nullptr;

	JPH::UnregisterTypes();

	// ファクトリーをデリート
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void AlicePhysics::JoltPhysics::Draw()
{
#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
	debugRenderer->Draw();
#endif // _DEBUG

}

void AlicePhysics::JoltPhysics::SetScaleShape(const AliceMathF::Vector3& scale,IRigidBody* rigidBody_,IShape*& shape_)
{
	JPH::BodyInterface& lBodyInterface = physicsSystem->GetBodyInterface();
	JoltRigidBody* lIRigidBody = dynamic_cast< JoltRigidBody* >( rigidBody_ );
	JPH::Shape* lShape;
	PGetShape(lShape,shape_);
	JPH::Shape::ShapeResult resilt = lShape->ScaleShape(scale);
	lBodyInterface.SetShape(lIRigidBody->id,resilt.Get(),false,JPH::EActivation::Activate);

	lIRigidBody->SetShape(shape_);
}

void AlicePhysics::JoltPhysics::SetDevice(IDevice* device_)
{
	device = device_;
}

void AlicePhysics::JoltPhysics::SetCommandList(ICommandList* commandList_)
{
	commandList = commandList_;
}

void AlicePhysics::JoltPhysics::SetViewProjection(const AliceMathF::Matrix4& viewMat_,const AliceMathF::Matrix4& projectionMat_)
{
#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
	debugRenderer->SetViewProjection(viewMat_,projectionMat_);
#else
	static_cast< void >( viewMat_ );
	static_cast< void >( projectionMat_ );
#endif // _DEBUG

}

void AlicePhysics::JoltPhysics::SetLight(AliceMathF::Vector3* lightV_,AliceMathF::Vector4* lightColor_)
{
#if defined(_DEBUG) && defined(JPH_DEBUG_RENDERER)
	debugRenderer->SetLight(lightV_,lightColor_);
#else
	static_cast< void >( lightV_ );
	static_cast< void >( lightColor_ );
#endif // _DEBUG

}

void AlicePhysics::JoltPhysics::PGetShape(JPH::Shape*& joltShape_,IShape* shape_)
{
	AliceAssertNull(shape_,"シェイプが設定されてません\n");

	switch ( shape_->GetShapeType() )
	{
	case SPHERE:
	{
		JPH::SphereShape* lSphere = static_cast< JPH::SphereShape* >( shape_->GetGetShape() );
		joltShape_ = lSphere;
	}
	break;

	case BOX:
	{
		JPH::BoxShape* lBox = static_cast< JPH::BoxShape* >( shape_->GetGetShape() );
		joltShape_ = lBox;
	}
	break;
	case CAPSULE:
	{
		JPH::CapsuleShape* lCapsule = static_cast< JPH::CapsuleShape* >( shape_->GetGetShape() );
		joltShape_ = lCapsule;
	}
	break;
	case MESH:
	{
		JPH::MeshShape* lMesh = static_cast< JPH::MeshShape* >( shape_->GetGetShape() );
		joltShape_ = lMesh;
	}
	break;
	case PHYSICSSHAPETYPE_COUNT:
	default:
		AliceAssert(0,"形以外入れないで");
		break;
	}
}

void AlicePhysics::JoltPhysics::EnableCollision(uint32_t groupA_,uint32_t groupB_,bool enable_)
{
	collisionFilterConfigTable.EnableCollision(groupA_,groupB_,enable_);
}


void AlicePhysics::JoltBodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID,uint64_t inBodyUserData)
{
	static_cast< void >( inBodyID );

	RigidBodyUserData* data = reinterpret_cast< RigidBodyUserData* >( inBodyUserData );

	printf("%sは起動しました。\n",data->GetName().c_str());
}

void AlicePhysics::JoltBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID,uint64_t inBodyUserData)
{

	static_cast< void >( inBodyID );
	RigidBodyUserData* data = reinterpret_cast< RigidBodyUserData* >( inBodyUserData );

	printf("%sは停止しました。\n",data->GetName().c_str());
}