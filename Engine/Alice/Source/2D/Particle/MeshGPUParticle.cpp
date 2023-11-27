#include "MeshGPUParticle.h"

#include<AliceAssert.h>

void MeshGPUParticle::Initialize()
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

	modelData = std::make_unique<MeshGPUParticleAliceModel>();

	//フリーリスト初期化

	ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetComputeCommandList();

	{

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleFreeListInit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress());//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleEmit",AdaptersIndex::MAIN);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleUpdate",AdaptersIndex::MAIN);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleDrawArgumentUpdate",AdaptersIndex::MAIN);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void MeshGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetComputeCommandList();

	for ( ParticleEmit& emitData : emitDatas )
	{
		//生成
		if ( PCanEmit(emitData,deltaTime_) )
		{
			if ( determineTexture->resourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE )
			{
				graphicAdapter->ResourceTransition(determineTexture->texBuff.Get(),determineTexture->resourceState,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				determineTexture->resourceState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

			}

			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				fireGPUParticleGPUData.emitDataIndex = emitData.index;
				gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

				ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleEmit",AdaptersIndex::MAIN);

				lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
				lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

				ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
				lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

				lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
				lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
				lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
				lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->constBoneBuffer->GetAddress());//b2

				lComputeCommandList->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress());//u0
				lComputeCommandList->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress());//u1

				lComputeCommandList->SetComputeRootDescriptorTable(6,determineTexture->gpuHandle);//t0
				lComputeCommandList->SetComputeRootDescriptorTable(7,mesh->GetVertexSRVAddress());//t1
				lComputeCommandList->SetComputeRootDescriptorTable(8,mesh->GetIndicesSRVAddress());//t2

				lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);
			}
		}
	}


	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress());//u1
		lComputeCommandList->SetComputeRootDescriptorTable(5,drawListBuffer->GetUAVAddress());//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleDrawArgumentUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress());//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress());//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void MeshGPUParticle::Finalize()
{

}

void MeshGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	AliceAssertNull(texture,"テクスチャがありません");

	{
		worldBillboardGPUData.worldMat = worldMat_;
		worldBillboardGPUData.billboardMat = billboardMat_;

		worldBillboardBuffer->Update(&worldBillboardGPUData);
	}

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	drawArgumentBuffer->Transition(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);

	Material* lMaterial = MaterialManager::SGetMaterial("MeshGPUParticleDraw",AdaptersIndex::MAIN);

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

void MeshGPUParticle::SetSetting()
{
}

void MeshGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

int32_t MeshGPUParticle::Emit(const MeshGPUParticleSetting& setting_,int32_t index_)
{
	if ( index_ == -1 )
	{
		ParticleEmit lEmitData;
		ParticleConstantGPUData particleConstant;

		lEmitData.velocity = particleConstant.velocity = setting_.velocity;
		lEmitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
		lEmitData.matWorld = particleConstant.matWorld = setting_.matWorld;
		lEmitData.maxParticles = particleConstant.maxParticles = setting_.maxParticles;
		lEmitData.startColor = particleConstant.startColor = setting_.startColor;
		lEmitData.endColor = particleConstant.endColor = setting_.endColor;
		lEmitData.speed = particleConstant.speed = setting_.speed;
		lEmitData.size = particleConstant.size = setting_.size;

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

void MeshGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_,int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	particleConstants[ lIndex ].matWorld = emitDatas[ lIndex ].matWorld = matWorld_;

	particleConstantsBuffer->Update(particleConstants.data(),sizeof(ParticleConstantGPUData) * particleConstants.size());

}

void MeshGPUParticle::SetDetermineTex(uint32_t textureHandle_)
{
	determineTexture = TextureManager::SGetTextureData(textureHandle_);
}

void MeshGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void MeshGPUParticle::EmitPlay(int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	if ( !emitDatas[ lIndex ].isPlay )
	{
		emitDatas[ lIndex ].isPlay = true;
		emitDatas[ lIndex ].emitTimeCounter = 0.0f;
	}
}

void MeshGPUParticle::EmitStop(int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	if ( emitDatas[ lIndex ].isPlay )
	{
		emitDatas[ lIndex ].isPlay = false;
	}
}

void MeshGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::MAIN);
}

float MeshGPUParticle::GetDeltaTime()
{
	return timeGPUData.deltaTime;
}

void MeshGPUParticle::PBufferCreate()
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
	gpuParticleDataBuffer = CreateUniqueConstantBuffer(sizeof(FireGPUParticleGPUData),AdaptersIndex::MAIN);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstantGPUData),AdaptersIndex::MAIN);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);

	gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

}

void MeshGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	timeGPUData.deltaTime = deltaTime_;
	timeGPUData.totalTime += deltaTime_;
	timeGPUData.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	timeConstantsBuffer->Update(&timeGPUData);
}

bool MeshGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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
