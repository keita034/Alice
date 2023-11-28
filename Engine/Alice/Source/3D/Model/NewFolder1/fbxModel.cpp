#include "fbxModel.h"
#include"FbxLoader.h"

std::shared_ptr<ComputeRelation> fbxModel::computeRelation;
Microsoft::WRL::ComPtr<ID3D12Device> fbxModel::device;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> fbxModel::cmdList;

void fbxModel::Initialize()
{
	postureMatBuff = std::make_unique<ConstantBuffer>();

	postureMatBuff->Create(sizeof(AliceMathF::Matrix4));

	AliceMathF::Matrix4 tmp = AliceMathF::Matrix4();
	postureMatBuff->Update(&tmp);
}

void fbxModel::Draw(Transform* transform, Material* material)
{
	if (!material)
	{
		modelMaterialData = DefaultMaterial::GetDefaultMaterial()->DEFAULT_FBX_MATERIAL.get();
	}
	else
	{
		modelMaterialData = material;
	}

	for (size_t i = 0; i < meshes.size(); i++)
	{

		if (!IsAnime && vertexInitialize)
		{
			if (i == meshes.size() - 1)
			{
				vertexInitialize = false;
			}

			meshes[i].InitializeVertexPos();

		}

		meshes[i].DrawBgin();

		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		if (IsAnime)
		{
			AliceMathF::Matrix4 identity = AliceMathF::MakeIdentity();
			postureMatBuff->Update(&identity);
		}
		else
		{
			if (meshes[i].node)
			{
				postureMatBuff->Update(&meshes[i].node->globalTransform);
			}
		}


		cmdList->SetGraphicsRootConstantBufferView(3, postureMatBuff->GetAddress());
		meshes[i].Draw(cmdList.Get(), transform, light);
	}

	IsAnime = false;
}

void fbxModel::AnimationUpdate(const fbxAnimation* animation, float frame)
{
	IsAnime = true;

	vertexInitialize = true;

	AliceMathF::Matrix4 mxIdentity = AliceMathF::MakeIdentity();
	aiNode* pNode = mScene->mRootNode;
	aiAnimation* pAnimation = animation->mAnimation->mAnimations[0];

	FLOAT TicksPerSecond = (FLOAT)(pAnimation->mTicksPerSecond != 0 ? pAnimation->mTicksPerSecond : 25.0f);

	FLOAT TimeInTicks = frame * TicksPerSecond;
	FLOAT AnimationTime = fmod(TimeInTicks, (FLOAT)pAnimation->mDuration);

	for (ModelMesh& mesh : meshes)
	{
		ReadNodeHeirarchy(&mesh, pAnimation, AnimationTime, pNode, mxIdentity);

		UINT nNumBones = (UINT)mesh.bones.size();

		for (UINT i = 0; i < nNumBones; i++)
		{
			mesh.vecBones[i].matrix = mesh.bones[mesh.vecBones[i].name]->matrix;
		}

		mesh.Update(computeRelation.get(), cmdList.Get());
	}


}

void fbxModel::Create(const char* filePath, bool smoothing, bool inverseU, bool inverseV, bool animeFlag)
{
	static_cast<void>(smoothing);

	FbxLoader::GetInstance()->LoadModel(this, filePath, inverseU, inverseV, animeFlag);

	//バッファ生成
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].CreateBuffer();
	}
}

void fbxModel::CommonInitialize()
{
	device = DirectX12Core::GetInstance()->GetDevice();
	cmdList = DirectX12Core::GetInstance()->GetCommandList();

	//初期化
	computeRelation = std::make_shared<ComputeRelation>();

	//ルートシグネチャの初期化
	computeRelation->rootSignature = std::make_unique<RootSignature>();
	//ComputeShader用
	computeRelation->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0

	computeRelation->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	//計算された頂点データ用
	computeRelation->rootSignature->Add(RootSignature::RangeType::UAV, 0);//u0
	//生成
	computeRelation->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//シェーダーの初期化
	computeRelation->computeShader = std::make_unique<Shader>();
	//シェーダー読み込み
	computeRelation->computeShader->Create("Resources/Shaders/3D/Model/GpuSkinningCS.hlsl", "main", "cs_5_0", Shader::ShaderType::CS);

	//パイプラインステート初期化
	computeRelation->computePipelineState = std::make_shared<ComputePipelineState>();
	//シェーダーをセット
	computeRelation->computePipelineState->SetShader(*computeRelation->computeShader->GetShader());
	//ルートシグネチャのセット
	computeRelation->computePipelineState->SetRootSignature(computeRelation->rootSignature.get());
	//パイプラインステートの生成
	computeRelation->computePipelineState->Create(DirectX12Core::GetInstance()->GetDevice().Get());
}

bool fbxModel::TransTexture(const std::string& materialName, const std::string& textureName, TextureData* textureData)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(meshes.begin(), meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != meshes.end())
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

bool fbxModel::TransTexture(const std::string& materialName, size_t textureIndex, TextureData* textureData)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(meshes.begin(), meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != meshes.end())
	{
		//マテリアルの中からテクスチャ探す
		
			materialItr->textures[textureIndex] = textureData;

			return true;
	}

	return false;
}

bool fbxModel::FlipUV(const std::string& materialName, bool inverseU, bool inverseV)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(meshes.begin(), meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != meshes.end())
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

bool fbxModel::rotationUV(const std::string& materialName, float angle)
{
	//メッシュの中からマテリアルを探す
	auto materialItr = std::find_if(meshes.begin(), meshes.end(), [&](ModelMesh& p)
		{
			return p.material.name == materialName;
		});

	if (materialItr != meshes.end())
	{

		AliceMathF::Matrix3 mat;
		mat.MakeRotation(angle);

		for (PosNormUvTangeCol& vertice : materialItr->vertice)
		{
			vertice.uv = AliceMathF::Vec2Mat3Mul(vertice.uv, mat);
		}

		materialItr->vertexBuffer->Update(materialItr->vertice.data());

		materialItr->dirtyFlag = true;
	}
	return false;
}

void fbxModel::InitializeVertex()
{
	for (ModelMesh& mesh : meshes)
	{
		if (mesh.dirtyFlag)
		{
			mesh.InitializeVertex();
			mesh.dirtyFlag = false;
		}
	}
}

const std::vector<ModelMesh>& fbxModel::GetMeshs()
{
	return meshes;
}

void fbxModel::ReadNodeHeirarchy(ModelMesh* mesh, const aiAnimation* pAnimation, float AnimationTime, const aiNode* pNode, const AliceMathF::Matrix4& mxParentTransform)
{
	AliceMathF::Matrix4 mxNodeTransformation = AliceMathF::MakeIdentity();
	mxNodeTransformation = pNode->mTransformation;
	mxNodeTransformation = mxNodeTransformation.Transpose();

	AliceMathF::Matrix4 mxThisTrans = mxNodeTransformation.Transpose();

	std::string strNodeName(pNode->mName.data);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, strNodeName);

	if (pNodeAnim)
	{
		//スケーリング
		AliceMathF::Vector3 vScaling = {};
		CalcInterpolatedScaling(vScaling, AnimationTime, pNodeAnim);
		AliceMathF::Matrix4 mxScaling;
		mxScaling.MakeScaling(vScaling);

		//回転角
		AliceMathF::Vector4 vRotationQ = {};
		CalcInterpolatedRotation(vRotationQ, AnimationTime, pNodeAnim);
		AliceMathF::Matrix4 mxRotationM = AliceMathF::Quaternion(vRotationQ).Rotate();

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
			MatrixMultiply(globalInverseTransform);

		mesh->bones[strNodeName]->matrix = matirx;

	}

	for (UINT i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(mesh
			, pAnimation
			, AnimationTime
			, pNode->mChildren[i]
			, mxGlobalTransformation);
	}
}

aiNodeAnim* fbxModel::FindNodeAnim(const aiAnimation* pAnimation, const std::string& strNodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++)
	{
		if (std::string(pAnimation->mChannels[i]->mNodeName.data) == strNodeName)
		{
			return pAnimation->mChannels[i];
		}
	}

	return nullptr;
}

void fbxModel::CalcInterpolatedScaling(AliceMathF::Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		mxOut = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	UINT ScalingIndex = 0;
	if (!FindScaling(AnimationTime, pNodeAnim, ScalingIndex))
	{
		mxOut = AliceMathF::Vector3(1.0f, 1.0f, 1.0f);
		return;
	}

	UINT NextScalingIndex = (ScalingIndex + 1);
	ATLASSERT(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = AliceMathF::Vector3Lerp(pNodeAnim->mScalingKeys[ScalingIndex].mValue, pNodeAnim->mScalingKeys[NextScalingIndex].mValue, Factor);

}

bool fbxModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nScalingIndex)
{
	nScalingIndex = 0;
	if (!(pNodeAnim->mNumScalingKeys > 0))
	{
		return FALSE;
	}

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if ((AnimationTime >= (float)pNodeAnim->mScalingKeys[i].mTime)
			&& (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime))
		{
			nScalingIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

void fbxModel::CalcInterpolatedRotation(AliceMathF::Vector4& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		mxOut = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	UINT RotationIndex = 0;
	if (!FindRotation(AnimationTime, pNodeAnim, RotationIndex))
	{
		mxOut = AliceMathF::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	UINT NextRotationIndex = (RotationIndex + 1);
	ATLASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime
		- pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	AliceMathF::QuaternionSlerp(mxOut
		, pNodeAnim->mRotationKeys[RotationIndex].mValue
		, pNodeAnim->mRotationKeys[NextRotationIndex].mValue
		, Factor);

}

bool fbxModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nRotationIndex)
{
	nRotationIndex = 0;
	if (!(pNodeAnim->mNumRotationKeys > 0))
	{
		return FALSE;
	}

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{

		if ((AnimationTime >= (float)pNodeAnim->mRotationKeys[i].mTime)
			&& (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime))
		{
			nRotationIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

void fbxModel::CalcInterpolatedPosition(AliceMathF::Vector3& mxOut, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		mxOut = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	UINT PositionIndex = 0;
	if (!FindPosition(AnimationTime, pNodeAnim, PositionIndex))
	{
		mxOut = AliceMathF::Vector3( 0.0f, 0.0f, 0.0f );
		return;
	}

	UINT NextPositionIndex = (PositionIndex + 1);

	ATLASSERT(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	ATLASSERT(Factor >= 0.0f && Factor <= 1.0f);

	mxOut = AliceMathF::Vector3Lerp(pNodeAnim->mPositionKeys[PositionIndex].mValue, pNodeAnim->mPositionKeys[NextPositionIndex].mValue, Factor);
}

bool fbxModel::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, UINT& nPosIndex)
{
	nPosIndex = 0;
	if (!(pNodeAnim->mNumPositionKeys > 0))
	{
		return FALSE;
	}

	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if ((AnimationTime >= (float)pNodeAnim->mPositionKeys[i].mTime)
			&& (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime))
		{
			nPosIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}