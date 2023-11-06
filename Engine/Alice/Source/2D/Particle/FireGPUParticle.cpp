#include "FireGPUParticle.h"

#include<DescriptorHeap.h>

void FireGPUParticle::Initialize()
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

	//フリーリスト初期化
	{
		ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetComputeCommandList();

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeFireGPUParticleFreeListInit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress());//u0
		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}
}

void FireGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetComputeCommandList();

	for ( ParticleEmit& emitData : emitDatas )
	{
		//生成
		if ( PCanEmit(emitData,deltaTime_) )
		{
			fireGPUParticleGPUData.emitDataIndex = emitData.index;
			fireGPUParticleDataBuffer->Update(&fireGPUParticleGPUData);

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeFireGPUParticleEmit",AdaptersIndex::MAIN);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
			lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b1
			lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

			lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress());//u0
			lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress());//u1

			lComputeCommandList->Dispatch(static_cast< UINT >( emitData.emitCount / 1024 ) + 1,1,1);
		}
	}

	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeFireGPUParticleUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress());//u1
		lComputeCommandList->SetComputeRootDescriptorTable(5,drawListBuffer->GetUAVAddress());//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeFireGPUParticleDrawArgumentUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress());//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void FireGPUParticle::Finalize()
{
	texture = nullptr;
}

void FireGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	{
		worldBillboardGPUData.worldMat = worldMat_;
		worldBillboardGPUData.billboardMat = billboardMat_;

		worldBillboardBuffer->Update(&worldBillboardGPUData);
	}

	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("FireGPUParticleDraw",AdaptersIndex::MAIN);

	lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
	lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
	lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lGraphicCommandList->SetGraphicsRootConstantBufferView(0,worldBillboardBuffer->GetAddress());

	lGraphicCommandList->SetGraphicsRootDescriptorTable(1,particlePoolBuffer->GetAddress());
	lGraphicCommandList->SetGraphicsRootDescriptorTable(2,drawListBuffer->GetUAVAddress());
	lGraphicCommandList->SetGraphicsRootDescriptorTable(3,texture->gpuHandle);

	lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(),0,nullptr,0);

	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void FireGPUParticle::SetSetting()
{
}

void FireGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

int32_t FireGPUParticle::Emit(const FireGPUParticleSetting& setting_,int32_t index_)
{
	if ( index_ == -1 )
	{
		ParticleEmit lEmitData;
		ParticleConstantGPUData particleConstant;

		lEmitData.convergePointPosition = particleConstant.convergePointPosition = setting_.convergePointPosition;
		lEmitData.radius = particleConstant.radius = setting_.radius;
		lEmitData.position = particleConstant.position = setting_.position;
		lEmitData.maxParticles = particleConstant.maxParticles = setting_.maxParticles;
		lEmitData.endColor = particleConstant.endColor = setting_.endColor;
		lEmitData.startColor = particleConstant.startColor = setting_.startColor;
		lEmitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
		lEmitData.size = particleConstant.size = setting_.size;
		lEmitData.speed = particleConstant.speed = setting_.speed;

		lEmitData.emitCount = setting_.emitCount;
		lEmitData.emitLifeTime = lEmitData.emitMaxLifeTime = setting_.emitLifeTime;
		lEmitData.timeBetweenEmit = setting_.timeBetweenEmit;
		lEmitData.index = static_cast< uint32_t >( emitDatas.size() );
		lEmitData.isPlay = setting_.isPlay;

		emitDatas.push_back(lEmitData);
		particleConstants.push_back(particleConstant);

		particleConstantsBuffer->Update(particleConstants.data(),( sizeof(ParticleConstantGPUData) * particleConstants.size() ));

		return static_cast< int32_t >( emitDatas.size() - 1 );

	}
	else
	{
		return -1;
	}

}

void FireGPUParticle::Move(const AliceMathF::Vector3& move_,int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	emitDatas[ lIndex ].position += move_;

	particleConstants[ lIndex ].position = emitDatas[ lIndex ].position;

	particleConstantsBuffer->Update(particleConstants.data(),sizeof(ParticleConstantGPUData) * particleConstants.size());
}

void FireGPUParticle::SetPos(const AliceMathF::Vector3& pos_,int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	particleConstants[ lIndex ].position = emitDatas[ lIndex ].position = pos_;

	particleConstantsBuffer->Update(particleConstants.data(),sizeof(ParticleConstantGPUData) * particleConstants.size());
}

void FireGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void FireGPUParticle::EmitPlay(int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	if ( !emitDatas[ lIndex ].isPlay )
	{
		emitDatas[ lIndex ].isPlay = true;
		emitDatas[ lIndex ].emitLifeTime = emitDatas[ lIndex ].emitMaxLifeTime;
		emitDatas[ lIndex ].emitTimeCounter = 0.0f;
	}
}

void FireGPUParticle::EmitStop(int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	if ( emitDatas[ lIndex ].isPlay )
	{
		emitDatas[ lIndex ].isPlay = false;
	}
}

void FireGPUParticle::PBufferCreate()
{
	//particlePoolBuffer = CreateUniqueCrossAdapterBuffer(maxParticles,sizeof(ParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);
	//drawListBuffer = CreateUniqueDrawListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	//freeListBuffer = CreateUniqueFreeListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	//drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);

	//particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUDatas),AdaptersIndex::SUB);
	//fireGPUParticleDataBuffer = CreateUniqueConstantBuffer(sizeof(FireGPUParticleGPUData),AdaptersIndex::SUB);
	//timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstantGPUData),AdaptersIndex::SUB);
	//worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);

	particlePoolBuffer = CreateUniqueUAVRWStructuredBuffer(maxParticles,sizeof(ParticleGPUData),AdaptersIndex::MAIN,HEAP_TYPE::DEFAULT);
	drawListBuffer = CreateUniqueDrawListBuffer(maxParticles,sizeof(uint32_t),BufferType::MAIN);
	freeListBuffer = CreateUniqueFreeListBuffer(maxParticles,sizeof(uint32_t),BufferType::MAIN);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::MAIN);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUDatas),AdaptersIndex::MAIN);
	fireGPUParticleDataBuffer = CreateUniqueConstantBuffer(sizeof(FireGPUParticleGPUData),AdaptersIndex::MAIN);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstantGPUData),AdaptersIndex::MAIN);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);

	fireGPUParticleDataBuffer->Update(&fireGPUParticleGPUData);
}

void FireGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	timeGPUData.deltaTime = deltaTime_;
	timeGPUData.totalTime += deltaTime_;
	timeGPUData.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	timeConstantsBuffer->Update(&timeGPUData);
}

bool FireGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
{
	if ( data_.isPlay )
	{
		if ( data_.emitLifeTime >= 0 )
		{
			if ( data_.emitTimeCounter <= 0.0f )
			{
				data_.emitTimeCounter = data_.timeBetweenEmit;
				return true;
			}
			else
			{
				data_.emitTimeCounter -= deltaTime_;
				data_.emitLifeTime -= deltaTime_;
			}
		}
		else
		{
			if ( data_.emitLifeTime == -1 )
			{
				if ( data_.emitTimeCounter <= 0.0f )
				{
					data_.emitTimeCounter = data_.timeBetweenEmit;

					return true;
				}
				else
				{
					data_.emitTimeCounter -= deltaTime_;
				}
			}
		}
	}

	return false;
}