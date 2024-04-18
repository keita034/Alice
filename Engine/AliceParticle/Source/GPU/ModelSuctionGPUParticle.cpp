#include "ModelSuctionGPUParticle.h"
#include"AliceAssert.h"

void ModelSuctionGPUParticle::Initialize()
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

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,particleConstantsBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void ModelSuctionGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	if ( !flag )
	{
		flag = true;

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,particleConstantsBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//生成
	if ( PCanEmit(emitData,deltaTime_) )
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleEmit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
		{
			if ( boneMesh )
			{
				PReadChildren(lComputeCommandList,mesh.get(),boneMesh,boneMeshRoot);
			}
			else
			{
				particleConstant.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
				particleConstantsBuffer->Update(&particleConstant);

				lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
				lComputeCommandList->SetComputeRootConstantBufferView(1,mesh->postureMatBuff->GetAddress());//b3

				lComputeCommandList->SetComputeRootDescriptorTable(2,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
				lComputeCommandList->SetComputeRootDescriptorTable(3,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

				lComputeCommandList->SetComputeRootDescriptorTable(4,mesh->GetVertexSRVAddress());//t1

				lComputeCommandList->Dispatch(static_cast< uint32_t >( emitData.emitSize / 1024 + 1 ),1,1);
			}

		}

	}


	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b0

		lComputeCommandList->SetComputeRootDescriptorTable(1,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(2,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1
		lComputeCommandList->SetComputeRootDescriptorTable(3,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void ModelSuctionGPUParticle::Finalize()
{
}

void ModelSuctionGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	AliceAssertNull(texture,"テクスチャがありません");

	{
		worldBillboardGPUData.worldMat = worldMat_;
		worldBillboardGPUData.billboardMat = billboardMat_;

		worldBillboardBuffer->Update(&worldBillboardGPUData);
	}

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("ModelSuctionGPUParticleDraw",AdaptersIndex::MAIN);

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

void ModelSuctionGPUParticle::Create(uint32_t maxParticles_)
{
	particleConstant.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

void ModelSuctionGPUParticle::Emit(const ModelSuctionGPUParticleSetting& setting_)
{
	emitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
	emitData.matWorld = particleConstant.matWorld = setting_.matWorld;
	emitData.maxParticles = particleConstant.maxParticles = setting_.maxParticles;
	emitData.startColor = particleConstant.startColor = setting_.startColor;
	emitData.endColor = particleConstant.endColor = setting_.endColor;
	emitData.size = particleConstant.size = setting_.size;
	emitData.emitSize = particleConstant.emitSize = setting_.emitSize;

	emitData.timeBetweenEmit = setting_.timeBetweenEmit;
	emitData.index = 0;
	emitData.isPlay = setting_.isPlay;
	emitData.isInfinityEmit = setting_.isInfinityEmit;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void ModelSuctionGPUParticle::EmitStop()
{
	if ( emitData.isPlay )
	{
		emitData.isPlay = false;
	}
}

void ModelSuctionGPUParticle::EmitPlay(bool flag_)
{
	if ( !emitData.isPlay )
	{
		emitData.isPlay = true;

		if ( flag_ )
		{

			emitData.emitTimeCounter = emitData.timeBetweenEmit;

			BaseGPUParticle::ParticleBegin();

			ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelSuctionGPUParticleEmit",AdaptersIndex::SUB);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				if ( boneMesh )
				{
					PReadChildren(lComputeCommandList,mesh.get(),boneMesh,boneMeshRoot);
				}
				else
				{
					particleConstant.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
					particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

					lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
					lComputeCommandList->SetComputeRootConstantBufferView(1,mesh->postureMatBuff->GetAddress());//b3

					lComputeCommandList->SetComputeRootDescriptorTable(2,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
					lComputeCommandList->SetComputeRootDescriptorTable(3,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

					lComputeCommandList->SetComputeRootDescriptorTable(4,mesh->GetVertexSRVAddress());//t1

					lComputeCommandList->Dispatch(static_cast< uint32_t >( emitData.emitSize / 1024 + 1 ),1,1);
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

float ModelSuctionGPUParticle::GetDeltaTime() const
{
	return particleConstant.deltaTime;
}

void ModelSuctionGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_)
{
	particleConstant.matWorld = emitData.matWorld = matWorld_;
	particleConstant.pos = AliceMathF::GetWorldPosition(matWorld_);

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void ModelSuctionGPUParticle::SetDetermineTex(uint32_t textureHandle_)
{
	determineTexture = TextureManager::SGetTextureData(textureHandle_);
}

void ModelSuctionGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void ModelSuctionGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::SUB);
}

void ModelSuctionGPUParticle::SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	boneMesh = modelData->GetBoneMesh(meshName_,boneName_);
	boneMeshRoot = root_;
}

void ModelSuctionGPUParticle::PBufferCreate()
{
	particlePoolBuffer = CreateUniqueCrossAdapterBuffer(maxParticles,sizeof(ParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawListBuffer = CreateUniqueDrawListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	freeListBuffer = CreateUniqueFreeListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUData),AdaptersIndex::SUB);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);
}

void ModelSuctionGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	particleConstant.deltaTime = deltaTime_;
	particleConstant.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	particleConstantsBuffer->Update(&particleConstant);
}

bool ModelSuctionGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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

void ModelSuctionGPUParticle::PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_)
{
	particleConstant.vertexSize = static_cast< uint32_t >( boneMesh_->vertices.size() );
	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

	computeCommandList_->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
	computeCommandList_->SetComputeRootConstantBufferView(1,mesh_->postureMatBuff->GetAddress());//b3

	computeCommandList_->SetComputeRootDescriptorTable(2,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
	computeCommandList_->SetComputeRootDescriptorTable(3,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

	computeCommandList_->SetComputeRootDescriptorTable(4,boneMesh_->vertexBuffer->GetSRVAddress());//t1

	computeCommandList_->Dispatch(static_cast< uint32_t >( emitData.emitSize / 1024 + 1 ),1,1);

	if ( root_ )
	{
		for ( BoneMesh* mesh : boneMesh_->parent )
		{
			PReadChildren(computeCommandList_,mesh_,mesh,root_);
		}
	}
}
