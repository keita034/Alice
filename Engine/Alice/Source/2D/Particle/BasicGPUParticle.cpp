#include "BasicGPUParticle.h"
#include<BaseBuffer.h>
#include<DescriptorHeap.h>

void BasicGPUParticle::Initialize()
{

	D3D12_INDIRECT_ARGUMENT_DESC arg[ 1 ]{};
	arg[ 0 ].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	D3D12_COMMAND_SIGNATURE_DESC  desc{};
	desc.ByteStride = sizeof(D3D12_DRAW_ARGUMENTS);
	desc.pArgumentDescs = arg; 
	desc.NumArgumentDescs = 1;

	ID3D12Device* lGraphicDevice = graphicAdapter->GetDevice()->Get();

	lGraphicDevice->CreateCommandSignature(&desc,nullptr,IID_PPV_ARGS(&particleCommandSignature));

	PBufferCreate();

	emitDataBuffer->Update(&emitData);
	particleConstantsBuffer->Update(&particleConstant);

	//フリーリスト初期化
	{
		ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetComputeCommandList();

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleFreeListInit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress());//u0
		lComputeCommandList->SetComputeRootConstantBufferView(1,emitDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( emitData.maxParticles / 1024 ) + 1,1,1);
	}
}

void BasicGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetComputeCommandList();

	//生成
	if ( emitTimeCounter >= timeBetweenEmit )
	{
		emitTimeCounter = 0.0f;

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleEmit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,emitDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress());//u1

		lComputeCommandList->Dispatch(static_cast< UINT >( emitData.emitCount / 1024 ) + 1,1,1);
	}
	else
	{
		emitTimeCounter += deltaTime_;
	}

	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,emitDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress());//u1
		lComputeCommandList->SetComputeRootDescriptorTable(5,drawListBuffer->GetUAVAddress());//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( emitData.maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBasicGPUParticleDrawArgumentUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress());//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void BasicGPUParticle::Finalize(){}

void BasicGPUParticle::Draw(const AliceMathF::Matrix4& viewMat_,const AliceMathF::Matrix4& projectionMat_)
{
	{
		ViewProjection lViewProjection;

		lViewProjection.view = viewMat_;
		lViewProjection.projection = projectionMat_;

		viewProjectionBuffer->Update(&lViewProjection);
	}

	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("BasicGPUParticleDraw",AdaptersIndex::MAIN);

	lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
	lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
	lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lGraphicCommandList->SetGraphicsRootConstantBufferView(0,viewProjectionBuffer->GetAddress());

	lGraphicCommandList->SetGraphicsRootDescriptorTable(1,particlePoolBuffer->GetAddress());
	lGraphicCommandList->SetGraphicsRootDescriptorTable(2,drawListBuffer->GetUAVAddress());

	lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(),0,nullptr,0);

	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

}

void BasicGPUParticle::SetSetting()
{
	emitDataBuffer->Update(&emitData);
	particleConstantsBuffer->Update(&particleConstant);
}

void BasicGPUParticle::ADD(const AliceMathF::Vector3& pos_,const BasicGPUParticleSetting& setting_)
{
	static_cast< void >( pos_ );

	particleConstant.accel = setting_.acceleration;
	particleConstant.endColor = setting_.endColor;
	particleConstant.lifeTime = setting_.LifeTime;
	particleConstant.rotation = setting_.rotation;
	particleConstant.startColor = setting_.startColor;
	particleConstant.startPosition = setting_.emitRadius;
	particleConstant.scale = setting_.scale;
	particleConstant.rotation = setting_.rotation;
	particleConstant.velocity = setting_.velocity;

	emitData.maxParticles = setting_.maxParticles;
	emitData.emitCount = setting_.emitCount;

	emitData.maxParticles = setting_.maxParticles;

	emitLifeTime = setting_.emitLifeTime;
	emitTimeCounter = timeBetweenEmit = setting_.timeBetweenEmit;
}

void BasicGPUParticle::PBufferCreate()
{
	particlePoolBuffer = CreateUniqueUAVRWStructuredBuffer(emitData.maxParticles,sizeof(ParticleData),AdaptersIndex::MAIN,HEAP_TYPE::DEFAULT);

	drawListBuffer = CreateUniqueDrawListBuffer(emitData.maxParticles,sizeof(uint32_t),BufferType::MAIN);
	freeListBuffer = CreateUniqueFreeListBuffer(emitData.maxParticles,sizeof(uint32_t),BufferType::MAIN);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::MAIN);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstant),AdaptersIndex::MAIN);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstants),AdaptersIndex::MAIN);
	emitDataBuffer = CreateUniqueConstantBuffer(sizeof(EmitData),AdaptersIndex::MAIN);
	viewProjectionBuffer = CreateUniqueConstantBuffer(sizeof(ViewProjection),AdaptersIndex::MAIN);
}

void BasicGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	time.deltaTime = deltaTime_;
	time.totalTime += deltaTime_;

	timeConstantsBuffer->Update(&time);
}
