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

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	{

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void MeshGPUParticle::Update(float deltaTime_)
{

	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();


		//生成
	if ( PCanEmit(emitData,deltaTime_) )
	{
		if ( determineTexture->resourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE )
		{
			computeAdapter->ResourceTransition(determineTexture->texBuff.Get(),determineTexture->resourceState,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
			determineTexture->resourceState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

		}

		for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
		{
			fireGPUParticleGPUData.emitDataIndex = emitData.index;
			gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

			particleConstant.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
			particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleEmit",AdaptersIndex::SUB);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
			lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
			lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
			lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->postureMatBuff->GetAddress());//b3

			lComputeCommandList->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
			lComputeCommandList->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

			lComputeCommandList->SetComputeRootDescriptorTable(6,mesh->GetVertexSRVAddress());//t1

			lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);
		}
	}


//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
		lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
		lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1
		lComputeCommandList->SetComputeRootDescriptorTable(5,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

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

	Material* lMaterial = MaterialManager::SGetMaterial("MeshGPUParticleDraw",AdaptersIndex::MAIN);

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

void MeshGPUParticle::SetSetting()
{
}

void MeshGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

void MeshGPUParticle::Emit(const MeshGPUParticleSetting& setting_)
{
		emitData.velocity = particleConstant.velocity = setting_.velocity;
		emitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
		emitData.matWorld = particleConstant.matWorld = setting_.matWorld;
		emitData.maxParticles = particleConstant.maxParticles = setting_.maxParticles;
		emitData.startColor = particleConstant.startColor = setting_.startColor;
		emitData.endColor = particleConstant.endColor = setting_.endColor;
		emitData.speed = particleConstant.speed = setting_.speed;
		emitData.size = particleConstant.size = setting_.size;

		emitData.timeBetweenEmit = setting_.timeBetweenEmit;
		emitData.index = 0;
		emitData.isPlay = setting_.isPlay;
		emitData.isInfinityEmit = setting_.isInfinityEmit;

		particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void MeshGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_)
{
	particleConstant.matWorld = emitData.matWorld = matWorld_;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void MeshGPUParticle::SetDetermineTex(uint32_t textureHandle_)
{
	determineTexture = TextureManager::SGetTextureData(textureHandle_);
}

void MeshGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void MeshGPUParticle::PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_)
{
	particleConstant.vertexSize = static_cast< uint32_t >( boneMesh_->vertices.size() );
	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

	computeCommandList_->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
	computeCommandList_->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
	computeCommandList_->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
	computeCommandList_->SetComputeRootConstantBufferView(3,mesh_->postureMatBuff->GetAddress());//b3

	computeCommandList_->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
	computeCommandList_->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

	computeCommandList_->SetComputeRootDescriptorTable(6,boneMesh_->vertexBuffer->GetSRVAddress());//t1

	computeCommandList_->Dispatch(static_cast< uint32_t >( boneMesh_->vertices.size() / 1024 + 1 ),1,1);

	if ( root_ )
	{
		for ( BoneMesh* mesh : boneMesh_->parent )
		{
			PReadChildren(computeCommandList_,mesh_,mesh,root_);
		}
	}
}

void MeshGPUParticle::EmitPlay(bool flag_)
{
	if ( !emitData.isPlay )
	{
		emitData.isPlay = true;

		if ( flag_ )
		{

			emitData.emitTimeCounter = emitData.timeBetweenEmit;

			BaseGPUParticle::ParticleBegin();

			ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

			if ( determineTexture->resourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE )
			{
				computeAdapter->ResourceTransition(determineTexture->texBuff.Get(),determineTexture->resourceState,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				determineTexture->resourceState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

			}

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeMeshGPUParticleEmit",AdaptersIndex::SUB);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				fireGPUParticleGPUData.emitDataIndex = emitData.index;
				gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

				if ( boneMesh )
				{
					PReadChildren(lComputeCommandList,mesh.get(),boneMesh,boneMeshRoot);
				}
				else
				{
					particleConstant.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
					particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

					lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
					lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
					lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
					lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->postureMatBuff->GetAddress());//b3

					lComputeCommandList->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
					lComputeCommandList->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

					lComputeCommandList->SetComputeRootDescriptorTable(6,mesh->GetVertexSRVAddress());//t1

					lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);
				}

			}

			BaseGPUParticle::ParticleEnd();
		}
		else
		{
			emitData.emitTimeCounter = 0.0f;
		}
	}
}

void MeshGPUParticle::EmitStop()
{
	if ( emitData.isPlay )
	{
		emitData.isPlay = false;
	}
}

void MeshGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::SUB);
}

void MeshGPUParticle::SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	boneMesh = modelData->GetBoneMesh(meshName_,boneName_);
	boneMeshRoot = root_;
}

float MeshGPUParticle::GetDeltaTime() const
{
	return timeGPUData.deltaTime;
}

void MeshGPUParticle::PBufferCreate()
{
	particlePoolBuffer = CreateUniqueCrossAdapterBuffer(maxParticles,sizeof(ParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawListBuffer = CreateUniqueDrawListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	freeListBuffer = CreateUniqueFreeListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUDatas),AdaptersIndex::SUB);
	gpuParticleDataBuffer = CreateUniqueConstantBuffer(sizeof(FireGPUParticleGPUData),AdaptersIndex::SUB);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstantGPUData),AdaptersIndex::SUB);
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
		if ( data_.isInfinityEmit )
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
			if ( data_.emitTimeCounter <= 0.0f )
			{
				data_.emitTimeCounter = data_.timeBetweenEmit;

				return true;
			}
			else
			{
				data_.emitTimeCounter -= deltaTime_;
				data_.emitLifeTime -= deltaTime_;

				if ( data_.emitLifeTime <= 0.0f )
				{
					data_.isPlay = false;
				}
			}
		}


	}

	return false;
}
