#include "LaserGPUParticle.h"

void LaserGPUParticle::Initialize()
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

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void LaserGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();
		//生成
	if ( PCanEmit(emitData,deltaTime_) )
	{
		fireGPUParticleGPUData.emitDataIndex = emitData.index;
		fireGPUParticleDataBuffer->Update(&fireGPUParticleGPUData);

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleEmit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,fireGPUParticleDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}

//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleUpdate",AdaptersIndex::SUB);

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
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeLaserGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void LaserGPUParticle::Finalize()
{
	mainTexture = nullptr;
	subTexture = nullptr;
}

void LaserGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	{
		worldBillboardGPUData.worldMat = worldMat_;
		worldBillboardGPUData.billboardMat = billboardMat_;

		worldBillboardBuffer->Update(&worldBillboardGPUData);
	}

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("LaserGPUParticleDraw",AdaptersIndex::MAIN);

	lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
	lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

	ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
	lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

	lGraphicCommandList->SetGraphicsRootConstantBufferView(0,worldBillboardBuffer->GetAddress());

	lGraphicCommandList->SetGraphicsRootDescriptorTable(1,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::SUB));
	lGraphicCommandList->SetGraphicsRootDescriptorTable(2,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::SUB));

	lGraphicCommandList->SetGraphicsRootDescriptorTable(3,mainTexture->gpuHandle);
	lGraphicCommandList->SetGraphicsRootDescriptorTable(4,subTexture->gpuHandle);

	lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(CrossAdapterResourceIndex::SUB),0,nullptr,0);
}

void LaserGPUParticle::SetSetting()
{
}

void LaserGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

void LaserGPUParticle::Emit(const LaserGPUParticleSetting& setting_)
{

		emitData.velocity = particleConstant.velocity = setting_.velocity;
		emitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
		emitData.position = particleConstant.position = setting_.position;
		emitData.maxParticles = particleConstant.maxParticles = setting_.maxParticles;
		emitData.startColor = particleConstant.startColor = setting_.startColor;
		emitData.endColor = particleConstant.endColor = setting_.endColor;
		emitData.speed = particleConstant.speed = setting_.speed;
		emitData.maxSize = particleConstant.maxSize = setting_.maxSize;
		emitData.maxSizeTime = particleConstant.maxSizeTime = setting_.maxSizeTime;

		emitData.timeBetweenEmit = setting_.timeBetweenEmit;
		emitData.index = 0;
		emitData.isPlay = setting_.isPlay;

		particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData) );
}

void LaserGPUParticle::Move(const AliceMathF::Vector3& move_)
{
	emitData.position += move_;

	particleConstant.position = emitData.position;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData) );
}

void LaserGPUParticle::SetPos(const AliceMathF::Vector3& pos_)
{
	particleConstant.position = emitData.position = pos_;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

}

void LaserGPUParticle::SetVelocity(const AliceMathF::Vector3& velocity_)
{
	particleConstant.velocity = emitData.velocity = velocity_;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void LaserGPUParticle::EmitPlay()
{
	if ( !emitData.isPlay )
	{
		emitData.isPlay = true;
		emitData.emitTimeCounter = 0.0f;
	}
}

void LaserGPUParticle::EmitStop()
{

	if ( emitData.isPlay )
	{
		emitData.isPlay = false;
	}
}

float LaserGPUParticle::GetDeltaTime()
{
	return timeGPUData.deltaTime;
}

void LaserGPUParticle::SetMainTex(uint32_t textureHandle_)
{
	mainTexture = TextureManager::SGetTextureData(textureHandle_);
}

void LaserGPUParticle::SetSubTex(uint32_t textureHandle_)
{
	subTexture = TextureManager::SGetTextureData(textureHandle_);
}

void LaserGPUParticle::PBufferCreate()
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

void LaserGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	timeGPUData.deltaTime = deltaTime_;
	timeGPUData.totalTime += deltaTime_;
	timeGPUData.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	timeConstantsBuffer->Update(&timeGPUData);
}

bool LaserGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
{
	if ( data_.isPlay )
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

	return false;
}