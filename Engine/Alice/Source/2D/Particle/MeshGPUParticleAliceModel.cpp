#include "MeshGPUParticleAliceModel.h"

#include<BaseGPUParticle.h>

IMultiAdapters* MeshGPUParticleAliceModel::sMultiAdapters;

std::unordered_map<std::string,std::unique_ptr<MeshGPUParticleAliceModelData>> MeshGPUParticleAliceModel::sModelDatas;


const std::vector<std::unique_ptr<MeshGPUParticleModelMesh>>& MeshGPUParticleAliceModel::GetMeshs()
{
	return modelData->meshes;
}


void MeshGPUParticleAliceModel::SetModel(AliceModel* model_,BufferType type,bool bufferCreat_)
{
	std::string lFilePath = model_->modelData->filePath;

		//一回読み込んだことがあるファイルはそのまま返す
	auto lModelItr = find_if(sModelDatas.begin(),sModelDatas.end(),[ & ] (std::pair<const std::string,std::unique_ptr<MeshGPUParticleAliceModelData,std::default_delete<MeshGPUParticleAliceModelData>>>& modelData)
		{
			return modelData.second->filePath == lFilePath;
		});

	if ( lModelItr == sModelDatas.end() )
	{
		if ( type == BufferType::SHARED )
		{

		}
		else
		{
			std::unique_ptr<MeshGPUParticleAliceModelData> lData = std::make_unique<MeshGPUParticleAliceModelData>();

			AliceModelData* lModelData = model_->modelData;

			lData->name = lModelData->name;
			lData->filePath = lModelData->filePath;
			lData->postureMatBuff = lModelData->postureMatBuff.get();

			for ( std::unique_ptr<ModelMesh>& mesh : lModelData->meshes )
			{
				std::unique_ptr<MeshGPUParticleModelMesh> lMesh = std::make_unique<MeshGPUParticleModelMesh>();

				if ( !mesh->vecBones.empty() )
				{

					for ( uint32_t index : mesh->indices )
					{
						AddBoneMesh(lMesh->boneMeshs,index,mesh->vertices[ index ],mesh->vecBones);
					}
				}


				lMesh->name = mesh->name;
				lMesh->vertices = &mesh->vertices;
				lMesh->indices = &mesh->indices;
				lMesh->constBoneBuffer = CreateUniqueConstantBuffer(sizeof(BoneData),static_cast< AdaptersIndex >( type ));
				lMesh->bonedata = &mesh->bonedata;
				lMesh->postureMat = &mesh->node->globalTransform;

				lMesh->vertexBuffer = CreateUniqueVertexBuffer(mesh->vertices.size(),sizeof(mesh->vertices[ 0 ]),static_cast< AdaptersIndex >( type ),mesh->vertices.data());
				lMesh->vertexBuffer->CreateSRV();
				lMesh->indexBuffer = CreateUniqueIndexBuffer(mesh->indices.size(),static_cast< AdaptersIndex >( type ),mesh->indices.data());
				lMesh->indexBuffer->CreateSRV();
				lMesh->postureMatBuff = CreateUniqueConstantBuffer(sizeof(AliceMathF::Matrix4),static_cast< AdaptersIndex >( type ));
				lMesh->postureMatBuff->Update(lMesh->postureMat);

				if ( bufferCreat_ )
				{
					lMesh->particlePoolBuffer = CreateUniqueCrossAdapterBuffer(lMesh->vertices->size(),sizeof(BaseGPUParticle::ParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);
					lMesh->drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);

				}

				lData->meshes.push_back(std::move(lMesh));
			}

			sModelDatas[ lFilePath ] = std::move(lData);
			modelData = sModelDatas[ lFilePath ].get();
		}
	}
	else
	{
		modelData = sModelDatas[ lFilePath ].get();
	}
}

void MeshGPUParticleAliceModel::SSetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	sMultiAdapters = multiAdapters_;
}

const std::vector<PosNormUvTangeColSkin>& MeshGPUParticleModelMesh::GetVertices() const
{
	return *vertices;
}

D3D12_GPU_DESCRIPTOR_HANDLE MeshGPUParticleModelMesh::GetVertexSRVAddress() const
{
	return vertexBuffer->GetSRVAddress();
}

D3D12_GPU_DESCRIPTOR_HANDLE MeshGPUParticleModelMesh::GetIndicesSRVAddress() const
{
	return indexBuffer->GetSRVAddress();
}

IConstantBuffer* MeshGPUParticleModelMesh::GetBoneBuffer() const
{
	return constBoneBuffer.get();
}

IConstantBuffer* MeshGPUParticleModelMesh::GetPostureMatBuffer() const
{
	return postureMatBuff.get();
}

IDrawArgumentBuffer* MeshGPUParticleModelMesh::GetDrawArgumentBuffer() const
{
	return drawArgumentBuffer.get();
}

ICrossAdapterBuffer* MeshGPUParticleModelMesh::GetCrossAdapterBuffer() const
{
	return particlePoolBuffer.get();
}

void MeshGPUParticleAliceModel::Finalize()
{
	sMultiAdapters = nullptr;


	sModelDatas.clear();
}

const ReturnMotionNode* MeshGPUParticleAliceModel::PFindNodeAnim(const AliceMotionData* pAnimation_,const std::string& strNodeName_)
{
	return pAnimation_->GetMotion(strNodeName_);
}

void MeshGPUParticleAliceModel::AddBoneMesh(std::vector<std::unique_ptr<BoneMesh>>& boneMeshs_,uint32_t indice_,const PosNormUvTangeColSkin& ver_,const std::vector<Bone>& bone_)
{
	uint32_t lBoneIndex = GetBoneIndex(ver_);

	std::string lBoneName = bone_[ lBoneIndex ].name;

	auto lModelItr = find_if(boneMeshs_.begin(),boneMeshs_.end(),[ & ] (std::unique_ptr<BoneMesh,std::default_delete<BoneMesh>>& modelData)
		{
			return modelData->boneName == lBoneName;
		});


	if ( lModelItr == boneMeshs_.end() )
	{
		std::unique_ptr<BoneMesh> lMessh = std::make_unique<BoneMesh>();

		lMessh->boneName = lBoneName;
		lMessh->vertices.push_back(ver_);
		lMessh->indices.push_back(indice_);
	}
	else
	{
		BoneMesh* lMessh = lModelItr->get();
		lMessh->vertices.push_back(ver_);
		lMessh->indices.push_back(indice_);
	}
}

uint32_t MeshGPUParticleAliceModel::GetBoneIndex(const PosNormUvTangeColSkin& ver_)
{
	float lWeight = 0.0f;
	size_t lIndex = 0;

	for ( size_t i = 0; i < ver_.boneWeight.size(); i++ )
	{
		if ( lWeight < ver_.boneWeight[ i ] )
		{
			lWeight = ver_.boneWeight[ i ];
			lIndex = i;
		}
	}

	return ver_.boneIndex[ lIndex ];
}
