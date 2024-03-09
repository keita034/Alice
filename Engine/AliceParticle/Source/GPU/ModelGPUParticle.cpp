#include "ModelGPUParticle.h"

#include<AliceAssert.h>

void ModelGPUParticle::Initialize()
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
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelGPUParticleEmit",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelGPUParticleUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}

	{
		ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);
		lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
	}
}

void ModelGPUParticle::Update(float deltaTime_)
{
	if ( emitDatas.isPlay && dirty )
	{
		dirty = false;

		PUpdateConstantBuffer(deltaTime_);

		ID3D12GraphicsCommandList* lComputeCommandList = computeAdapter->GetComputeCommandList();

		if ( !isEmit )
		{
			isEmit = true;

			//生成
			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

				particleConstants.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
				particleConstantsBuffer->Update(&particleConstants);

				ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelGPUParticleEmit",AdaptersIndex::SUB);

				lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
				lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

				ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
				lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

				lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
				lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
				lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
				lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->postureMatBuff->GetAddress());//b3

				lComputeCommandList->SetComputeRootDescriptorTable(4,mesh->GetVertexSRVAddress());//t0

				lComputeCommandList->SetComputeRootDescriptorTable(5,mesh->GetCrossAdapterBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

				lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);
			}

			//ドローアギュメント更新
			{
				ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelGPUParticleDrawArgumentUpdate",AdaptersIndex::SUB);

				lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
				lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

				ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
				lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

				lComputeCommandList->SetComputeRootConstantBufferView(0,particleConstantsBuffer->GetAddress());//b0

				for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
				{
					particleConstants.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
					particleConstantsBuffer->Update(&particleConstants);

					lComputeCommandList->SetComputeRootDescriptorTable(1,mesh->GetDrawArgumentBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u1

					lComputeCommandList->Dispatch(1,1,1);
				}
			}
		}

		//更新
		{
			ComputeMaterial* lComputeMaterial = MaterialManager::SGetComputeMaterial("ComputeModelGPUParticleUpdate",AdaptersIndex::SUB);

			lComputeCommandList->SetPipelineState(lComputeMaterial->pipelineState->GetPipelineState());
			lComputeCommandList->SetComputeRootSignature(lComputeMaterial->rootSignature->GetRootSignature());

			ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::SUB)->GetHeap() };
			lComputeCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

			size_t i = 0;
			for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
			{
				particleConstants.vertexSize = static_cast< uint32_t >( mesh->GetVertices().size() );
				particleConstantsBuffer->Update(&particleConstants);

				lComputeCommandList->SetComputeRootConstantBufferView(0,timeConstantsBuffer->GetAddress());//b0
				lComputeCommandList->SetComputeRootConstantBufferView(1,gpuParticleDataBuffer->GetAddress());//b1
				lComputeCommandList->SetComputeRootConstantBufferView(2,particleConstantsBuffer->GetAddress());//b2
				lComputeCommandList->SetComputeRootConstantBufferView(3,mesh->postureMatBuff->GetAddress());//b3

				lComputeCommandList->SetComputeRootDescriptorTable(4,mesh->GetCrossAdapterBuffer()->GetAddress(CrossAdapterResourceIndex::MAIN));//u0

				lComputeCommandList->SetComputeRootDescriptorTable(5,mesh->GetVertexSRVAddress());//t0

				lComputeCommandList->Dispatch(static_cast< uint32_t >( mesh->GetVertices().size() / 1024 + 1 ),1,1);

				i++;
			}
		}
	}
}

void ModelGPUParticle::Finalize()
{

}

void ModelGPUParticle::Draw(const AliceMathF::Matrix4& worldMat_,const AliceMathF::Matrix4& billboardMat_)
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

		Material* lMaterial = MaterialManager::SGetMaterial("ModelGPUParticleDraw",AdaptersIndex::MAIN);

		lGraphicCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		lGraphicCommandList->SetPipelineState(lMaterial->pipelineState->GetPipelineState());
		lGraphicCommandList->SetGraphicsRootSignature(lMaterial->rootSignature->GetRootSignature());

		ID3D12DescriptorHeap* lDescriptorHeaps[ ] = { BaseBuffer::SGetSRVDescriptorHeap(AdaptersIndex::MAIN)->GetHeap() };
		lGraphicCommandList->SetDescriptorHeaps(_countof(lDescriptorHeaps),lDescriptorHeaps);

		for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->GetMeshs() )
		{
			lGraphicCommandList->SetGraphicsRootConstantBufferView(0,worldBillboardBuffer->GetAddress());

			lGraphicCommandList->SetGraphicsRootDescriptorTable(1,mesh->GetCrossAdapterBuffer()->GetAddress(CrossAdapterResourceIndex::SUB));

			lGraphicCommandList->SetGraphicsRootDescriptorTable(2,texture->gpuHandle);

			lGraphicCommandList->ExecuteIndirect(particleCommandSignature.Get(),1,mesh->GetDrawArgumentBuffer()->GetResource(CrossAdapterResourceIndex::SUB),0,nullptr,0);
		}
	}
}

void ModelGPUParticle::SetSetting()
{
}

void ModelGPUParticle::Create(uint32_t maxParticles_)
{
	fireGPUParticleGPUData.maxParticles = maxParticles_;

	maxParticles = static_cast< size_t >( maxParticles_ );

	Initialize();
}

void ModelGPUParticle::Emit(const ModelGPUParticleSetting& setting_)
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
	lEmitData.isPlay = setting_.isPlay;

	emitDatas = lEmitData;
	particleConstants = particleConstant;

	particleConstantsBuffer->Update(&particleConstants);
}

void ModelGPUParticle::SetMat(const AliceMathF::Matrix4& matWorld_)
{
	dirty = true;
	particleConstants.matWorld = emitDatas.matWorld = matWorld_;

	particleConstantsBuffer->Update(&particleConstants);
}

void ModelGPUParticle::SetTex(uint32_t textureHandle_)
{
	texture = TextureManager::SGetTextureData(textureHandle_);
}

void ModelGPUParticle::EmitPlay()
{
	if ( !emitDatas.isPlay )
	{
		emitDatas.isPlay = true;
		emitDatas.emitTimeCounter = 0.0f;
	}
}

void ModelGPUParticle::EmitStop()
{
	if ( emitDatas.isPlay )
	{
		emitDatas.isPlay = false;
	}
}

void ModelGPUParticle::SetModel(AliceModel* model_)
{
	modelData->SetModel(model_,BufferType::SUB,true);
}

float ModelGPUParticle::GetDeltaTime() const
{
	return timeGPUData.deltaTime;
}

void ModelGPUParticle::PBufferCreate()
{
	particleConstantsBuffer = CreateUniqueConstantBuffer(sizeof(ParticleConstantGPUDatas),AdaptersIndex::SUB);
	gpuParticleDataBuffer = CreateUniqueConstantBuffer(sizeof(FireGPUParticleGPUData),AdaptersIndex::SUB);
	timeConstantsBuffer = CreateUniqueConstantBuffer(sizeof(TimeConstantGPUData),AdaptersIndex::SUB);
	worldBillboardBuffer = CreateUniqueConstantBuffer(sizeof(WorldBillboardGPUData),AdaptersIndex::MAIN);

	gpuParticleDataBuffer->Update(&fireGPUParticleGPUData);

}

void ModelGPUParticle::PUpdateConstantBuffer(float deltaTime_)
{
	timeGPUData.deltaTime = deltaTime_;
	timeGPUData.totalTime += deltaTime_;
	timeGPUData.computeTime = static_cast< uint32_t >( GetTickCount64() / 1000 );

	timeConstantsBuffer->Update(&timeGPUData);
}

bool ModelGPUParticle::PCanEmit(ParticleEmit& data_,float deltaTime_)
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