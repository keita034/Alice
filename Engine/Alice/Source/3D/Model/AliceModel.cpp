#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<Windows.h>
#include<atldef.h>

#pragma warning(pop)

#include<AliceModel.h>
#include<AliceFileStream.h>
#include<AliceFunctionUtility.h>

ID3D12Device* AliceModel::sDevice;
ID3D12GraphicsCommandList* AliceModel::sCmdList;
Light* AliceModel::sLight;
std::vector<std::string>AliceModel::sFilePaths;
std::unordered_map<std::string, std::unique_ptr<AliceModelData>> AliceModel::sModelDatas;
uint32_t AliceModel::sModelCount = 0u;

uint32_t AliceModel::SCreateModel(const std::string& fileDirectoryPath_)
{
	std::string lDirectoryPath = fileDirectoryPath_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lModelItr = find_if(sModelDatas.begin(), sModelDatas.end(), [&](std::pair<const std::string, std::unique_ptr<AliceModelData, std::default_delete<AliceModelData>>>& modelData)
		{
			return modelData.second->filePath == lDirectoryPath;
		});

	if (lModelItr == sModelDatas.end())
	{
		uint32_t lModelHandle = sModelCount;

		std::unique_ptr<AliceModelData> lModelData;
		lModelData = std::make_unique<AliceModelData>();

		{
			std::vector<std::string> lFiles;

			std::string lFilePath;

			lFiles = AliceFunctionUtility::getFileNames(lDirectoryPath);

			//ディレクトリからFBXファイルを探す
			for (std::string file : lFiles)
			{
				if (file.find(".alpb") != std::string::npos || file.find(".alp") != std::string::npos)
				{
					lFilePath = file;
				}
			}

			std::string lFileExtension = AliceFunctionUtility::FileExtension(lFilePath);

			if (lFileExtension == "alpb")
			{
				AliceFileStream::SLoadAlicePolygonBinaryData(lFilePath, lModelData.get());
			}
			else if (lFileExtension == "alp")
			{
				AliceFileStream::SLoadAlicePolygonData(lFilePath, lModelData.get());
			}

			lModelData->postureMatBuff = CreateUniqueConstantBuffer(sizeof(AliceMathF::Matrix4));

			AliceMathF::Matrix4 lTmp = AliceMathF::Matrix4();
			lModelData->postureMatBuff->Update(&lTmp);

			for (size_t i = 0; i < lModelData->nodes.size(); i++)
			{
				if (lModelData->nodes[i].parent)
				{
					auto lParentItr = std::find_if(lModelData->nodes.begin(), lModelData->nodes.end(), [&](Node& node)
						{
							return node.name == lModelData->nodes[i].parent->name;
						});

					lParentItr->childrens.push_back(&lModelData->nodes[i]);
				}

			}

			//バッファ生成
			for (size_t i = 0; i < lModelData->meshes.size(); i++)
			{
				for (size_t j = 0; j < lModelData->meshes[i]->vecBones.size(); j++)
				{
					lModelData->meshes[i]->bones[lModelData->meshes[i]->vecBones[j].name] = &lModelData->meshes[i]->vecBones[j];
				}

				lModelData->meshes[i]->CreateBuffer();
			}
		}

		lModelData->modelHandle = sModelCount;

		lModelData->filePath = lDirectoryPath;

		sModelDatas[lDirectoryPath] = std::move(lModelData);

		sFilePaths[sModelCount] = lDirectoryPath;

		sModelCount++;

		return lModelHandle;
	}
	else
	{

		uint32_t lModelHandle = sModelDatas[lDirectoryPath]->modelHandle;

		return lModelHandle;
	}

	return INT32_MAX;
}

void AliceModel::Draw(const Transform& transform_, const AliceMotionData* animation_, float frame_, const Material* material_)
{
	if (animation_)
	{
		if (!material_)
		{
			modelMaterialData = MaterialManager::SGetMaterial("DefaultFbxAnimation");
		}
		else
		{
			modelMaterialData = material_;
		}

		AnimationUpdate(animation_, frame_);

		PModelAnimationDraw(transform_);
	}
	else
	{
		if (!material_)
		{
			modelMaterialData = MaterialManager::SGetMaterial("DefaultFbx");;
		}
		else
		{
			modelMaterialData = material_;
		}

		PModelDraw(transform_);
	}

	modelData->IsAnime = false;
}

void AliceModel::AnimationUpdate(const AliceMotionData* animation_, float frame_)
{
	modelData->IsAnime = true;

	modelData->vertexInitialize = true;

	AliceMathF::Matrix4 lMatIdentity = AliceMathF::MakeIdentity();

	Node* lPtrNode = &modelData->nodes[0];

	float lTicksPerSecond = (animation_->motionData->ticksPerSecond != 0 ? animation_->motionData->ticksPerSecond : 25.0f);

	float lTimeInTicks = frame_ * lTicksPerSecond;
	float lAnimationTime = fmod(lTimeInTicks, animation_->motionData->duration);

	for (std::unique_ptr<ModelMesh>& mesh : modelData->meshes)
	{
		PReadNodeHeirarchy(mesh.get(), animation_, lAnimationTime, lPtrNode, lMatIdentity);

		mesh->Update();
	}
}

uint32_t AliceModel::SCreateToonModel(const std::string& fileDirectoryPath_, const std::string& rampFilePath_)
{
	std::string lDirectoryPath = fileDirectoryPath_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lModelItr = find_if(sModelDatas.begin(), sModelDatas.end(), [&](std::pair<const std::string, std::unique_ptr<AliceModelData, std::default_delete<AliceModelData>>>& modelData)
		{
			return modelData.second->filePath == lDirectoryPath;
		});

	if (lModelItr == sModelDatas.end())
	{
		uint32_t lModelHandle = sModelCount;

		std::unique_ptr<AliceModelData> lModelData;
		lModelData = std::make_unique<AliceModelData>();

		{
			std::vector<std::string> lFiles;

			std::string lFilePath;

			lFiles = AliceFunctionUtility::getFileNames(lDirectoryPath);

			//ディレクトリからFBXファイルを探す
			for (std::string file : lFiles)
			{
				if (file.find(".alpb") != std::string::npos || file.find(".alp") != std::string::npos)
				{
					lFilePath = file;
				}
			}

			if (lFilePath == "")
			{
				assert(0);
			}

			std::string lFileExtension = AliceFunctionUtility::FileExtension(lFilePath);

			if (lFileExtension == "alpb")
			{
				AliceFileStream::SLoadAlicePolygonBinaryData(lFilePath, lModelData.get());
			}
			else if (lFileExtension == "alp")
			{
				AliceFileStream::SLoadAlicePolygonData(lFilePath, lModelData.get());
			}

			lModelData->postureMatBuff = CreateUniqueConstantBuffer(sizeof(AliceMathF::Matrix4));

			AliceMathF::Matrix4 lTmp = AliceMathF::Matrix4();
			lModelData->postureMatBuff->Update(&lTmp);

			for (size_t i = 0; i < lModelData->nodes.size(); i++)
			{
				if (lModelData->nodes[i].parent)
				{
					auto lParentItr = std::find_if(lModelData->nodes.begin(), lModelData->nodes.end(), [&](Node& node)
						{
							return node.name == lModelData->nodes[i].parent->name;
						});

					lParentItr->childrens.push_back(&lModelData->nodes[i]);
				}

			}

			//バッファ生成
			for (size_t i = 0; i < lModelData->meshes.size(); i++)
			{
				for (size_t j = 0; j < lModelData->meshes[i]->vecBones.size(); j++)
				{
					lModelData->meshes[i]->bones[lModelData->meshes[i]->vecBones[j].name] = &lModelData->meshes[i]->vecBones[j];
				}

				lModelData->meshes[i]->CreateBuffer();
			}
		}

		lModelData->modelHandle = sModelCount;

		lModelData->filePath = lDirectoryPath;

		lModelData->isToon = true;
		uint32_t rampHnadel = TextureManager::SLoad(rampFilePath_);
		lModelData->rampTex = TextureManager::SGetTextureData(rampHnadel);

		sModelDatas[lDirectoryPath] = std::move(lModelData);

		sFilePaths[sModelCount] = lDirectoryPath;

		sModelCount++;

		return lModelHandle;
	}
	else
	{
		if (!sModelDatas[lDirectoryPath]->isToon)
		{
			printf("NotToonModel");

		}
		uint32_t lModelHandle = sModelDatas[lDirectoryPath]->modelHandle;

		return lModelHandle;

	}
}

void AliceModel::SCommonInitialize(DirectX12Core* directX12Core_)
{
	sDevice = directX12Core_->GetDevice();
	sCmdList = directX12Core_->GetCommandList();

	sFilePaths.resize(sMAX_MODEL);
}

bool AliceModel::TransTexture(const std::string& materialName_, const std::string& textureName_, TextureData* textureData_)
{
	//メッシュの中からマテリアルを探す
	auto lMaterialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](std::unique_ptr<ModelMesh>& model)
		{
			return model->material.name == materialName_;
		});

	if (lMaterialItr != modelData->meshes.end())
	{
		std::unique_ptr<ModelMesh>& mesh = *lMaterialItr;

		//マテリアルの中からテクスチャ探す
		auto textureItr = std::find_if(mesh->textures.begin(), mesh->textures.end(), [&](TextureData* p)
			{
				return p->path == textureName_;
			});

		if (textureItr != mesh->textures.end())
		{
			*textureItr = textureData_;

			return true;
		}
	}

	return false;
}

bool AliceModel::TransTexture(const std::string& materialName_, size_t textureIndex_, TextureData* textureData_)
{
	//メッシュの中からマテリアルを探す
	auto lMaterialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](std::unique_ptr<ModelMesh>& modelData)
		{
			return modelData->material.name == materialName_;
		});

	if (lMaterialItr != modelData->meshes.end())
	{
		//マテリアルの中からテクスチャ探す
		std::unique_ptr<ModelMesh>& lMesh = *lMaterialItr;

		lMesh->textures[textureIndex_] = textureData_;

		return true;
	}

	return false;
}

bool AliceModel::FlipUV(const std::string& materialName_, bool inverseU_, bool inverseV_)
{
	//メッシュの中からマテリアルを探す
	auto lMaterialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](std::unique_ptr<ModelMesh>& mesh)
		{
			return mesh->material.name == materialName_;
		});

	if (lMaterialItr != modelData->meshes.end())
	{
		std::unique_ptr<ModelMesh>& lMesh = *lMaterialItr;

		for (PosNormUvTangeColSkin& vertice : lMesh->vertices)
		{
			if (inverseU_)
			{
				vertice.uv.x *= -1.0f;
			}

			if (inverseV_)
			{
				vertice.uv.y *= -1.0f;
			}
		}

		lMesh->dirtyFlag = true;
		lMesh->vertexBuffer->Update(lMesh->vertices.data());
	}
	return false;
}

bool AliceModel::rotationUV(const std::string& materialName_, float angle_)
{
	//メッシュの中からマテリアルを探す
	auto lMaterialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](std::unique_ptr<ModelMesh>& mesh)
		{
			return mesh->material.name == materialName_;
		});

	if (lMaterialItr != modelData->meshes.end())
	{
		std::unique_ptr<ModelMesh>& mesh = *lMaterialItr;

		AliceMathF::Matrix3 lMat;
		lMat.MakeRotation(angle_);

		for (PosNormUvTangeColSkin& vertice : mesh->vertices)
		{
			vertice.uv = AliceMathF::Vec2Mat3Mul(vertice.uv, lMat);
		}

		mesh->vertexBuffer->Update(mesh->vertices.data());

		mesh->dirtyFlag = true;
	}
	return false;
}

void AliceModel::InitializeVertex()
{
	for (std::unique_ptr<ModelMesh>& mesh : modelData->meshes)
	{
		if (mesh->dirtyFlag)
		{
			mesh->InitializeVertex();
			mesh->dirtyFlag = false;
		}
	}
}

const std::vector<std::unique_ptr<ModelMesh>>& AliceModel::GetMeshs()
{
	return modelData->meshes;
}

void AliceModel::SSetLight(Light* light_)
{
	sLight = light_;
}

void AliceModel::SetModel(uint32_t lModelHandle_)
{
	modelData = sModelDatas[sFilePaths[lModelHandle_]].get();
}

void AliceModel::PReadNodeHeirarchy(ModelMesh* mesh_, const AliceMotionData* pAnimation_, float animationTime_, const Node* pNode_, const AliceMathF::Matrix4& mxParentTransform_)
{
	AliceMathF::Matrix4 lMatNodeTransformation = AliceMathF::MakeIdentity();
	lMatNodeTransformation = pNode_->transform;

	AliceMathF::Matrix4 lMatThisTrans = lMatNodeTransformation.Transpose();

	std::string lStrNodeName = pNode_->name;

	const MotionNode* lPtrNodeAnim = PFindNodeAnim(pAnimation_, lStrNodeName);

	if (lPtrNodeAnim)
	{
		//スケーリング
		AliceMathF::Vector3 lScaling = {};
		PCalcInterpolatedScaling(lScaling, animationTime_, lPtrNodeAnim);
		AliceMathF::Matrix4 lMatScaling;
		lMatScaling.MakeScaling(lScaling);

		//回転角
		AliceMathF::Quaternion lRotation = {};
		PCalcInterpolatedRotation(lRotation, animationTime_, lPtrNodeAnim);
		AliceMathF::Matrix4 lMatRotation = lRotation.Rotate();

		//移動
		AliceMathF::Vector3 lTranslation = {};
		PCalcInterpolatedPosition(lTranslation, animationTime_, lPtrNodeAnim);
		AliceMathF::Matrix4 lMatTranslation;
		lMatTranslation.MakeTranslation(lTranslation);

		lMatNodeTransformation = lMatScaling * lMatRotation * lMatTranslation;
	}

	AliceMathF::Matrix4 mxGlobalTransformation = lMatNodeTransformation * mxParentTransform_;

	AliceMathF::Matrix4 lOffsetMatirx;
	AliceMathF::Matrix4 lMatWorld;
	if (mesh_->bones.find(lStrNodeName) != mesh_->bones.end())
	{
		lOffsetMatirx = mesh_->bones[lStrNodeName]->offsetMatirx;

		lMatWorld = lOffsetMatirx * mxGlobalTransformation * modelData->globalInverseTransform;

		mesh_->bones[lStrNodeName]->matrix = lMatWorld;

	}

	for (uint32_t i = 0; i < pNode_->childrens.size(); i++)
	{
		PReadNodeHeirarchy(mesh_, pAnimation_, animationTime_, pNode_->childrens[i], mxGlobalTransformation);
	}
}

const MotionNode* AliceModel::PFindNodeAnim(const AliceMotionData* pAnimation_, const std::string& strNodeName_)
{
	for (uint32_t i = 0; i < pAnimation_->motionData->channels.size(); i++)
	{
		if (std::string(pAnimation_->motionData->channels[i].name) == strNodeName_)
		{
			return &pAnimation_->motionData->channels[i];
		}
	}

	return nullptr;
}

void AliceModel::PCalcInterpolatedScaling(AliceMathF::Vector3& mxOut_, float animationTime_, const MotionNode* pNodeAnim_)
{
	if (pNodeAnim_->scalingKeys.size() == 1)
	{
		mxOut_ = pNodeAnim_->scalingKeys[0].value;
		return;
	}

	uint32_t lScalingIndex = 0;
	if (!PFindScaling(animationTime_, pNodeAnim_, lScalingIndex))
	{
		mxOut_ = AliceMathF::Vector3(1.0f, 1.0f, 1.0f);
		return;
	}

	uint32_t lNextScalingIndex = (lScalingIndex + 1);
	ATLASSERT(lNextScalingIndex < pNodeAnim_->scalingKeys.size());

	float lDeltaTime = (pNodeAnim_->scalingKeys[lNextScalingIndex].time - pNodeAnim_->scalingKeys[lScalingIndex].time);

	float lFactor = (animationTime_ - pNodeAnim_->scalingKeys[lScalingIndex].time) / lDeltaTime;
	ATLASSERT(lFactor >= 0.0f && lFactor <= 1.0f);

	mxOut_ = AliceMathF::Vector3Lerp(pNodeAnim_->scalingKeys[lScalingIndex].value, pNodeAnim_->scalingKeys[lNextScalingIndex].value, lFactor);

}

bool AliceModel::PFindScaling(float animationTime_, const MotionNode* pNodeAnim_, uint32_t& nScalingIndex_)
{
	nScalingIndex_ = 0;
	if (!(pNodeAnim_->scalingKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim_->scalingKeys.size() - 1; i++)
	{
		if ((animationTime_ >= pNodeAnim_->scalingKeys[i].time) && (animationTime_ < pNodeAnim_->scalingKeys[i + 1].time))
		{
			nScalingIndex_ = static_cast<uint32_t>(i);
			return true;
		}
	}

	return false;
}

void AliceModel::PCalcInterpolatedRotation(AliceMathF::Quaternion& mxOut_, float animationTime_, const MotionNode* pNodeAnim_)
{
	if (pNodeAnim_->rotationKeys.size() == 1)
	{
		mxOut_ = pNodeAnim_->rotationKeys[0].value;
		return;
	}

	uint32_t lRotationIndex = 0;
	if (!PFindRotation(animationTime_, pNodeAnim_, lRotationIndex))
	{
		mxOut_ = AliceMathF::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	uint32_t lNextRotationIndex = (lRotationIndex + 1);
	ATLASSERT(lNextRotationIndex < pNodeAnim_->rotationKeys.size());

	float lDeltaTime = pNodeAnim_->rotationKeys[lNextRotationIndex].time - pNodeAnim_->rotationKeys[lRotationIndex].time;
	float lFactor = (animationTime_ - pNodeAnim_->rotationKeys[lRotationIndex].time) / lDeltaTime;
	ATLASSERT(lFactor >= 0.0f && lFactor <= 1.0f);

	AliceMathF::QuaternionSlerp(mxOut_, pNodeAnim_->rotationKeys[lRotationIndex].value, pNodeAnim_->rotationKeys[lNextRotationIndex].value, lFactor);

}

bool AliceModel::PFindRotation(float animationTime_, const MotionNode* pNodeAnim_, uint32_t& nRotationIndex_)
{
	nRotationIndex_ = 0;
	if (!(pNodeAnim_->rotationKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim_->rotationKeys.size() - 1; i++)
	{

		if ((animationTime_ >= pNodeAnim_->rotationKeys[i].time) && (animationTime_ < pNodeAnim_->rotationKeys[i + 1].time))
		{
			nRotationIndex_ = static_cast<uint32_t>(i);
			return true;
		}
	}

	return false;
}

void AliceModel::PCalcInterpolatedPosition(AliceMathF::Vector3& mxOut_, float animationTime_, const MotionNode* pNodeAnim_)
{
	if (pNodeAnim_->positionKeys.size() == 1)
	{
		mxOut_ = pNodeAnim_->positionKeys[0].value;
		return;
	}

	uint32_t lPositionIndex = 0;
	if (!PFindPosition(animationTime_, pNodeAnim_, lPositionIndex))
	{
		mxOut_ = AliceMathF::Vector3(0.0f, 0.0f, 0.0f);
		return;
	}

	uint32_t lNextPositionIndex = (lPositionIndex + 1);

	ATLASSERT(lNextPositionIndex < pNodeAnim_->positionKeys.size());
	float lDeltaTime = (pNodeAnim_->positionKeys[lNextPositionIndex].time - pNodeAnim_->positionKeys[lPositionIndex].time);
	float lFactor = (animationTime_ - pNodeAnim_->positionKeys[lPositionIndex].time) / lDeltaTime;
	ATLASSERT(lFactor >= 0.0f && lFactor <= 1.0f);

	mxOut_ = AliceMathF::Vector3Lerp(pNodeAnim_->positionKeys[lPositionIndex].value, pNodeAnim_->positionKeys[lNextPositionIndex].value, lFactor);
}

bool AliceModel::PFindPosition(float animationTime_, const MotionNode* pNodeAnim_, uint32_t& nPosIndex_)
{
	nPosIndex_ = 0;
	if (!(pNodeAnim_->positionKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim_->positionKeys.size() - 1; i++)
	{
		if ((animationTime_ >= pNodeAnim_->positionKeys[i].time) && (animationTime_ < pNodeAnim_->positionKeys[i + 1].time))
		{
			nPosIndex_ = static_cast<uint32_t>(i);
			return true;
		}
	}

	return false;
}

void AliceModel::PModelDraw(const Transform& transform_)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		sCmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		sCmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		if (modelData->meshes[i]->node)
		{
			modelData->postureMatBuff->Update(&modelData->meshes[i]->node->globalTransform);
		}

		sCmdList->SetGraphicsRootConstantBufferView(3, modelData->postureMatBuff->GetAddress());
		modelData->meshes[i]->Draw(sCmdList, transform_, sLight);
	}
}

void AliceModel::PModelAnimationDraw(const Transform& transform_)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		sCmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		sCmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());


		modelData->meshes[i]->AnimDraw(sCmdList, transform_, sLight);
	}
}

AliceModelData::AliceModelData()
{
}

AliceModelData::~AliceModelData()
{
}
