#include "AggregatingGPUParticle.h"

void AggregatingGPUParticle::Initialize()
{
	constantBuffer = CreateUniqueConstantBuffer(sizeof(GPUData),AdaptersIndex::SUB);


	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAggregating",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());
}

void AggregatingGPUParticle::SetCenterPos(const AliceMathF::Vector3& centerPos_)
{
	centerPos = centerPos_;
	dirty = true;
}

void AggregatingGPUParticle::SetVelocityMeshGPUParticle(MeshGPUParticle* meshGPUParticle_)
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

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAggregating",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
	lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lComputeCommandList->SetComputeRootConstantBufferView(0,constantBuffer->GetAddress());//b0

	lComputeCommandList->SetComputeRootDescriptorTable(1,meshGPUParticle_->GetParticlePoolBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

	lComputeCommandList->Dispatch(static_cast< UINT >( meshGPUParticle_->GetMaxParticles() / 1024 ) + 1,1,1);
}

void AggregatingGPUParticle::SetVelocityMeshGPUParticle(AnimationMeshGPUParticle* meshGPUParticle_)
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

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAggregating",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
	lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lComputeCommandList->SetComputeRootConstantBufferView(0,constantBuffer->GetAddress());//b0

	lComputeCommandList->SetComputeRootDescriptorTable(1,meshGPUParticle_->GetParticlePoolBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

	lComputeCommandList->Dispatch(static_cast< UINT >( meshGPUParticle_->GetMaxParticles() / 1024 ) + 1,1,1);
}

void AggregatingGPUParticle::SetSpeed(float speed_)
{
	speed = speed_;
	dirty = true;

}

void AggregatingGPUParticle::SetAccel(const AliceMathF::Vector3& accel_)
{
	accel = accel_;
	dirty = true;

}

void AggregatingGPUParticle::SetLifeTime(float lifeTime_)
{
	lifeTime = lifeTime_;
	dirty = true;

}

void AggregatingGPUParticle::Update(float deltaTime_)
{
	static_cast< void >( deltaTime_ );
}

void AggregatingGPUParticle::Finalize()
{
}

void AggregatingGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	static_cast< void >( worldMat_ );
	static_cast< void >( billboardMat_ );

}

void AggregatingGPUParticle::SetSetting()
{
}

void AggregatingGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	static_cast< void >( deltaTime_ );

}
