#include "BloodGushGPUParticle.h"

void BloodGushGPUParticle::Initialize()
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

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	//フリーリスト初期化
	{

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void BloodGushGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	//生成
	if ( PCanEmit(emitData,deltaTime_) )
	{
		fireGPUParticleGPUData.emitDataIndex = emitData.index;
		fireGPUParticleDataBuffer->Update(&fireGPUParticleGPUData);

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleEmit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1
		lComputeCommandList->SetComputeRootDescriptorTable(5,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( emitData.emitCount / 1024 ) + 1,1,1);
	}


	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1
		lComputeCommandList->SetComputeRootDescriptorTable(5,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeBloodGushGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void BloodGushGPUParticle::Finalize()
{
	texture = nullptr;
}

void BloodGushGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	{
		worldBillboardGPUData.worldMat = worldMat_;
		worldBillboardGPUData.billboardMat = billboardMat_;

		worldBillboardBuffer->Update(&worldBillboardGPUData);
	}

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("BloodGushGPUParticleDraw",AdaptersIndex::MAIN);

	lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
	lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
	lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lGraphicCommandList->SetGraphicsRootConstantBufferView(0,worldBillboardBuffer->GetAddress());

	lGraphicCommandList->SetGraphicsRootDescriptorTable(1,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::SUB));
	lGraphicCommandList->SetGraphicsRootDescriptorTable(2,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::SUB));

	lGraphicCommandList->SetGraphicsRootDescriptorTable(3,texture->gpuHandle);

	lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(CrossAdapterResourceIndex::SUB),0,nullptr,0);
}

void BloodGushGPUParticle::SetSetting()
{
}

void BloodGushGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

void BloodGushGPUParticle::Emit(const BloodGushGPUParticleSetting& setting_)
{

		emitData.endColor = particleConstant.endColor = setting_.endColor;
		emitData.startColor = particleConstant.startColor = setting_.startColor;
		emitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
		emitData.size = particleConstant.size = setting_.size;
		emitData.speed = particleConstant.speed = setting_.speed;
		emitData.emitCount = particleConstant.emitCount = static_cast< uint32_t >( 20 * setting_.amount );
		emitData.accel = particleConstant.accel = setting_.accel;

		emitData.timeBetweenEmit = setting_.timeBetweenEmit;
		emitData.isPlay = setting_.isPlay;

		emitData.emitLifeTime = emitData.emitMaxLifeTime = setting_.emitLifeTime;
		emitData.index = 0;

		particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

}

void BloodGushGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void BloodGushGPUParticle::EmitPlay(const AliceMathF::Vector3& pos_,const AliceMathF::Vector3& velocity_)
{
	if ( !emitData.isPlay )
	{
		emitData.isPlay = true;
		emitData.emitLifeTime = emitData.emitMaxLifeTime;
		emitData.position = pos_;
		emitData.velocity = particleConstant.velocity = velocity_;
		emitData.position = particleConstant.position = pos_;

		particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
	}
}

void BloodGushGPUParticle::EmitStop()
{
	if ( emitData.isPlay )
	{
		emitData.isPlay = false;
	}
}

void BloodGushGPUParticle::PBufferCreate()
{
	particlePoolBuffer = CreateUniqueCrossAdapterBuffer(maxParticles,sizeof(ParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);

	drawListBuffer = CreateUniqueDrawListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	freeListBuffer = CreateUniqueFreeListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUDatas),AdaptersIndex::SUB);
	fireGPUParticleDataBuffer = CreateUniqueConstantBuffer(sizeof(FireGPUParticleGPUData),AdaptersIndex::SUB);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstantGPUData),AdaptersIndex::SUB);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);

	fireGPUParticleDataBuffer->Update(&fireGPUParticleGPUData);
}

void BloodGushGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	timeGPUData.deltaTime = deltaTime_;
	timeGPUData.totalTime += deltaTime_;
	timeGPUData.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	timeConstantsBuffer->Update(&timeGPUData);
}

bool BloodGushGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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
			else
			{
				data_.isPlay = false;
			}
		}
	}

	return false;
}
