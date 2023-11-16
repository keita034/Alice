#include "MeshGPUParticleAliceModel.h"

IMultiAdapters* MeshGPUParticleAliceModel::sMultiAdapters;

std::unordered_map<std::string,std::unique_ptr<MeshGPUParticleAliceModelData>> MeshGPUParticleAliceModel::sModelDatas;


const std::vector<std::unique_ptr<MeshGPUParticleModelMesh>>& MeshGPUParticleAliceModel::GetMeshs()
{
	return modelData->meshes;
}

void MeshGPUParticleAliceModel::AnimationUpdate(const AliceMotionData* animation_)
{
	AliceMathF::Matrix4 lMatIdentity = AliceMathF::MakeIdentity();

	Node* lPtrNode = &modelData->nodes->data()[ 0 ];

	for ( std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->meshes )
	{
		PReadNodeHeirarchy(mesh.get(),animation_,lPtrNode,lMatIdentity);

		mesh->Update();
	}
}

void MeshGPUParticleAliceModel::AnimationUpdate(AliceBlendTree* blendTree_)
{
	AliceMathF::Matrix4 lMatIdentity = AliceMathF::MakeIdentity();

	Node* lPtrNode = &modelData->nodes->data()[ 0 ];

	for ( std::unique_ptr<MeshGPUParticleModelMesh>& mesh : modelData->meshes )
	{
		PReadNodeHeirarchy(mesh.get(),blendTree_,lPtrNode,lMatIdentity);

		mesh->Update();
	}
}

void MeshGPUParticleAliceModel::SetModel(AliceModel* model_,BufferType type)
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
			lData->nodes = &lModelData->nodes;
			lData->filePath = lModelData->filePath;
			lData->globalInverseTransform = lModelData->globalInverseTransform;

			lData->postureMatBuff = CreateUniqueConstantBuffer(sizeof(AliceMathF::Matrix4),static_cast< AdaptersIndex >( type ));

			for ( std::unique_ptr<ModelMesh>& mesh : lModelData->meshes )
			{
				std::unique_ptr<MeshGPUParticleModelMesh> lMesh = std::make_unique<MeshGPUParticleModelMesh>();

				lMesh->name = mesh->name;
				lMesh->nodeName = mesh->nodeName;
				lMesh->vertices = &mesh->vertices;
				lMesh->bones = &mesh->bones;
				lMesh->vecBones = &mesh->vecBones;
				lMesh->node = mesh->node;

				lMesh->vertexBuffer = CreateUniqueVertexBuffer(mesh->vertices.size(),sizeof(mesh->vertices[0]),static_cast<AdaptersIndex>(type),mesh->vertices.data());
				lMesh->vertexBuffer->CreateSRV();
				lMesh->constBoneBuffer = CreateUniqueConstantBuffer(sizeof(BoneData),static_cast< AdaptersIndex >( type ));

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

void MeshGPUParticleAliceModel::SSetMultiAdapters(IMultiAdapters* multiAdapters_)
{
	sMultiAdapters = multiAdapters_;
}

void MeshGPUParticleModelMesh::Update()
{
	for ( size_t i = 0; i < vecBones->size(); i++ )
	{
		bonedata.boneMat[ i ] = vecBones->data()[ i ].matrix;
	}

	constBoneBuffer->Update(bonedata.boneMat.data());
}

const std::vector<PosNormUvTangeColSkin>& MeshGPUParticleModelMesh::GetVertices()
{
	return *vertices;
}

D3D12_GPU_DESCRIPTOR_HANDLE MeshGPUParticleModelMesh::GetSRVAddress()
{
	return vertexBuffer->GetSRVAddress();
}

void MeshGPUParticleAliceModel::Finalize()
{
	sMultiAdapters = nullptr;


	sModelDatas.clear();
}

void MeshGPUParticleAliceModel::PReadNodeHeirarchy(MeshGPUParticleModelMesh* mesh_,const AliceMotionData* pAnimation_,const Node* pNode_,const AliceMathF::Matrix4& mxParentTransform_)
{
	AliceMathF::Matrix4 lMatNodeTransformation = AliceMathF::MakeIdentity();
	lMatNodeTransformation = pNode_->transform;

	std::string lStrNodeName = pNode_->name;

	const ReturnMotionNode* lPtrNodeAnim = PFindNodeAnim(pAnimation_,lStrNodeName);

	if ( lPtrNodeAnim )
	{
		//スケーリング
		AliceMathF::Vector3 lScaling = lPtrNodeAnim->scalingKeys;
		AliceMathF::Matrix4 lMatScaling;
		lMatScaling.MakeScaling(lScaling);

		//回転角
		AliceMathF::Quaternion lRotation = lPtrNodeAnim->rotationKeys;
		AliceMathF::Matrix4 lMatRotation = lRotation.Rotate();

		//移動
		AliceMathF::Vector3 lTranslation = lPtrNodeAnim->positionKeys;
		AliceMathF::Matrix4 lMatTranslation;
		lMatTranslation.MakeTranslation(lTranslation);

		lMatNodeTransformation = lMatScaling * lMatRotation * lMatTranslation;
	}

	AliceMathF::Matrix4 mxGlobalTransformation = lMatNodeTransformation * mxParentTransform_;

	AliceMathF::Matrix4 lOffsetMatirx;
	AliceMathF::Matrix4 lMatWorld;
	if ( mesh_->bones->find(lStrNodeName) != mesh_->bones->end() )
	{
		lOffsetMatirx = mesh_->bones->at(lStrNodeName)->offsetMatirx;

		lMatWorld = lOffsetMatirx * mxGlobalTransformation * modelData->globalInverseTransform;

		mesh_->bones->at(lStrNodeName)->matrix = lMatWorld;

	}

	for ( uint32_t i = 0; i < pNode_->childrens.size(); i++ )
	{
		PReadNodeHeirarchy(mesh_,pAnimation_,pNode_->childrens[ i ],mxGlobalTransformation);
	}
}

void MeshGPUParticleAliceModel::PReadNodeHeirarchy(MeshGPUParticleModelMesh* mesh_,AliceBlendTree* blendTree_,const Node* pNode_,const AliceMathF::Matrix4& mxParentTransform_)
{

	AliceMathF::Matrix4 lMatNodeTransformation = AliceMathF::MakeIdentity();
	lMatNodeTransformation = pNode_->transform;

	std::string lStrNodeName = pNode_->name;

	const ReturnMotionNode* lPtrNodeAnim = blendTree_->GetMotion(lStrNodeName);

	if ( lPtrNodeAnim )
	{
		//スケーリング
		AliceMathF::Vector3 lScaling = lPtrNodeAnim->scalingKeys;
		AliceMathF::Matrix4 lMatScaling;
		lMatScaling.MakeScaling(lScaling);

		//回転角
		AliceMathF::Quaternion lRotation = lPtrNodeAnim->rotationKeys;
		AliceMathF::Matrix4 lMatRotation = lRotation.Rotate();

		//移動
		AliceMathF::Vector3 lTranslation = lPtrNodeAnim->positionKeys;
		AliceMathF::Matrix4 lMatTranslation;
		lMatTranslation.MakeTranslation(lTranslation);

		lMatNodeTransformation = lMatScaling * lMatRotation * lMatTranslation;
	}

	AliceMathF::Matrix4 mxGlobalTransformation = lMatNodeTransformation * mxParentTransform_;

	AliceMathF::Matrix4 lOffsetMatirx;
	AliceMathF::Matrix4 lMatWorld;
	if ( mesh_->bones->find(lStrNodeName) != mesh_->bones->end() )
	{
		lOffsetMatirx = mesh_->bones->at(lStrNodeName)->offsetMatirx;

		lMatWorld = lOffsetMatirx * mxGlobalTransformation * modelData->globalInverseTransform;

		mesh_->bones->at(lStrNodeName)->matrix = lMatWorld;

	}

	for ( uint32_t i = 0; i < pNode_->childrens.size(); i++ )
	{
		PReadNodeHeirarchy(mesh_,blendTree_,pNode_->childrens[ i ],mxGlobalTransformation);
	}
}

const ReturnMotionNode* MeshGPUParticleAliceModel::PFindNodeAnim(const AliceMotionData* pAnimation_,const std::string& strNodeName_)
{
	return pAnimation_->GetMotion(strNodeName_);
}
