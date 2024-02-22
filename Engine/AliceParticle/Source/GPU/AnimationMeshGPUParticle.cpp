#include "AnimationMeshGPUParticle.h"

#include<AliceAssert.h>

void AnimationMeshGPUParticle::Initialize()
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

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void AnimationMeshGPUParticle::Update(float deltaTime_)
{
	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

	for ( ParticleEmit& emitData : emitDatas )
	{
		//生成
		if ( PCanEmit(emitData,deltaTime_) )
		{
			if ( determineTexture->resourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE )
			{
				computeAdapter->ResourceTransition(determineTexture->texBuff.Get(),determineTexture->resourceState,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				determineTexture->resourceState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
			}

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleEmit",AdaptersIndex::SUB);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				BoneData* lBonedata = mesh->bonedata;
				mesh->constBoneBuffer->Update(lBonedata->boneMat.data());

				fireGPUParticleGPUData.emitDataIndex = emitDatas[ emitData.index ].index;
				gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

				if ( boneMesh )
				{
					PReadChildren(lComputeCommandList,emitData.index,mesh.get(),boneMesh,boneMeshRoot);
				}
				else
				{
					particleConstants[ emitDatas[ emitData.index ].index ].vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
					particleConstantsBuffer->Update(particleConstants.data(),( sizeof(ParticleConstantGPUData) * particleConstants.size() ));

					lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
					lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
					lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
					lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->constBoneBuffer->GetAddress());//b2

					lComputeCommandList->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
					lComputeCommandList->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

					lComputeCommandList->SetComputeRootDescriptorTable(6,determineTexture->gpuHandle);//t0
					lComputeCommandList->SetComputeRootDescriptorTable(7,mesh->GetVertexSRVAddress());//t1
					lComputeCommandList->SetComputeRootDescriptorTable(8,mesh->GetIndicesSRVAddress());//t2

					lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);

				}

			}
		}
	}

	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleUpdate",AdaptersIndex::SUB);

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
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void AnimationMeshGPUParticle::Finalize()
{

}

void AnimationMeshGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	AliceAssertNull(texture,"テクスチャがありません");

	{
		worldBillboardGPUData.worldMat = worldMat_;
		worldBillboardGPUData.billboardMat = billboardMat_;

		worldBillboardBuffer->Update(&worldBillboardGPUData);
	}

	ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

	Material* lMaterial = MaterialManager::SGetMaterial("AnimationMeshGPUParticleDraw",AdaptersIndex::MAIN);

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

void AnimationMeshGPUParticle::SetSetting()
{
}

void AnimationMeshGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

int32_t AnimationMeshGPUParticle::Emit(const AnimationMeshGPUParticleSetting& setting_,int32_t index_)
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
		lEmitData.isInfinityEmit = setting_.isInfinityEmit;

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

void AnimationMeshGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_,int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	particleConstants[ lIndex ].matWorld = emitDatas[ lIndex ].matWorld = matWorld_;

	particleConstantsBuffer->Update(particleConstants.data(),sizeof(ParticleConstantGPUData) * particleConstants.size());

}

void AnimationMeshGPUParticle::SetDetermineTex(uint32_t textureHandle_)
{
	determineTexture = TextureManager::SGetTextureData(textureHandle_);
}

void AnimationMeshGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void AnimationMeshGPUParticle::EmitPlay(int32_t index_,bool flag_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	if ( !emitDatas[ lIndex ].isPlay )
	{
		emitDatas[ lIndex ].isPlay = true;

		if ( flag_ )
		{
			emitDatas[ lIndex ].emitTimeCounter = emitDatas[ lIndex ].timeBetweenEmit;

			BaseGPUParticle::ParticleBegin();

			ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

			if ( determineTexture->resourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE )
			{
				computeAdapter->ResourceTransition(determineTexture->texBuff.Get(),determineTexture->resourceState,D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				determineTexture->resourceState = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
			}

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationMeshGPUParticleEmit",AdaptersIndex::SUB);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				BoneData* lBonedata = mesh->bonedata;
				mesh->constBoneBuffer->Update(lBonedata->boneMat.data());

				fireGPUParticleGPUData.emitDataIndex = emitDatas[ lIndex ].index;
				gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

				if ( boneMesh )
				{
					PReadChildren(lComputeCommandList,lIndex,mesh.get(),boneMesh,boneMeshRoot);
				}
				else
				{
					particleConstants[ emitDatas[ lIndex ].index ].vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
					particleConstantsBuffer->Update(particleConstants.data(),( sizeof(ParticleConstantGPUData) * particleConstants.size() ));

					lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
					lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
					lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
					lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->constBoneBuffer->GetAddress());//b2

					lComputeCommandList->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
					lComputeCommandList->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

					lComputeCommandList->SetComputeRootDescriptorTable(6,determineTexture->gpuHandle);//t0
					lComputeCommandList->SetComputeRootDescriptorTable(7,mesh->GetVertexSRVAddress());//t1

					lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);

				}
			}
			BaseGPUParticle::ParticleEnd();
		}
		else
		{
			emitDatas[ lIndex ].emitTimeCounter = 0.0f;
		}
	}
}

void AnimationMeshGPUParticle::EmitStop(int32_t index_)
{
	size_t lIndex = static_cast< size_t >( index_ );

	if ( emitDatas[ lIndex ].isPlay )
	{
		emitDatas[ lIndex ].isPlay = false;
	}
}

void AnimationMeshGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::SUB);
	verticeSize = modelData->GetVerticeSize();
}

void ReadChildren(BoneMesh* boneMesh_,size_t& maxParticles_,bool root_)
{
	maxParticles_ += boneMesh_->vertices.size();

	if ( root_ )
	{
		for ( BoneMesh* mesh : boneMesh_->parent )
		{
			ReadChildren(mesh,maxParticles_,root_);
		}
	}
}

void AnimationMeshGPUParticle::SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	boneMesh = modelData->GetBoneMesh(meshName_,boneName_);
	boneMeshRoot = root_;
	//ReadChildren(boneMesh,verticeSize,boneMeshRoot);
}

const Bone* AnimationMeshGPUParticle::GetBone(const std::string& meshName_,const std::string& boneName_) const
{
	return modelData->GetBoneMesh(meshName_,boneName_)->bone;
}

AliceMathF::Vector3 AnimationMeshGPUParticle::GetMeshBoneCenterPos(const std::string& meshName_,const std::string& boneName_)const
{
	return modelData->GetBoneMesh(meshName_,boneName_)->centerPos;
}

float AnimationMeshGPUParticle::GetDeltaTime() const
{
	return timeGPUData.deltaTime;
}

size_t AnimationMeshGPUParticle::GetVerticeSize() const
{
	return verticeSize;
}

void AnimationMeshGPUParticle::PBufferCreate()
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

void AnimationMeshGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	timeGPUData.deltaTime = deltaTime_;
	timeGPUData.totalTime += deltaTime_;
	timeGPUData.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	timeConstantsBuffer->Update(&timeGPUData);
}

bool AnimationMeshGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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

void AnimationMeshGPUParticle::PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,size_t index_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_)
{
	particleConstants[ emitDatas[ index_ ].index ].vertexSize = static_cast< uint32_t >( boneMesh_->vertices.size() );
	particleConstantsBuffer->Update(particleConstants.data(),( sizeof(ParticleConstantGPUData) * particleConstants.size() ));

	computeCommandList_->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
	computeCommandList_->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
	computeCommandList_->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
	computeCommandList_->SetComputeRootConstantBufferView(3,mesh_->constBoneBuffer->GetAddress());//b2

	computeCommandList_->SetComputeRootDescriptorTable(4,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
	computeCommandList_->SetComputeRootDescriptorTable(5,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

	computeCommandList_->SetComputeRootDescriptorTable(6,determineTexture->gpuHandle);//t0
	computeCommandList_->SetComputeRootDescriptorTable(7,boneMesh_->vertexBuffer->GetSRVAddress());//t1

	computeCommandList_->Dispatch(static_cast< uint32_t >( boneMesh_->vertices.size() / 1024 + 1 ),1,1);

	if ( root_ )
	{
		for ( BoneMesh* mesh : boneMesh_->parent )
		{
			PReadChildren(computeCommandList_,index_,mesh_,mesh,root_);
		}
	}
}
