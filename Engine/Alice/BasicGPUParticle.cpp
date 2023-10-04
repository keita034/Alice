#include "BasicGPUParticle.h"
#include<BaseBuffer.h>

void BasicGPUParticle::Initialize()
{
	freeListBuffer = CreateUniqueUAVRWStructuredBuffer(particleMaxCount,sizeof(uint32_t),HEAP_TYPE::DEFAULT);
	particlePoolBuffer = CreateUniqueUAVRWStructuredBuffer(particleMaxCount,sizeof(ParticleData),HEAP_TYPE::DEFAULT);
	drawListBuffer = CreateUniqueUAVRWStructuredBuffer(particleMaxCount,sizeof(uint32_t),HEAP_TYPE::DEFAULT);

	viewProjectionBuffer = CreateUniqueConstantBuffer(sizeof(ViewProjection));
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstants));

	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(particleEmitterMaxCount,sizeof(IndirectCommand));
	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);


	D3D12_INDIRECT_ARGUMENT_DESC arg[ 2 ];
	memset(arg,0,sizeof(arg));
	arg[ 0 ].Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT;
	arg[ 0 ].Constant.RootParameterIndex = 0;
	arg[ 0 ].Constant.DestOffsetIn32BitValues = 0;
	arg[ 0 ].Constant.Num32BitValuesToSet = sizeof(AliceMathF::Matrix4) / sizeof(float); // 16
	arg[ 1 ].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	D3D12_COMMAND_SIGNATURE_DESC  desc;
	memset(&desc,0,sizeof(desc));
	desc.ByteStride = sizeof(IndirectCommand);
	desc.pArgumentDescs = arg;
	desc.NumArgumentDescs = 2;

	IRootSignature* lRootSignature = pipelineSet[ "BasicGPUParticleDraw" ]->rootSignature;

	device->Get()->CreateCommandSignature(&desc,lRootSignature->GetRootSignature(),IID_PPV_ARGS(&particleCommandSignature));

}

void BasicGPUParticle::Update(float deltaTime_)
{
	ID3D12GraphicsCommandList* lCommandList = commandList->GetComputeCommandList();

	emitTimeCounter += deltaTime_;

	PUpdateConstantBuffer(deltaTime_);

	//生成
	if ( emitTimeCounter > timeBetweenEmit )
	{
		IComputePipelineState* lPipelineState = pipelineSet[ "BasicGPUParticleEmit" ]->pipelineState;
		IRootSignature* lRootSignature = pipelineSet[ "BasicGPUParticleEmit" ]->rootSignature;

		lCommandList->SetPipelineState(lPipelineState->GetPipelineState());
		lCommandList->SetComputeRootSignature(lRootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap()->GetHeap() };
		lCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());

		lCommandList->SetComputeRootDescriptorTable(1,particlePoolBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(2,freeListBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(3,drawListBuffer->GetAddress());

		lCommandList->Dispatch(static_cast<UINT>(emitCount),1,1);
	}

	//更新
	{
		IComputePipelineState* lPipelineState = pipelineSet[ "BasicGPUParticleUpdate" ]->pipelineState;
		IRootSignature* lRootSignature = pipelineSet[ "BasicGPUParticleUpdate" ]->rootSignature;

		lCommandList->SetPipelineState(lPipelineState->GetPipelineState());
		lCommandList->SetComputeRootSignature(lRootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap()->GetHeap() };
		lCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());

		lCommandList->SetComputeRootDescriptorTable(1,particlePoolBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(2,freeListBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(3,drawListBuffer->GetAddress());

		lCommandList->Dispatch(static_cast< UINT >( particleMaxCount),1,1);

	}

	//ドローアギュメント更新
	{
		IComputePipelineState* lPipelineState = pipelineSet[ "BasicGPUParticleDrawArgumentUpdate" ]->pipelineState;
		IRootSignature* lRootSignature = pipelineSet[ "BasicGPUParticleDrawArgumentUpdate"]->rootSignature;

		lCommandList->SetPipelineState(lPipelineState->GetPipelineState());
		lCommandList->SetComputeRootSignature(lRootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap()->GetHeap() };
		lCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetAddress());
		lCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress());

		lCommandList->Dispatch(1,1,1);

	}
}

void BasicGPUParticle::Finalize()
{
}

void BasicGPUParticle::Draw()
{
	ID3D12GraphicsCommandList* lCommandList = commandList->GetComputeCommandList();

	IComputePipelineState* lPipelineState = pipelineSet[ "BasicGPUParticleDraw" ]->pipelineState;
	IRootSignature* lRootSignature = pipelineSet[ "BasicGPUParticleDraw" ]->rootSignature;

	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);

	lCommandList->SetPipelineState(lPipelineState->GetPipelineState());
	lCommandList->SetGraphicsRootSignature(lRootSignature->GetRootSignature());

	lCommandList->SetGraphicsRootConstantBufferView(1,timeConstantsBuffer->GetAddress());
	lCommandList->SetGraphicsRootConstantBufferView(2,viewProjectionBuffer->GetAddress());
	lCommandList->SetGraphicsRootConstantBufferView(3,timeConstantsBuffer->GetAddress());

	lCommandList->SetGraphicsRootDescriptorTable(4,particlePoolBuffer->GetAddress());
	lCommandList->SetGraphicsRootDescriptorTable(5,drawListBuffer->GetAddress());

	lCommandList->ExecuteIndirect(particleCommandSignature.Get(),static_cast< UINT >( positions.size()),drawArgumentBuffer->GetResource(),0,nullptr,0);
}

void BasicGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	time.deltaTime = deltaTime_;
	time.totalTime += deltaTime_;

	timeConstantsBuffer->Update(&time);

	viewProjectionBuffer->Update(&viewProjectionBuffer);
}
