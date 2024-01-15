#include "ScatteringGPUParticle.h"

void ScatteringGPUParticle::Initialize()
{
	constantBuffer = CreateUniqueConstantBuffer(sizeof(GPUData),AdaptersIndex::SUB);


	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeScattering",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());
}

void ScatteringGPUParticle::SetCenterPos(const AliceMathF::Vector3& centerPos_)
{
	centerPos = centerPos_;
	dirty = true;
}

void ScatteringGPUParticle::SetVelocityMeshGPUParticle(MeshGPUParticle* meshGPUParticle_)
{

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	{
		GPUData lData;
		lData.speed = speed;
		lData.accel = accel;
		lData.lifeTime = lifeTime;
		lData.centerPos = centerPos;
		lData.maxParticles =static_cast<uint32_t>(meshGPUParticle_->GetMaxParticles());

		constantBuffer->Update(&lData);
	}

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeScattering",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
	lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lComputeCommandList->SetComputeRootConstantBufferView(0,constantBuffer->GetAddress());//b0

	lComputeCommandList->SetComputeRootDescriptorTable(1,meshGPUParticle_->GetParticlePoolBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

	lComputeCommandList->Dispatch(static_cast< UINT >( meshGPUParticle_->GetMaxParticles() / 1024 ) + 1,1,1);
}

void ScatteringGPUParticle::SetVelocityMeshGPUParticle(AnimationMeshGPUParticle* meshGPUParticle_)
{
	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	{
		GPUData lData;
		lData.speed = speed;
		lData.accel = accel;
		lData.lifeTime = lifeTime;
		lData.centerPos = centerPos;
		lData.maxParticles = static_cast< uint32_t >( meshGPUParticle_->GetMaxParticles() );

		constantBuffer->Update(&lData);
	}

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeScatteringParticle",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
	lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lComputeCommandList->SetComputeRootConstantBufferView(0,constantBuffer->GetAddress());//b0

	lComputeCommandList->SetComputeRootDescriptorTable(1,meshGPUParticle_->GetParticlePoolBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

	lComputeCommandList->Dispatch(static_cast< UINT >( meshGPUParticle_->GetMaxParticles() / 1024 ) + 1,1,1);
}

void ScatteringGPUParticle::SetSpeed(float speed_)
{
	speed = speed_;
	dirty = true;

}

void ScatteringGPUParticle::SetAccel(const AliceMathF::Vector3& accel_)
{
	accel = accel_;
	dirty = true;

}

void ScatteringGPUParticle::SetLifeTime(float lifeTime_)
{
	lifeTime = lifeTime_;
	dirty = true;

}

void ScatteringGPUParticle::Update(float deltaTime_)
{
	static_cast< void >( deltaTime_ );
}

void ScatteringGPUParticle::Finalize()
{
}

void ScatteringGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	static_cast< void >( worldMat_ );
	static_cast< void >( billboardMat_ );

}

void ScatteringGPUParticle::SetSetting()
{
}

void ScatteringGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	static_cast< void >( deltaTime_ );

}
