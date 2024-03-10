#include "MeshGPUParticleAliceModel.h"

#include<BaseGPUParticle.h>
#include<AliceAssert.h>

IMultiAdapters* MeshGPUParticleAliceModel::sMultiAdapters;

std::unordered_map<std::string,std::unique_ptr<MeshGPUParticleAliceModelData>> MeshGPUParticleAliceModel::sModelDatas;


const std::vector<std::unique_ptr<MeshGPUParticleModelMesh>>& MeshGPUParticleAliceModel::GetMeshs()
{
	return modelData->meshes;
}


void MeshGPUParticleAliceModel::SetModel(AliceModel* model_,BufferType type,bool bufferCreat_)
{
	std::string lFilePath = model_->modelData->filePath;

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
					for (const PosNormUvTangeColSkin& index : mesh->vertices )
					{
						AddBoneMesh(lMesh->boneMeshs,index,mesh->vecBones);

						lMesh->boneMesh = true;
					}

					for ( std::unique_ptr<BoneMesh>& boneMesh : lMesh->boneMeshs )
					{
						PReadChildren(lModelData->nodes,lMesh->boneMeshs,boneMesh.get());

						boneMesh->centerPos /= (float)boneMesh->vertices.size();

						boneMesh->vertexBuffer = CreateUniqueVertexBuffer(boneMesh->vertices.size(),sizeof(boneMesh->vertices[ 0 ]),static_cast< AdaptersIndex >( type ),boneMesh->vertices.data());
						boneMesh->vertexBuffer->CreateSRV();
						boneMesh->drawArgumentBuffer = CreateUniqueDrawArgumentBuffer(1,sizeof(D3D12_DRAW_ARGUMENTS),BufferType::SHARED,AdaptersIndex::SUB,AdaptersIndex::MAIN);
						boneMesh->particlePoolBuffer = CreateUniqueCrossAdapterBuffer(boneMesh->vertices.size(),sizeof(BaseGPUParticle::ModelParticleGPUData),AdaptersIndex::SUB,AdaptersIndex::MAIN);
						lData->verticeSize += boneMesh->vertices.size();
					}
				}

				if ( !lMesh->boneMesh )
				{
					lMesh->vertexBuffer = CreateUniqueVertexBuffer(mesh->vertices.size(),sizeof(mesh->vertices[ 0 ]),static_cast< AdaptersIndex >( type ),mesh->vertices.data());
					lMesh->vertexBuffer->CreateSRV();
				}

				lMesh->name = mesh->name;
				lMesh->vertices = &mesh->vertices;
				lMesh->constBoneBuffer = CreateUniqueConstantBuffer(sizeof(BoneData),static_cast< AdaptersIndex >( type ));
				lMesh->bonedata = &mesh->bonedata;
				lMesh->postureMat = &mesh->node->globalTransform;
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

static void ReadParent(std::unordered_map<std::string,bool>& boneMeshIsVisibles_,BoneMesh* parent_,bool isVisible_)
{
	boneMeshIsVisibles_[ parent_ ->boneName] = isVisible_;

	for ( BoneMesh* children : parent_->parent )
	{
		ReadParent(boneMeshIsVisibles_,children,isVisible_);
	}
}

void MeshGPUParticleModelMesh::InvisibleBoneMesh(std::unordered_map<std::string,bool>& boneMeshIsVisibles_,const std::string& boneName_,bool root_)
{
	auto lModelItr = find_if(boneMeshs.begin(),boneMeshs.end(),[ & ] (std::unique_ptr<BoneMesh,std::default_delete<BoneMesh>>& modelData)
	{
		return modelData->boneName == boneName_;
	});

	if ( lModelItr == boneMeshs.end() )
	{
		AliceDebugPrint("そのボーンは存在しません");
	}
	else
	{
		BoneMesh* mesh = lModelItr->get();

		if ( root_ )
		{
			ReadParent(boneMeshIsVisibles_,mesh,false);
		}
		else
		{
			boneMeshIsVisibles_[ boneName_ ] = false;
		}
	}
}

void MeshGPUParticleModelMesh::VisibleBoneMesh(std::unordered_map<std::string,bool>& boneMeshIsVisibles_,const std::string& boneName_,bool root_)
{
	auto lModelItr = find_if(boneMeshs.begin(),boneMeshs.end(),[ & ] (std::unique_ptr<BoneMesh,std::default_delete<BoneMesh>>& modelData)
	{
		return modelData->boneName == boneName_;
	});

	if ( lModelItr == boneMeshs.end() )
	{
		AliceDebugPrint("そのボーンは存在しません");
	}
	else
	{
		BoneMesh* mesh = lModelItr->get();

		if ( root_ )
		{
			ReadParent(boneMeshIsVisibles_,mesh,true);
		}
		else
		{
			boneMeshIsVisibles_[ boneName_ ] = true;
		}
	}
}

BoneMesh* MeshGPUParticleModelMesh::GetBoneMesh(const std::string& boneName_)
{
	auto lModelItr = find_if(boneMeshs.begin(),boneMeshs.end(),[ & ] (std::unique_ptr<BoneMesh,std::default_delete<BoneMesh>>& modelData)
{
	return modelData->boneName == boneName_;
});

	if ( lModelItr == boneMeshs.end() )
	{
		AliceDebugPrint("そのボーンは存在しません");

		return nullptr;
	}
	else
	{
		return lModelItr->get();
	}
}

void MeshGPUParticleModelMesh::CreateBoneMeshIsVisibles(std::unordered_map<std::string,bool>& boneMeshIsVisibles_)
{
	for ( std::unique_ptr<BoneMesh>& mesh : boneMeshs )
	{
		boneMeshIsVisibles_[ mesh->boneName ] = true;
	}
}

void MeshGPUParticleAliceModel::Finalize()
{
	sMultiAdapters = nullptr;


	sModelDatas.clear();
}

void MeshGPUParticleAliceModel::CreateBoneMeshIsVisibles(std::unordered_map<std::string,std::unordered_map<std::string,bool>>& boneMeshIsVisibles_)
{
	for ( const std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->meshes )
	{
		std::unordered_map<std::string,bool> lMeshIsVisibles;
		mesh->CreateBoneMeshIsVisibles(lMeshIsVisibles);
		boneMeshIsVisibles_[ mesh->name ] = lMeshIsVisibles;
	}
}

void MeshGPUParticleAliceModel::InvisibleBoneMesh(std::unordered_map<std::string,std::unordered_map<std::string,bool>>& boneMeshIsVisibles_,const std::string& meshName_,const std::string& boneName_,bool root_)
{
	auto lModelItr = find_if(modelData->meshes.begin(),modelData->meshes.end(),[ & ] (const std::unique_ptr<MeshGPUParticleModelMesh,std::default_delete<MeshGPUParticleModelMesh>>& modelData)
	{
		return modelData->name == meshName_;
	});

	if ( lModelItr == modelData->meshes.end() )
	{
		AliceDebugPrint("そのはメッシュは存在しません");
	}
	else
	{
		std::unordered_map<std::string,bool>& lBoneMeshIsVisibles = boneMeshIsVisibles_[ meshName_ ];
		MeshGPUParticleModelMesh* mesh = lModelItr->get();

		mesh->InvisibleBoneMesh(lBoneMeshIsVisibles,boneName_,root_);
	}
}

void MeshGPUParticleAliceModel::VisibleBoneMesh(std::unordered_map<std::string,std::unordered_map<std::string,bool>>& boneMeshIsVisibles_,const std::string& meshName_,const std::string& boneName_,bool root_)
{

	auto lModelItr = find_if(modelData->meshes.begin(),modelData->meshes.end(),[ & ] (const std::unique_ptr<MeshGPUParticleModelMesh,std::default_delete<MeshGPUParticleModelMesh>>& modelData)
{
	return modelData->name == meshName_;
});

	if ( lModelItr == modelData->meshes.end() )
	{
		AliceDebugPrint("そのはメッシュは存在しません");
	}
	else
	{
		std::unordered_map<std::string,bool>& lBoneMeshIsVisibles = boneMeshIsVisibles_[ meshName_ ];
		MeshGPUParticleModelMesh* mesh = lModelItr->get();

		mesh->VisibleBoneMesh(lBoneMeshIsVisibles,boneName_,root_);
	}
}

BoneMesh* MeshGPUParticleAliceModel::GetBoneMesh(const std::string& meshName_,const std::string& boneName_)
{
	auto lModelItr = find_if(modelData->meshes.begin(),modelData->meshes.end(),[ & ] (const std::unique_ptr<MeshGPUParticleModelMesh,std::default_delete<MeshGPUParticleModelMesh>>& modelData)
	{
		return modelData->name == meshName_;
	});

	if ( lModelItr == modelData->meshes.end() )
	{
		AliceDebugPrint("そのはメッシュは存在しません");

		return nullptr;

	}
	else
	{
		MeshGPUParticleModelMesh* mesh = lModelItr->get();

		return mesh->GetBoneMesh(boneName_);
	}
}

size_t MeshGPUParticleAliceModel::GetVerticeSize()
{
	return modelData->verticeSize;
}

const ReturnMotionNode* MeshGPUParticleAliceModel::PFindNodeAnim(const AliceMotionData* pAnimation_,const std::string& strNodeName_)
{
	return pAnimation_->GetMotion(strNodeName_);
}

void MeshGPUParticleAliceModel::AddBoneMesh(std::vector<std::unique_ptr<BoneMesh>>& boneMeshs_,const PosNormUvTangeColSkin& ver_,const std::vector<Bone>& bone_)
{
	uint32_t lBoneIndex = GetBoneIndex(ver_);

	const Bone& lBone = bone_[ lBoneIndex ];

	auto lModelItr = find_if(boneMeshs_.begin(),boneMeshs_.end(),[ & ] (std::unique_ptr<BoneMesh,std::default_delete<BoneMesh>>& modelData)
		{
			return modelData->boneName == lBone.name;
		});


	if ( lModelItr == boneMeshs_.end() )
	{
		std::unique_ptr<BoneMesh> lMessh = std::make_unique<BoneMesh>();

		lMessh->boneName = lBone.name;
		lMessh->vertices.push_back(ver_);
		lMessh->bone = (Bone*)&bone_[ lBoneIndex ];
		lMessh->centerPos.x += ver_.position.x;
		lMessh->centerPos.y += ver_.position.y;
		lMessh->centerPos.z += ver_.position.z;

		boneMeshs_.push_back(std::move(lMessh));
	}
	else
	{
		BoneMesh* lMessh = lModelItr->get();
		lMessh->vertices.push_back(ver_);
		lMessh->centerPos.x += ver_.position.x;
		lMessh->centerPos.y += ver_.position.y;
		lMessh->centerPos.z += ver_.position.z;
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

void MeshGPUParticleAliceModel::PReadChildren(const std::vector<Node>& nodes_,const std::vector<std::unique_ptr<BoneMesh>>& boneMeshs_,BoneMesh* boneMesh_)
{
	auto lNodelItr = find_if(nodes_.begin(),nodes_.end(),[ & ] (const Node& data)
		{
			return data.name == boneMesh_->boneName;
		});

	if ( lNodelItr != nodes_.end() )
	{
		const Node& lNode = *lNodelItr;

		if ( !lNode.childrens.empty() )
		{
			for ( Node* children : lNode.childrens )
			{
				auto lBoneMeshlItr = find_if(boneMeshs_.begin(),boneMeshs_.end(),[ & ] (const std::unique_ptr<BoneMesh,std::default_delete<BoneMesh>>& modelData)
				{
					return modelData->boneName == children->name;
				});

				if ( lBoneMeshlItr != boneMeshs_.end() )
				{
					boneMesh_->parent.push_back(lBoneMeshlItr->get());
				}
			}

		}
	}
}
