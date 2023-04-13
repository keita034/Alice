﻿#include "AliceModel.h"
#include"AliceFileStream.h"
#include<AliceFunctionUtility.h>

ID3D12Device* AliceModel::device;
ID3D12GraphicsCommandList* AliceModel::cmdList;
Light* AliceModel::light;
std::vector<std::string>AliceModel::filePaths;
std::unordered_map<std::string, std::unique_ptr<AliceModelData>> AliceModel::modelDatas;
uint32_t AliceModel::modelCount = 0u;

uint32_t AliceModel::CreateModel(const std::string& fileDirectoryPath)
{
	std::string path = fileDirectoryPath;

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(modelDatas.begin(), modelDatas.end(), [&](std::pair<const std::string, std::unique_ptr<AliceModelData, std::default_delete<AliceModelData>>>& p)
		{
			return p.second->filePath == path;
		});

	if (itr == modelDatas.end())
	{

		uint32_t modelHandle = modelCount;

		std::unique_ptr<AliceModelData> data;
		data = std::make_unique<AliceModelData>();

		{
			std::vector<std::string> files;

			std::string filePath;

			files = AliceFunctionUtility::getFileNames(path);

			//ディレクトリからFBXファイルを探す
			for (std::string file : files)
			{
				if (file.find(".alpb") != std::string::npos || file.find(".alp") != std::string::npos)
				{
					filePath = file;
				}
			}

			std::string fileExtension = AliceFunctionUtility::FileExtension(filePath);

			if (fileExtension == "alpb")
			{
				AliceFileStream::LoadAlicePolygonBinaryData(filePath, data.get());
			}
			else if (fileExtension == "alp")
			{
				AliceFileStream::LoadAlicePolygonData(filePath, data.get());
			}

			data->postureMatBuff = std::make_unique<ConstantBuffer>();
			data->postureMatBuff->Create(sizeof(AliceMathF::Matrix4));

			AliceMathF::Matrix4 tmp = AliceMathF::Matrix4();
			data->postureMatBuff->Update(&tmp);

			for (size_t i = 0; i < data->nodes.size(); i++)
			{
				if (data->nodes[i].parent)
				{
					auto itr = std::find_if(data->nodes.begin(), data->nodes.end(), [&](Node& node)
						{
							return node.name == data->nodes[i].parent->name;
						});

					itr->childrens.push_back(&data->nodes[i]);
				}

			}

			//バッファ生成
			for (size_t i = 0; i < data->meshes.size(); i++)
			{
				for (size_t j = 0; j < data->meshes[i].vecBones.size(); j++)
				{
					data->meshes[i].bones[data->meshes[i].vecBones[j].name] = &data->meshes[i].vecBones[j];
				}

				data->meshes[i].CreateBuffer();
			}
		}

		data->modelHandle = modelCount;

		data->filePath = path;

		modelDatas[path] = std::move(data);

		filePaths[modelCount] = path;

		modelCount++;

		return modelHandle;
	}
	else
	{

		uint32_t modelHandle = modelDatas[path]->modelHandle;

		return modelHandle;
	}

	return -1;
}

void AliceModel::Draw(Transform& transform, const AliceMotionData* animation, float frame, Material* material)
{
	if (animation)
	{
		if (!material)
		{
			modelMaterialData = MaterialManager::GetMaterial("DefaultFbxAnimation");
		}
		else
		{
			modelMaterialData = material;
		}

		AnimationUpdate(animation, frame);

		ModelAnimationDraw(transform, material);
	}
	else
	{
		if (!material)
		{
			modelMaterialData = MaterialManager::GetMaterial("DefaultFbx");;
		}
		else
		{
			modelMaterialData = material;
		}

		ModelDraw(transform, material);
	}

	modelData->IsAnime = false;
}

void AliceModel::AnimationUpdate(const AliceMotionData* animation, float frame)
{
	modelData->IsAnime = true;

	modelData->vertexInitialize = true;

	AliceMathF::Matrix4 mxIdentity = AliceMathF::MakeIdentity();

	Node* pNode = &modelData->nodes[0];

	float TicksPerSecond = (animation->motionData->ticksPerSecond != 0 ? animation->motionData->ticksPerSecond : 25.0f);

	float TimeInTicks = frame * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, animation->motionData->duration);

	for (ModelMesh& mesh : modelData->meshes)
	{
		ReadNodeHeirarchy(&mesh, animation, AnimationTime, pNode, mxIdentity);

		mesh.Update();
	}
}

void AliceModel::CommonInitialize()
{
	device = DirectX12Core::GetInstance()->GetDevice().Get();
	cmdList = DirectX12Core::GetInstance()->GetCommandList().Get();

	filePaths.resize(maxModel);
}

bool AliceModel::TransTexture(const std::string& materialName, const std::string& textureName, TextureData* textureData)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != modelData->meshes.end())
	{
		//マテリアルの中からテクスチャ探す
		auto textureItr = std::find_if(materialItr->textures.begin(), materialItr->textures.end(), [&](TextureData* p)
			{
				return p->path == textureName;
			});

		if (textureItr != materialItr->textures.end())
		{
			*textureItr = textureData;

			return true;
		}
	}

	return false;
}

bool AliceModel::TransTexture(const std::string& materialName, size_t textureIndex, TextureData* textureData)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != modelData->meshes.end())
	{
		//マテリアルの中からテクスチャ探す

		materialItr->textures[textureIndex] = textureData;

		return true;
	}

	return false;
}

bool AliceModel::FlipUV(const std::string& materialName, bool inverseU, bool inverseV)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != modelData->meshes.end())
	{
		for (PosNormUvTangeColSkin& vertice : materialItr->vertices)
		{
			if (inverseU)
			{
				vertice.uv.x *= -1.0f;
			}

			if (inverseV)
			{
				vertice.uv.y *= -1.0f;
			}
		}

		materialItr->dirtyFlag = true;
		materialItr->vertexBuffer->Update(materialItr->vertices.data());
	}
	return false;
}

bool AliceModel::rotationUV(const std::string& materialName, float angle)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(modelData->meshes.begin(), modelData->meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != modelData->meshes.end())
	{
		AliceMathF::Matrix3 mat;
		mat.MakeRotation(angle);

		for (PosNormUvTangeColSkin& vertice : materialItr->vertices)
		{
			vertice.uv = AliceMathF::Vec2Mat3Mul(vertice.uv, mat);
		}

		materialItr->vertexBuffer->Update(materialItr->vertices.data());

		materialItr->dirtyFlag = true;
	}
	return false;
}

void AliceModel::InitializeVertex()
{
	for (ModelMesh& mesh : modelData->meshes)
	{
		if (mesh.dirtyFlag)
		{
			mesh.InitializeVertex();
			mesh.dirtyFlag = false;
		}
	}
}

const std::vector<ModelMesh>& AliceModel::GetMeshs()
{
	return modelData->meshes;
}

void AliceModel::SetLight(Light* lightPtr)
{
	light = lightPtr;
}

void AliceModel::SetModel(uint32_t modelHandle)
{
	modelData = modelDatas[filePaths[modelHandle]].get();
}

void AliceModel::ReadNodeHeirarchy(ModelMesh* mesh, const AliceMotionData* pAnimation, float AnimationTime, const Node* pNode, const AliceMathF::Matrix4& mxParentTransform)
{
	AliceMathF::Matrix4 mxNodeTransformation = AliceMathF::MakeIdentity();
	mxNodeTransformation = pNode->transform;

	AliceMathF::Matrix4 mxThisTrans = mxNodeTransformation.Transpose();

	std::string strNodeName = pNode->name;

	const MotionNode* pNodeAnim = FindNodeAnim(pAnimation, strNodeName);

	if (pNodeAnim)
	{
		//スケーリング
		AliceMathF::Vector3 vScaling = {};
		CalcInterpolatedScaling(vScaling, AnimationTime, pNodeAnim);
		AliceMathF::Matrix4 mxScaling;
		mxScaling.MakeScaling(vScaling);

		//回転角
		AliceMathF::Quaternion vRotationQ = {};
		CalcInterpolatedRotation(vRotationQ, AnimationTime, pNodeAnim);
		AliceMathF::Matrix4 mxRotationM = vRotationQ.Rotate();

		//移動
		AliceMathF::Vector3 vTranslation = {};
		CalcInterpolatedPosition(vTranslation, AnimationTime, pNodeAnim);
		AliceMathF::Matrix4 mxTranslationM;
		mxTranslationM.MakeTranslation(vTranslation);

		mxNodeTransformation = mxScaling.MatrixMultiply(mxRotationM).MatrixMultiply(mxTranslationM);
	}

	AliceMathF::Matrix4 mxGlobalTransformation = mxNodeTransformation.MatrixMultiply(mxParentTransform);

	AliceMathF::Matrix4 offsetMatirx;
	AliceMathF::Matrix4 matirx;
	if (mesh->bones.find(strNodeName) != mesh->bones.end())
	{
		offsetMatirx = mesh->bones[strNodeName]->offsetMatirx;

		matirx
			= offsetMatirx.MatrixMultiply(mxGlobalTransformation).
			MatrixMultiply(modelData->globalInverseTransform);

		mesh->bones[strNodeName]->matrix = matirx;

	}

	for (UINT i = 0; i < pNode->childrens.size(); i++)
	{
		ReadNodeHeirarchy(mesh
			, pAnimation
			, AnimationTime
			, pNode->childrens[i]
			, mxGlobalTransformation);
	}
}

const MotionNode* AliceModel::FindNodeAnim(const AliceMotionData* pAnimation, const std::string& strNodeName)
{
	for (UINT i = 0; i < pAnimation->motionData->channels.size(); i++)
	{
		if (std::string(pAnimation->motionData->channels[i].name) == strNodeName)
		{
			return &pAnimation->motionData->channels[i];
		}
	}

	return nullptr;
}

void AliceModel::CalcInterpolatedScaling(AliceMathF::Vector3& mxOut, float AnimationTime, const MotionNode* pNodeAnim)
{
	if (pNodeAnim->scalingKeys.size() == 1)
	{
		mxOut = pNodeAnim->scalingKeys[0].value;
		return;
	}

	UINT ScalingIndex = 0;
	if (!FindScaling(AnimationTime, pNodeAnim, ScalingIndex))
	{
		mxOut = AliceMathF::Vector3(1.0f, 1.0f, 1.0f);
		return;
	}

	UINT NextScalingIndex = (ScalingIndex + 1);
	ATLASSERT(NextScalingIndex < pNodeAnim->scalingKeys.size());

	float DeltaTime = (pNodeAnim->scalingKeys[NextScalingIndex].time - pNodeAnim->scalingKeys[ScalingIndex].time);

	float Factor = (AnimationTime - pNodeAnim->scalingKeys[ScalingIndex].time) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = AliceMathF::Vector3Lerp(pNodeAnim->scalingKeys[ScalingIndex].value, pNodeAnim->scalingKeys[NextScalingIndex].value, Factor);

}

bool AliceModel::FindScaling(float AnimationTime, const MotionNode* pNodeAnim, UINT& nScalingIndex)
{
	nScalingIndex = 0;
	if (!(pNodeAnim->scalingKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim->scalingKeys.size() - 1; i++)
	{
		if ((AnimationTime >= pNodeAnim->scalingKeys[i].time) && (AnimationTime < pNodeAnim->scalingKeys[i + 1].time))
		{
			nScalingIndex = static_cast<UINT>(i);
			return true;
		}
	}

	return false;
}

void AliceModel::CalcInterpolatedRotation(AliceMathF::Quaternion& mxOut, float AnimationTime, const MotionNode* pNodeAnim)
{
	if (pNodeAnim->rotationKeys.size() == 1)
	{
		mxOut = pNodeAnim->rotationKeys[0].value;
		return;
	}

	UINT RotationIndex = 0;
	if (!FindRotation(AnimationTime, pNodeAnim, RotationIndex))
	{
		mxOut = AliceMathF::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	UINT NextRotationIndex = (RotationIndex + 1);
	ATLASSERT(NextRotationIndex < pNodeAnim->rotationKeys.size());

	float DeltaTime = pNodeAnim->rotationKeys[NextRotationIndex].time - pNodeAnim->rotationKeys[RotationIndex].time;
	float Factor = (AnimationTime - pNodeAnim->rotationKeys[RotationIndex].time) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	AliceMathF::QuaternionSlerp(mxOut, pNodeAnim->rotationKeys[RotationIndex].value, pNodeAnim->rotationKeys[NextRotationIndex].value, Factor);

}

bool AliceModel::FindRotation(float AnimationTime, const MotionNode* pNodeAnim, UINT& nRotationIndex)
{
	nRotationIndex = 0;
	if (!(pNodeAnim->rotationKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim->rotationKeys.size() - 1; i++)
	{

		if ((AnimationTime >= pNodeAnim->rotationKeys[i].time) && (AnimationTime < pNodeAnim->rotationKeys[i + 1].time))
		{
			nRotationIndex = static_cast<UINT>(i);
			return true;
		}
	}

	return false;
}

void AliceModel::CalcInterpolatedPosition(AliceMathF::Vector3& mxOut, float AnimationTime, const MotionNode* pNodeAnim)
{
	if (pNodeAnim->positionKeys.size() == 1)
	{
		mxOut = pNodeAnim->positionKeys[0].value;
		return;
	}

	UINT PositionIndex = 0;
	if (!FindPosition(AnimationTime, pNodeAnim, PositionIndex))
	{
		mxOut = AliceMathF::Vector3(0.0f, 0.0f, 0.0f);
		return;
	}

	UINT NextPositionIndex = (PositionIndex + 1);

	ATLASSERT(NextPositionIndex < pNodeAnim->positionKeys.size());
	float DeltaTime = (pNodeAnim->positionKeys[NextPositionIndex].time - pNodeAnim->positionKeys[PositionIndex].time);
	float Factor = (AnimationTime - pNodeAnim->positionKeys[PositionIndex].time) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = AliceMathF::Vector3Lerp(pNodeAnim->positionKeys[PositionIndex].value, pNodeAnim->positionKeys[NextPositionIndex].value, Factor);
}

bool AliceModel::FindPosition(float AnimationTime, const MotionNode* pNodeAnim, UINT& nPosIndex)
{
	nPosIndex = 0;
	if (!(pNodeAnim->positionKeys.size() > 0))
	{
		return false;
	}

	for (size_t i = 0; i < pNodeAnim->positionKeys.size() - 1; i++)
	{
		if ((AnimationTime >= pNodeAnim->positionKeys[i].time) && (AnimationTime < pNodeAnim->positionKeys[i + 1].time))
		{
			nPosIndex = static_cast<UINT>(i);
			return true;
		}
	}

	return false;
}

void AliceModel::ModelDraw(Transform& transform, Material* material)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		if (modelData->meshes[i].node)
		{
			modelData->postureMatBuff->Update(&modelData->meshes[i].node->globalTransform);
		}

		cmdList->SetGraphicsRootConstantBufferView(3, modelData->postureMatBuff->GetAddress());
		modelData->meshes[i].Draw(cmdList, transform, light);
	}
}

void AliceModel::ModelAnimationDraw(Transform& transform, Material* material)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());


		modelData->meshes[i].AnimDraw(cmdList, transform, light);
	}
}

AliceModelData::AliceModelData()
{
}

AliceModelData::~AliceModelData()
{
}
