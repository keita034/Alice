#include "MeshGPUParticleAliceModel.h"

IMultiAdapters* AnimationMeshGPUParticleAliceModel::sMultiAdapters;

std::unordered_map<std::string,std::unique_ptr<AnimationMeshGPUParticleAliceModelData>> AnimationMeshGPUParticleAliceModel::sModelDatas;


const std::vector<std::unique_ptr<AnimationMeshGPUParticleModelMesh>>& AnimationMeshGPUParticleAliceModel::GetMeshs()
{
	return modelData->meshes;
}


void AnimationMeshGPUParticleAliceModel::SetModel(AliceModel* model_,BufferType type)
{
	std::string lFilePath = model_->modelData->filePath;

		//一回読み込んだことがあるファイルはそのまま返す
	auto lModelItr = find_if(sModelDatas.begin(),sModelDatas.end(),[ & ] (std::pair<const std::string,std::unique_ptr<AnimationMeshGPUParticleAliceModelData,std::default_delete<AnimationMeshGPUParticleAliceModelData>>>& modelData)
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
			std::unique_ptr<AnimationMeshGPUParticleAliceModelData> lData = std::make_unique<AnimationMeshGPUParticleAliceModelData>();

			AliceModelData* lModelData = model_->modelData;

			lData->name = lModelData->name;
			lData->filePath = lModelData->filePath;
			lData->postureMatBuff = lModelData->postureMatBuff.get();

			for ( std::unique_ptr<ModelMesh>& mesh : lModelData->meshes )
			{
				std::unique_ptr<AnimationMeshGPUParticleModelMesh> lMesh = std::make_unique<AnimationMeshGPUParticleModelMesh>();

				lMesh->name = mesh->name;
				lMesh->vertices = &mesh->vertices;
				lMesh->indices = &mesh->indices;
				lMesh->constBoneBuffer = CreateUniqueConstantBuffer(sizeof(BoneData),static_cast< AdaptersIndex >( type ));
				lMesh->bonedata = &mesh->bonedata;

				lMesh->vertexBuffer = CreateUniqueVertexBuffer(mesh->vertices.size(),sizeof(mesh->vertices[0]),static_cast<AdaptersIndex>(type),mesh->vertices.data());
				lMesh->vertexBuffer->CreateSRV();
				lMesh->indexBuffer = CreateUniqueIndexBuffer(mesh->indices.size(), static_cast<AdaptersIndex>(type), mesh->indices.data());
				lMesh->indexBuffer->CreateSRV();

				lData->meshes.push_back(std::move(lMesh));
			}

			sModelDatas[ lFilePath ] = std::move(lData);
			modelData = sModelDatas[ lFilePath ].get();
		}
	}
	else
	{
		modelData = sModelDatas[ lFilePath].get();
	}
}

void AnimationMeshGPUParticleAliceModel::SSetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	sMultiAdapters = multiAdapters_;
}

const std::vector<PosNormUvTangeColSkin>& AnimationMeshGPUParticleModelMesh::GetVertices()
{
	return *vertices;
}

D3D12_GPU_DESCRIPTOR_HANDLE AnimationMeshGPUParticleModelMesh::GetVertexSRVAddress() const
{
	return vertexBuffer->GetSRVAddress();
}

D3D12_GPU_DESCRIPTOR_HANDLE AnimationMeshGPUParticleModelMesh::GetIndicesSRVAddress() const
{
	return indexBuffer->GetSRVAddress();
}

IConstantBuffer* AnimationMeshGPUParticleModelMesh::GetBoneBuffer()
{
	return constBoneBuffer.get();
}

void AnimationMeshGPUParticleAliceModel::Finalize()
{
	sMultiAdapters = nullptr;


	sModelDatas.clear();
}

const ReturnMotionNode* AnimationMeshGPUParticleAliceModel::PFindNodeAnim(const AliceMotionData* pAnimation_,const std::string& strNodeName_)
{
	return pAnimation_->GetMotion(strNodeName_);
}
