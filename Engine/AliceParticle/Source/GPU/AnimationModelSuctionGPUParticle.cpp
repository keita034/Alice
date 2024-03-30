#include "AnimationModelSuctionGPUParticle.h"

#include<AliceAssert.h>

void AnimationModelSuctionGPUParticle::Initialize()
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

	ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetGraphicCommandList();

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleFreeListInit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootConstantBufferView(1,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleEmit",AdaptersIndex::MAIN);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleUpdate",AdaptersIndex::MAIN);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleDrawArgumentUpdate",AdaptersIndex::MAIN);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void AnimationModelSuctionGPUParticle::Update(float deltaTime_)
{

	if ( emitTime > 0 )
	{
		emitTime -= deltaTime_;
	}

	PUpdateConstantBuffer(deltaTime_);

	ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetGraphicCommandList();

	if ( !init )
	{
		init = true;

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleFreeListInit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootConstantBufferView(1,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);
	}

		//生成
	if ( PCanEmit(emitData,deltaTime_) )
	{

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleEmit",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
		{
			BoneData* lBonedata = mesh->bonedata;
			mesh->constBoneBuffer->Update(lBonedata->boneMat.data());

			if ( boneMesh )
			{
				PReadChildren(lComputeCommandList,mesh.get(),boneMesh,boneMeshRoot);
			}
			else
			{
				particleConstant.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
				particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

				lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
				lComputeCommandList->SetComputeRootConstantBufferView(1,mesh->constBoneBuffer->GetAddress());//b2

				lComputeCommandList->SetComputeRootDescriptorTable(2,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
				lComputeCommandList->SetComputeRootDescriptorTable(3,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

				lComputeCommandList->SetComputeRootDescriptorTable(4,mesh->GetVertexSRVAddress());//t1

				lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);

			}
		}
	}

	//更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2

		lComputeCommandList->SetComputeRootDescriptorTable(1,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(2,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1
		lComputeCommandList->SetComputeRootDescriptorTable(3,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u2

		lComputeCommandList->Dispatch(static_cast< UINT >( maxParticles / 1024 ) + 1,1,1);

	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleDrawArgumentUpdate",AdaptersIndex::MAIN);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void AnimationModelSuctionGPUParticle::Finalize()
{

}

void AnimationModelSuctionGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	if ( emitTime )
	{
		if ( draw )
		{
			AliceAssertNull(texture,"テクスチャがありません");

			{
				worldBillboardGPUData.worldMat = worldMat_;
				worldBillboardGPUData.billboardMat = billboardMat_;

				worldBillboardBuffer->Update(&worldBillboardGPUData);
			}

			ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

			Material* lMaterial = MaterialManager::SGetMaterial("AnimationModelSuctionGPUParticleDraw",AdaptersIndex::MAIN);

			lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

			lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
			lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
			lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			lGraphicCommandList->SetGraphicsRootConstantBufferView(0,worldBillboardBuffer->GetAddress());

			lGraphicCommandList->SetGraphicsRootDescriptorTable(1,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));
			lGraphicCommandList->SetGraphicsRootDescriptorTable(2,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));

			lGraphicCommandList->SetGraphicsRootDescriptorTable(3,texture->gpuHandle);

			graphicAdapter->ResourceTransition(drawArgumentBuffer->GetResource(CrossAdapterResourceIndex::MAIN),D3D12_RESOURCE_STATE_UNORDERED_ACCESS,D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);

			lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(CrossAdapterResourceIndex::MAIN),0,nullptr,0);

			graphicAdapter->ResourceTransition(drawArgumentBuffer->GetResource(CrossAdapterResourceIndex::MAIN),D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		}
	}
}

void AnimationModelSuctionGPUParticle::SetSetting()
{
}

void AnimationModelSuctionGPUParticle::Create(uint32_t maxParticles_)
{
	particleConstant.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

void AnimationModelSuctionGPUParticle::Emit(const AnimationModelSuctionGPUParticleSetting& setting_)
{
	emitData.lifeTime = particleConstant.lifeTime = setting_.lifeTime;
	emitData.matWorld = particleConstant.matWorld = setting_.matWorld;
	emitData.startColor = particleConstant.startColor = setting_.startColor;
	emitData.endColor = particleConstant.endColor = setting_.endColor;
	emitData.size = particleConstant.size = setting_.size;

	particleConstant.invisibleTime = setting_.invisibleTime;
	particleConstant.moveTime = setting_.moveTime;
	particleConstant.distance = setting_.distance;
	emitData.isInfinityEmit = setting_.isInfinityEmit;
	emitData.timeBetweenEmit = setting_.timeBetweenEmit;
	emitData.isPlay = setting_.isPlay;

	emitData.maxParticles = particleConstant.maxParticles;
	emitData.index = 0;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void AnimationModelSuctionGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_)
{
	particleConstant.matWorld = emitData.matWorld = matWorld_;

	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));
}

void AnimationModelSuctionGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void AnimationModelSuctionGPUParticle::EmitPlay(bool flag_)
{
	if ( !emitData.isPlay )
	{
		emitData.isPlay = true;

		emitTime = particleConstant.lifeTime;

		if ( flag_ )
		{
			emitData.emitTimeCounter = emitData.timeBetweenEmit;

			BaseGPUParticle::ParticleBegin();

			ID3D12GraphicsCommandList* lComputeCommandList = graphicAdapter->GetGraphicCommandList();

			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelSuctionGPUParticleEmit",AdaptersIndex::MAIN);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				BoneData* lBonedata = mesh->bonedata;
				mesh->constBoneBuffer->Update(lBonedata->boneMat.data());

				if ( boneMesh )
				{
					PReadChildren(lComputeCommandList,mesh.get(),boneMesh,boneMeshRoot);
				}
				else
				{
					particleConstant.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
					particleConstantsBuffer->Update(&particleConstant);

					lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
					lComputeCommandList->SetComputeRootConstantBufferView(1,mesh->constBoneBuffer->GetAddress());//b2

					lComputeCommandList->SetComputeRootDescriptorTable(2,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
					lComputeCommandList->SetComputeRootDescriptorTable(3,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

					lComputeCommandList->SetComputeRootDescriptorTable(4,mesh->GetVertexSRVAddress());//t1

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

void AnimationModelSuctionGPUParticle::EmitStop()
{
	if ( emitData.isPlay )
	{
		emitData.isPlay = false;
	}
}

void AnimationModelSuctionGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::MAIN);
}

void AnimationModelSuctionGPUParticle::SetBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	boneMesh = modelData->GetBoneMesh(meshName_,boneName_);
	boneMeshRoot = root_;
}

void AnimationModelSuctionGPUParticle::SetCenterPos(const AliceMathF::Vector3& pos_)
{
	particleConstant.centerPos = pos_;
	particleConstantsBuffer->Update(&particleConstant);
}

void AnimationModelSuctionGPUParticle::DrawStop()
{
	draw = false;
}

void AnimationModelSuctionGPUParticle::DrawStart()
{
	draw = true;
}

const Bone* AnimationModelSuctionGPUParticle::GetBone(const std::string& meshName_,const std::string& boneName_) const
{
	return modelData->GetBoneMesh(meshName_,boneName_)->bone;
}

AliceMathF::Vector3 AnimationModelSuctionGPUParticle::GetMeshBoneCenterPos(const std::string& meshName_,const std::string& boneName_)const
{
	return modelData->GetBoneMesh(meshName_,boneName_)->centerPos;
}

float AnimationModelSuctionGPUParticle::GetDeltaTime() const
{
	return particleConstant.deltaTime;
}

size_t AnimationModelSuctionGPUParticle::GetVerticeSize() const
{
	return verticeSize;
}

void AnimationModelSuctionGPUParticle::PBufferCreate()
{
	particlePoolBuffer = CreateUniqueCrossAdapterBuffer(maxParticles,sizeof(ModelParticleGPUData),AdaptersIndex::MAIN,AdaptersIndex::SUB);
	drawListBuffer = CreateUniqueDrawListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::MAIN,AdaptersIndex::SUB);
	freeListBuffer = CreateUniqueFreeListBuffer(maxParticles,sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::MAIN,AdaptersIndex::SUB);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::MAIN,AdaptersIndex::SUB);

	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUData),AdaptersIndex::MAIN);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);

	particleConstantsBuffer->Update(&particleConstant);

}

void AnimationModelSuctionGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	particleConstant.deltaTime = deltaTime_;
	particleConstant.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );
}

bool AnimationModelSuctionGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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

void AnimationModelSuctionGPUParticle::PReadChildren(ID3D12GraphicsCommandList* computeCommandList_,MeshGPUParticleModelMesh* mesh_,BoneMesh* boneMesh_,bool root_)
{
	particleConstant.vertexSize = static_cast< uint32_t >( boneMesh_->vertices.size() );
	particleConstantsBuffer->Update(&particleConstant,sizeof(ParticleConstantGPUData));

	computeCommandList_->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
	computeCommandList_->SetComputeRootConstantBufferView(1,mesh_->constBoneBuffer->GetAddress());//b2

	computeCommandList_->SetComputeRootDescriptorTable(2,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
	computeCommandList_->SetComputeRootDescriptorTable(3,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

	computeCommandList_->SetComputeRootDescriptorTable(4,boneMesh_->vertexBuffer->GetSRVAddress());//t1

	computeCommandList_->Dispatch(static_cast< uint32_t >( boneMesh_->vertices.size() / 1024 + 1 ),1,1);

	if ( root_ )
	{
		for ( BoneMesh* mesh : boneMesh_->parent )
		{
			PReadChildren(computeCommandList_,mesh_,mesh,root_);
		}
	}
}
