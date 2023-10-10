#include "BasicGPUParticle.h"
#include<BaseBuffer.h>
#include<DescriptorHeap.h>

void BasicGPUParticle::Initialize()
{
	freeListBuffer = CreateUniqueUAVRWStructuredBuffer(particleMaxCount,sizeof(uint32_t),AdaptersIndex::SUB,HEAP_TYPE::DEFAULT);
	particlePoolBuffer = CreateUniqueUAVRWStructuredBuffer(particleMaxCount,sizeof(ParticleData),AdaptersIndex::SUB,HEAP_TYPE::DEFAULT);
	drawListBuffer = CreateUniqueUAVRWStructuredBuffer(particleMaxCount,sizeof(uint32_t),AdaptersIndex::SUB,HEAP_TYPE::DEFAULT);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstants),AdaptersIndex::SUB);
	viewProjectionBuffer = CreateUniqueConstantBuffer(sizeof(ViewProjection),AdaptersIndex::SUB);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstants),AdaptersIndex::SUB);
	emitDataBuffer = CreateUniqueConstantBuffer(sizeof(EmitData),AdaptersIndex::SUB);

	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(particleEmitterMaxCount,sizeof(D3D12_DRAW_ARGUMENTS),AdaptersIndex::SUB);

	D3D12_INDIRECT_ARGUMENT_DESC arg[ 1 ];
	arg[ 0 ].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	D3D12_COMMAND_SIGNATURE_DESC  desc;
	memset(&desc,0,sizeof(desc));
	desc.ByteStride = 20;
	desc.pArgumentDescs = arg;
	desc.NumArgumentDescs = 1;

	device->Get()->CreateCommandSignature(&desc,nullptr,IID_PPV_ARGS(&particleCommandSignature));

}

void BasicGPUParticle::Update(float deltaTime_)
{
	ID3D12GraphicsCommandList* lCommandList = computeCmmandList->GetComputeCommandList();

	emitTimeCounter += deltaTime_;

	PUpdateConstantBuffer(deltaTime_);

	particleConstantsBuffer->Update(&particleConstants);

	//生成
	if ( emitTimeCounter > timeBetweenEmit )
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleEmit");

		lCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lCommandList->SetComputeRootConstantBufferView(0,viewProjectionBuffer->GetAddress());//b0
		lCommandList->SetComputeRootConstantBufferView(1,timeConstantsBuffer->GetAddress());//b1
		lCommandList->SetComputeRootConstantBufferView(2,emitDataBuffer->GetAddress());//b2
		lCommandList->SetComputeRootConstantBufferView(3,particleConstantsBuffer->GetAddress());//b3

		lCommandList->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress());//u1
		lCommandList->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress());//u2
		lCommandList->SetComputeRootDescriptorTable(6,drawListBuffer->GetAddress());//u3

		lCommandList->Dispatch(static_cast<UINT>(emitCount),1,1);
	}

	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleUpdate");

		lCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());

		lCommandList->SetComputeRootDescriptorTable(1,particlePoolBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(2,freeListBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(3,drawListBuffer->GetAddress());

		lCommandList->Dispatch(static_cast< UINT >( particleMaxCount),1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleDrawArgumentUpdate");

		lCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress());

		lCommandList->Dispatch(1,1,1);

	}
}

void BasicGPUParticle::Finalize()
{
}

void BasicGPUParticle::Draw(Camera* camera_)
{
	{
		ViewProjection lViewProjection;

		lViewProjection.view = camera_->GetViewMatrixInv();
		lViewProjection.projection = camera_->GetProjectionMatrix();

		viewProjectionBuffer->Update(&lViewProjection);
	}

	ID3D12GraphicsCommandList* lCommandList = commandList->GetComputeCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("BasicGPUParticleDraw");

	lCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
	lCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

	lCommandList->SetGraphicsRootConstantBufferView(1,timeConstantsBuffer->GetAddress());
	lCommandList->SetGraphicsRootConstantBufferView(2,viewProjectionBuffer->GetAddress());
	lCommandList->SetGraphicsRootConstantBufferView(3,timeConstantsBuffer->GetAddress());

	lCommandList->SetGraphicsRootDescriptorTable(4,particlePoolBuffer->GetAddress());
	lCommandList->SetGraphicsRootDescriptorTable(5,drawListBuffer->GetAddress());

	lCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(),0,nullptr,0);
}

void BasicGPUParticle::ADD(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_)
{
	static_cast< void >(pos_);

	ParticleConstant lParticleConstant;
	lParticleConstant.startColor = setting_.startColor;
	lParticleConstant.endColor = setting_.endColor;
	lParticleConstant.velocity = setting_.velocity;
	lParticleConstant.lifeTime = setting_.LifeTime;
	lParticleConstant.accel = setting_.acceleration;
	lParticleConstant.emitCount = emitCount = setting_.emitCount;

	particleConstants.constants[ emitData.particleDataCount ] = lParticleConstant;

	emitData.particleDataCount += 1;
	emitData.maxParticles = setting_.maxParticles;
}

void BasicGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	time.deltaTime = deltaTime_;
	time.totalTime += deltaTime_;

	timeConstantsBuffer->Update(&time);
}
