#include "ScatteringGPUParticle.h"

void ScatteringGPUParticle::Initialize()
{
	constantBuffer = CreateUniqueConstantBuffer(sizeof(GPUData),AdaptersIndex::SUB);
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

	ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeScatteringParticle",AdaptersIndex::SUB);

	lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
	lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lComputeCommandList->SetComputeRootConstantBufferView(0,constantBuffer->GetAddress());//b0

	lComputeCommandList->SetComputeRootDescriptorTable(1,meshGPUParticle_->GetParticlePoolBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
	lComputeCommandList->SetComputeRootDescriptorTable(2,meshGPUParticle_->GetDrawListBuffer()->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u2

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
