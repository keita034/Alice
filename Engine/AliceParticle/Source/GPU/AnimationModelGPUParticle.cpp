#include "AnimationModelGPUParticle.h"

#include<AliceAssert.h>

void AnimationModelGPUParticle::Initialize()
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
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void AnimationModelGPUParticle::Update(float deltaTime_)
{
	ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();
	MeshGPUParticleModelMesh* mesh = modelData->GetMeshs().front().get();

	if ( emitDatas.isPlay )
	{
		PUpdateConstantBuffer(deltaTime_);

		if ( !isEmit )
		{
			isEmit = true;

			//生成
			{
				BaseGPUParticle::ParticleEnd();

				ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleEmit",AdaptersIndex::SUB);

				if ( !animationStop )
				{
					BoneData* lBonedata = mesh->bonedata;
					mesh->constBoneBuffer->Update(lBonedata->boneMat.data());
				}

				for ( std::unique_ptr<BoneMesh>& boneMesh : mesh->boneMeshs )
				{
					BaseGPUParticle::ParticleBegin();

					lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
					lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

					ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
					lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);


					particleConstants.meshIndex  = (uint32_t)boneMesh->index;
					particleConstants.vertexSize = static_cast< uint32_t >( boneMesh->vertices.size() );
					particleConstantsBuffer->Update(&particleConstants,sizeof(ParticleConstantGPUData));

					lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
					lComputeCommandList->SetComputeRootConstantBufferView(1,mesh->constBoneBuffer->GetAddress());//b3

					lComputeCommandList->SetComputeRootDescriptorTable(2,boneMesh->vertexBuffer->GetSRVAddress());//t0

					lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
					lComputeCommandList->SetComputeRootDescriptorTable(4,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

					lComputeCommandList->Dispatch(static_cast< uint32_t >( boneMesh->vertices.size() / 1024 + 1 ),1,1);

					BaseGPUParticle::ParticleEnd();
				}

				BaseGPUParticle::ParticleBegin();

			}

		}
	}

	//更新
	{
		particleConstants.vertexSize = static_cast< uint32_t >( modelData->GetVerticeSize() );
		particleConstantsBuffer->Update(&particleConstants);

		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);


		if ( !animationStop )
		{
			BoneData* lBonedata = mesh->bonedata;
			mesh->constBoneBuffer->Update(lBonedata->boneMat.data());
		}

		lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b2
		lComputeCommandList->SetComputeRootConstantBufferView(1,mesh->constBoneBuffer->GetAddress());//b3
		lComputeCommandList->SetComputeRootConstantBufferView(2,boneMeshDatasBuffer->GetAddress());//b3

		lComputeCommandList->SetComputeRootDescriptorTable(3,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0
		lComputeCommandList->SetComputeRootDescriptorTable(4,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(static_cast< uint32_t >( modelData->GetVerticeSize() / 1024 + 1 ),1,1);
	}

	//ドローアギュメント更新
	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::MAIN));//u1
		lComputeCommandList->SetComputeRootDescriptorTable(1,drawArgumentBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

		lComputeCommandList->Dispatch(1,1,1);
	}
}

void AnimationModelGPUParticle::Finalize()
{

}

void AnimationModelGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
{
	if ( emitDatas.isPlay )
	{
		AliceAssertNull(texture,"テクスチャがありません");

		{
			worldBillboardGPUData.worldMat = worldMat_;
			worldBillboardGPUData.billboardMat = billboardMat_;

			worldBillboardBuffer->Update(&worldBillboardGPUData);
		}

		ID3D12GraphicsCommandList* lGraphicCommandList = graphicAdapter->GetGraphicCommandList();

		Material* lMaterial = MaterialManager::SGetMaterial("AnimationModelGPUParticleDraw",AdaptersIndex::MAIN);

		lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
		lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lGraphicCommandList->SetGraphicsRootConstantBufferView(0,worldBillboardBuffer->GetAddress());

		lGraphicCommandList->SetGraphicsRootDescriptorTable(1,particlePoolBuffer->GetAddress(CrossAdapterResourceIndex::SUB));
		lGraphicCommandList->SetGraphicsRootDescriptorTable(2,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::SUB));
		lGraphicCommandList->SetGraphicsRootDescriptorTable(3,drawListBuffer->GetUAVAddress(CrossAdapterResourceIndex::SUB));

		lGraphicCommandList->SetGraphicsRootDescriptorTable(4,texture->gpuHandle);

		lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,drawArgumentBuffer->GetResource(CrossAdapterResourceIndex::SUB),0,nullptr,0);

	}
}

void AnimationModelGPUParticle::SetSetting()
{
}

void AnimationModelGPUParticle::Create(uint32_t maxParticles_)
{
	static_cast< void >( maxParticles_ );
	Initialize();
}

void AnimationModelGPUParticle::Emit(const AnimationModelGPUParticleSetting& setting_)
{
	ParticleEmit lEmitData;
	ParticleConstantGPUData particleConstant;

	lEmitData.matWorld = particleConstant.matWorld = setting_.matWorld;
	lEmitData.startColor = particleConstant.color = setting_.startColor;
	lEmitData.endColor = particleConstant.setColor = setting_.endColor;
	lEmitData.size = particleConstant.size = setting_.size;

	lEmitData.timeBetweenEmit = setting_.timeBetweenEmit;
	lEmitData.isPlay = setting_.isPlay;

	emitDatas = lEmitData;
	particleConstants = particleConstant;

	particleConstantsBuffer->Update(&particleConstants);
}

void AnimationModelGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_)
{
	particleConstants.matWorld = emitDatas.matWorld = matWorld_;

	particleConstantsBuffer->Update(&particleConstants);
}

void AnimationModelGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void AnimationModelGPUParticle::EmitPlay()
{
	if ( !emitDatas.isPlay )
	{
		emitDatas.isPlay = true;
		emitDatas.emitTimeCounter = 0.0f;
	}
}

void AnimationModelGPUParticle::EmitStop()
{
	if ( emitDatas.isPlay )
	{
		emitDatas.isPlay = false;
	}
}

void AnimationModelGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::SUB,true);
	modelData->CreateBoneMeshIsVisibles(boneMeshIsVisibles);

	particlePoolBuffer = CreateUniqueCrossAdapterBuffer(modelData->GetVerticeSize(),sizeof(ParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawListBuffer = CreateUniqueDrawListBuffer(modelData->GetVerticeSize(),sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	freeListBuffer = CreateUniqueFreeListBuffer(modelData->GetVerticeSize(),sizeof(uint32_t),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
	drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);

	BaseGPUParticle::ParticleBegin();

	{
		particleConstants.vertexSize = static_cast< uint32_t >( modelData->GetVerticeSize() );
		particleConstantsBuffer->Update(&particleConstants);

		ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeAnimationModelGPUParticleFreeListInit",AdaptersIndex::SUB);

		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
		lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
		lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		lComputeCommandList->SetComputeRootDescriptorTable(0,freeListBuffer->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

		lComputeCommandList->SetComputeRootConstantBufferView(1,particleConstantsBuffer->GetAddress());//b0

		lComputeCommandList->Dispatch(static_cast< UINT >( modelData->GetVerticeSize() / 1024 ) + 1,1,1);
	}

	for ( BoneMeshData& index : boneMeshDatas.boneMeshisVisibles )
	{
		index.sVisibles = 1;
	}

	boneMeshDatasBuffer->Update(&boneMeshDatas);

	BaseGPUParticle::ParticleEnd();
}

float AnimationModelGPUParticle::GetDeltaTime() const
{
	return particleConstants.deltaTime;
}

void AnimationModelGPUParticle::DrawListRelease()
{

}

void AnimationModelGPUParticle::InvisibleBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	modelData->InvisibleBoneMesh(boneMeshIsVisibles,meshName_,boneName_,root_);

	MeshGPUParticleModelMesh* mesh = modelData->GetMeshs().front().get();

	for ( std::unique_ptr<BoneMesh>& boneMesh : mesh->boneMeshs )
	{
		boneMeshDatas.boneMeshisVisibles[ boneMesh->index ].sVisibles = (uint32_t)boneMeshIsVisibles[ meshName_ ][ boneMesh->boneName ];
	}

	boneMeshDatasBuffer->Update(&boneMeshDatas);

}

void AnimationModelGPUParticle::VisibleBoneMesh(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	modelData->VisibleBoneMesh(boneMeshIsVisibles,meshName_,boneName_,root_);
	MeshGPUParticleModelMesh* mesh = modelData->GetMeshs().front().get();

	for ( std::unique_ptr<BoneMesh>& boneMesh : mesh->boneMeshs )
	{
		boneMeshDatas.boneMeshisVisibles[ boneMesh->index ].sVisibles = ( uint32_t ) boneMeshIsVisibles[ meshName_ ][ boneMesh->boneName ];
	}

	boneMeshDatasBuffer->Update(&boneMeshDatas);
}

void AnimationModelGPUParticle::StopAnimation()
{
	animationStop = true;
}

void AnimationModelGPUParticle::PlayAnimation()
{
	animationStop = false;
}

void AnimationModelGPUParticle::PBufferCreate()
{
	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUDatas),AdaptersIndex::SUB);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);
	boneMeshDatasBuffer = CreateUniqueConstantBuffer(sizeof(BoneMeshDatas),AdaptersIndex::SUB);
}

void AnimationModelGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	particleConstants.deltaTime = deltaTime_;
	particleConstants.totalTime += deltaTime_;
	particleConstants.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	particleConstantsBuffer->Update(&particleConstants);
}

void AnimationModelGPUParticle::ChangeColor(const AliceMathF::Vector4& color_,const std::string& meshName_,const std::string& boneName_,bool root_)
{
	modelData->SetValueBoneMesh(2,boneMeshIsVisibles,meshName_,boneName_,root_);

	MeshGPUParticleModelMesh* mesh = modelData->GetMeshs().front().get();

	for ( std::unique_ptr<BoneMesh>& boneMesh : mesh->boneMeshs )
	{
		boneMeshDatas.boneMeshisVisibles[ boneMesh->index ].sVisibles = boneMeshIsVisibles[ meshName_ ][ boneMesh->boneName ];
	}

	boneMeshDatasBuffer->Update(&boneMeshDatas);

	particleConstants.setColor = color_;
	particleConstantsBuffer->Update(&particleConstants);
}

void AnimationModelGPUParticle::ReturnColor(const std::string& meshName_,const std::string& boneName_,bool root_)
{
	modelData->SetValueBoneMesh(true,boneMeshIsVisibles,meshName_,boneName_,root_);

	MeshGPUParticleModelMesh* mesh = modelData->GetMeshs().front().get();

	for ( std::unique_ptr<BoneMesh>& boneMesh : mesh->boneMeshs )
	{
		boneMeshDatas.boneMeshisVisibles[ boneMesh->index ].sVisibles = ( uint32_t ) boneMeshIsVisibles[ meshName_ ][ boneMesh->boneName ];
	}

	boneMeshDatasBuffer->Update(&boneMeshDatas);
}


bool AnimationModelGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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
