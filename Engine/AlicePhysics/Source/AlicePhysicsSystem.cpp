#include "AlicePhysicsSystem.h"


#include<JoltPhysics.h>

void AlicePhysics::AlicePhysicsSystem::Initialize()
{

//ジョルト
#if AlicePhysicsJolt

	physics = std::make_unique<JoltPhysics>();

	physics->SetDevice(device);
	physics->SetCommandList(commandList);

	physics->Initialize();

#endif
}

void AlicePhysics::AlicePhysicsSystem::Update(float deltaTime_,float baseDeltaTime_)
{
	physics->Update(deltaTime_,baseDeltaTime_);
}

void AlicePhysics::AlicePhysicsSystem::SetGravity(const AliceMathF::Vector3& gravity_)
{
	physics->SetGravity(gravity_);

}

void AlicePhysics::AlicePhysicsSystem::AddRigidBody(IRigidBody* rigidBody_)
{
	physics->AddRigidBody(rigidBody_);

}

void AlicePhysics::AlicePhysicsSystem::RemoveRigidBody(IRigidBody* rigidBody_)
{
	physics->RemoveRigidBody(rigidBody_);

}

void AlicePhysics::AlicePhysicsSystem::CreateRigidBody(IRigidBody*& rigidBody_, const IRigidBodyCreationSettings* settings_)
{
	physics->CreateRigidBody(rigidBody_, settings_);

}

void AlicePhysics::AlicePhysicsSystem::Finalize()
{
	physics->Finalize();

}

void AlicePhysics::AlicePhysicsSystem::Draw()
{
#if _DEBUG && defined(JPH_DEBUG_RENDERER)

	physics->Draw();

#endif // _DEBUG

}

void AlicePhysics::AlicePhysicsSystem::SetViewProjection(const AliceMathF::Matrix4& viewMat_,const AliceMathF::Matrix4& projectionMat_)
{
	physics->SetViewProjection(viewMat_, projectionMat_);
}

void AlicePhysics::AlicePhysicsSystem::SetLight(AliceMathF::Vector3* lightV_, AliceMathF::Vector4* lightColor_)
{
	physics->SetLight(lightV_, lightColor_);
}

void AlicePhysics::AlicePhysicsSystem::SetDevice(IDevice* device_)
{
	device = device_;
}

void AlicePhysics::AlicePhysicsSystem::SetCommandList(ICommandList* commandList_)
{
	commandList = commandList_;
}
