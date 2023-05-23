#include "FbxLoader.h"
#include"DirectX12Core.h"
#include"DefaultMaterial.h"
#include"AliceFunctionUtility.h"

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize()
{

}

void FbxLoader::Finalize()
{

}

void FbxLoader::LoadModel(fbxModel* model, const std::string& modelDirectory, bool inverseU_, bool inverseV_, bool animeFlag)
{
	if (modelDirectory.empty())
	{
		assert(0);
	}

	std::vector<std::string> files;

	std::string filePath;

	files = AliceFunctionUtility::getFileNames(modelDirectory);

	//ディレクトリからFBXファイルを探す
	for (std::string file : files)
	{
		if (file.find(".fbx") != std::string::npos || file.find(".FBX") != std::string::npos)
		{
			filePath = file;
		}
	}

	model->mScene = aiImportFile(filePath.c_str(), ASSIMP_LOAD_FLAG_DEFAULT);

	model->name = model->mScene->mName.C_Str();

	directoryPath = modelDirectory;

	inverseU = inverseU_;
	inverseV = inverseV_;

	anicmatrixion = animeFlag;

	if (model->mScene == nullptr)
	{
		assert(0);
	}
	else
	{
		UINT32 nodeNum = 0;
		GetNodeNum(model->mScene->mRootNode, nodeNum);
		model->nodes.reserve(nodeNum);
		model->globalInverseTransform = model->mScene->mRootNode->mTransformation.Transpose();
		// ルートノードから順に解析してモデルに流し込む
		PraseNodeRecurive(model, model->mScene->mRootNode);
	}

	//PrintModelData(model);
}

void FbxLoader::GetNodeNum(const aiNode* node, UINT32& num)
{
	if (node->mChildren)
	{
		for (UINT32 i = 0; i < node->mNumChildren; i++)
		{
			GetNodeNum(node->mChildren[i], num);
		}
	}

	num++;
}

void FbxLoader::PraseNodeRecurive(fbxModel* model, aiNode* fbxNode, Node* parent)
{
	//// ノード名を取得
	// モデルにノードを追加
	model->nodes.emplace_back();
	Node& node = model->nodes.back();
	node.name = fbxNode->mName.C_Str();
	node.transform =
	{
		fbxNode->mTransformation.a1, fbxNode->mTransformation.a2, fbxNode->mTransformation.a3, fbxNode->mTransformation.a4,
		fbxNode->mTransformation.b1, fbxNode->mTransformation.b2, fbxNode->mTransformation.b3, fbxNode->mTransformation.b4,
		fbxNode->mTransformation.c1, fbxNode->mTransformation.c2, fbxNode->mTransformation.c3, fbxNode->mTransformation.c4,
		fbxNode->mTransformation.d1, fbxNode->mTransformation.d2, fbxNode->mTransformation.d3, fbxNode->mTransformation.d4,
	};

	//転置行列
	node.transform = node.transform.Transpose();

	//グローバル変形行列の計算
	node.globalTransform = node.transform;
	if (parent)
	{
		node.parent = parent;
		// 親の変形を乗算
		node.globalTransform *= parent->globalTransform;
	}

	// FBXノードのメッシュ情報を解析
	for (UINT32 i = 0; i < fbxNode->mNumMeshes; i++)
	{
		aiMesh* fbxNodeAttribute = model->mScene->mMeshes[fbxNode->mMeshes[i]];

		if (fbxNodeAttribute)
		{
			model->meshes.emplace_back();
			model->meshes.back().name = fbxNodeAttribute->mName.C_Str();
			model->meshes.back().nodeName = node.name;
			model->meshes.back().node = &node;
			ParseMesh(model, fbxNodeAttribute);
		}
	}


	// 子ノードに対して再帰呼び出し
	for (UINT32 i = 0; i < fbxNode->mNumChildren; i++)
	{
		PraseNodeRecurive(model, fbxNode->mChildren[i], &node);
	}
}

void FbxLoader::ParseMesh(fbxModel* model, aiMesh* fbxMesh)
{
	// 頂点座標読み取り
	ParseMeshVertices(model, fbxMesh);
	// 面を構成するデータの読み取り
	ParseMeshFaces(model, fbxMesh);
	// マテリアルの読み取り
	ParseMaterial(model, fbxMesh, model->mScene->mMaterials[fbxMesh->mMaterialIndex]);
	if (fbxMesh->HasBones())
	{
		//スキニング情報の読み取り
		ParseSkin(model, fbxMesh);
	}

}

void FbxLoader::ParseMeshVertices(fbxModel* model, aiMesh* mesh)
{
	auto& modelMesh = model->meshes.back();

	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	//頂点数に応じてメモリを確保
	// 必要数だけ頂点データ配列を確保
	PosNormUvTangeColSkin vert = {};
	modelMesh.vertices.resize(mesh->mNumVertices, vert);

	//頂点
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		PosNormUvTangeColSkin& vertex = modelMesh.vertices[i];

		//座標
		// FBXメッシュの全頂点座標をモデル内の配列にコピーする。
		aiVector3D* position = &(mesh->mVertices[i]);
		vertex.position = AliceMathF::Vector4(position->x, position->y, position->z, 1.0f);

		//法線
		aiVector3D* normal = &(mesh->mNormals[i]);
		vertex.normal = AliceMathF::Vector3(normal->x, normal->y, normal->z);

		//接空間
		aiVector3D* tangent = (mesh->HasTangentsAndBitangents()) ? &(mesh->mTangents[i]) : &zero3D;
		vertex.tangent = AliceMathF::Vector3(tangent->x, tangent->y, tangent->z);
	}
}

void FbxLoader::ParseMeshFaces(fbxModel* model, aiMesh* mesh)
{
	auto& modelMesh = model->meshes.back();

	auto& vertices = modelMesh.vertices;
	auto& indices = modelMesh.indices;

	// 面の数
	UINT32 polygonCount = mesh->mNumVertices;

	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	// 面ごとの情報読み取り
	for (UINT32 i = 0; i < polygonCount; i++)
	{
		// 頂点法線読込
		auto& vertex = vertices[i];
		aiVector3D* normal = &(mesh->mNormals[i]);
		vertex.normal = AliceMathF::Vector3(normal->x, normal->y, normal->z);

		// テクスチャUV読込
		// 0番決め打ちで読込
		aiVector3D* uv = (mesh->HasTextureCoords(0)) ? &(mesh->mTextureCoords[0][i]) : &zero3D;
		// 反転オプションがあったらUVを反転させる
		if (inverseU)
		{
			uv->x = 1 - uv->x;
		}
		if (inverseV)
		{
			uv->y = 1 - uv->y;
		}
		vertex.uv = AliceMathF::Vector2(uv->x, uv->y);
	}

	//インデックス数に応じてメモリを確保
	indices.resize(static_cast<size_t>(mesh->mNumFaces) * 3);

	//インデックス
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];


		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices[static_cast<size_t>(i) * 3 + j] = face.mIndices[j];
		}
	}
}

void FbxLoader::ParseMaterial(fbxModel* model, aiMesh* mesh, aiMaterial* material)
{
	auto& modelMesh = model->meshes.back();

	aiColor4D zeroColor(1.0f, 1.0f, 1.0f, 1.0f);

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		PosNormUvTangeColSkin& vertex = modelMesh.vertices[i];
		//色
		aiColor4D* color = (mesh->HasVertexColors(0)) ? &(mesh->mColors[0][i]) : &zeroColor;
		vertex.color = AliceMathF::Vector4(color->r, color->g, color->b, color->a);
	}

	//環境光係数
	aiColor3D ambient(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	modelMesh.material.diffuse = AliceMathF::Vector3(ambient.r, ambient.g, ambient.b);

	//拡散反射光係数
	aiColor3D diffuse(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	modelMesh.material.diffuse = AliceMathF::Vector3(diffuse.r, diffuse.g, diffuse.b);

	//スペキュラー係数
	aiColor3D specular(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, specular);
	modelMesh.material.specular = AliceMathF::Vector3(specular.r, specular.g, specular.b);

	aiString str;
	material->Get(AI_MATKEY_NAME, str);
	modelMesh.material.name = str.C_Str();

	// ディフューズマップ
	std::vector<TextureData*> diffuseMaps = LoadMatrixerialTextures(material, aiTextureType_DIFFUSE, "Diffuse", model->mScene);
	// 法線マップ
	std::vector<TextureData*> normalMaps = LoadMatrixerialTextures(material, aiTextureType_HEIGHT, "Normal", model->mScene);

	modelMesh.textures.insert(modelMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	modelMesh.texturesNormal.insert(modelMesh.texturesNormal.end(), normalMaps.begin(), normalMaps.end());
}

void FbxLoader::ParseSkin(fbxModel* model, aiMesh* mesh)
{
	//ボーン番号とスキンウェイトのペア
	struct WeightSet
	{
		UINT index;
		float weight;
	};

	//二次元配列(ジャグ配列)
	//list:頂点が影響を受けるボーンの全リスト
	//vector:それを全頂点分
	std::vector<std::list<WeightSet>>weightLists(model->meshes.back().vertices.size());

	auto& vertices = model->meshes.back().vertices;

	//ボーン無ければ終了
	if (mesh->mNumBones == 0)
	{
		return;
	}

	//全てのボーンについて
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		auto& meshBone = mesh->mBones[i];

		//ボーン自体のノードの名前を取得
		const char* boneName = meshBone->mName.C_Str();

		//新しくボーンを追加し、追加したボーンの参照を得る
		Bone bone;

		bone.name = boneName;

		//FBXから初期姿勢行列を取得する

		//初期姿勢行列の逆行列を得る
		bone.offsetMatirx = meshBone->mOffsetMatrix.Transpose();
		bone.index = i;

		model->meshes.back().vecBones.push_back(bone);
		model->meshes.back().bones[bone.name] = &model->meshes.back().vecBones.back();

		//影響を受ける全頂点について
		for (size_t k = 0; k < meshBone->mNumWeights; k++)
		{
			//頂点番号
			UINT vertIndex = meshBone->mWeights[k].mVertexId;
			//スキンウェイト
			float weight = meshBone->mWeights[k].mWeight;
			//その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}

	//各頂点について処理
	for (size_t j = 0; j < vertices.size(); j++)
	{
		//頂点のウェイトから最も大きい4つを選択
		auto& weightList = weightLists[j];

		size_t weightArrayIndex = 0;
		//降順ソート済みのウェイトリストから

		for (auto& weightSet : weightList)
		{
			//頂点データに書き込み
			vertices[j].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[j].boneWeight[weightArrayIndex] = weightSet.weight;

			//4つに達したら修了
			if (++weightArrayIndex >= MAX_BONE_INDICES)
			{
				break;
			}

		}
	}
}

std::vector<TextureData*> FbxLoader::LoadMatrixerialTextures(aiMaterial* cmatrix, aiTextureType type, std::string typeName, const aiScene* scene_)
{
	std::vector<TextureData*> textures;

	for (size_t i = 0; i < cmatrix->GetTextureCount(type); i++)
	{
		aiString str;
		cmatrix->GetTexture(type, static_cast<UINT>(i), &str);
		TextureData* texture;
		{
			std::string filename = ExtractFileName(std::string(str.C_Str()));
			filename = directoryPath + '\\' + filename;

			if (str.data[0] == 46)
			{
				int id = atoi(&str.data[1]);
				std::string path = (const CHAR*)scene_->mTextures[id]->pcData;

				uint32_t handle = TextureManager::Load(path);
				texture = TextureManager::GetTextureData(handle);
			}
			else
			{
				uint32_t handle = TextureManager::Load(filename);
				texture = TextureManager::GetTextureData(handle);
			}

		}

		//texture->path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}

void FbxLoader::PrintModelData(fbxModel* model)
{
	//printf("モデル名::%s = {\n", model->name.c_str());

	//printf(" メッシュ = {\n");
	//printf("  数 : %zu\n", model->meshes.size());
	//for (size_t i = 0; i < model->meshes.size(); i++)
	//{
	//	printf("  %s = {\n", model->meshes[i].name.c_str());
	//	printf("   頂点数 : %zu\n", model->meshes[i].vertices.size());
	//	printf("   インデック数 : %zu\n", model->meshes[i].indices.size());

	//	printf("   マテリアル名 %s {\n", model->meshes[i].material.name.c_str());
	//	printf("    環境光係数 : { %f, %f, %f}\n", model->meshes[i].material.ambient.x, model->meshes[i].material.ambient.y, model->meshes[i].material.ambient.z);
	//	printf("    散反射光係数 : { %f, %f, %f}\n", model->meshes[i].material.diffuse.x, model->meshes[i].material.diffuse.y, model->meshes[i].material.diffuse.z);
	//	printf("    スペキュラー係数 : { %f, %f, %f}\n", model->meshes[i].material.specular.x, model->meshes[i].material.specular.y, model->meshes[i].material.specular.z);
	//	printf("   }\n");

	//	printf("   ノード名 %s {\n", model->meshes[i].nodeName.c_str());
	//	printf("    親 :  %s\n", model->meshes[i].node->parent->name.c_str());
	//	printf("   }\n");

	//	printf("   テクスチャ数 %zu\n", model->meshes[i].textures.size());
	//	printf("   テクスチャ = {\n");
	//	for (size_t j = 0; j < model->meshes[i].textures.size(); j++)
	//	{
	//		printf("    テクスチャ名 %s {\n", model->meshes[i].textures[j]->path.c_str());
	//		printf("     ファイルパス : %s\n", model->meshes[i].textures[j]->path.c_str());
	//		printf("     横幅 : %zu\n", model->meshes[i].textures[j]->width);
	//		printf("     縦幅 : %zu\n", model->meshes[i].textures[j]->height);
	//		printf("     GPUハンドル : %zu\n", model->meshes[i].textures[j]->gpuHandle.ptr);
	//		printf("    }\n");
	//	}
	//	printf("   }\n");

	//	printf("   法線テクスチャ数 %zu\n", model->meshes[i].texturesNormal.size());
	//	printf("   法線テクスチャ = {\n");
	//	for (size_t j = 0; j < model->meshes[j].texturesNormal.size(); j++)
	//	{
	//		printf("    テクスチャ名 %s {\n", model->meshes[i].texturesNormal[j]->path.c_str());
	//		printf("     ファイルパス : %s\n", model->meshes[i].texturesNormal[j]->path.c_str());
	//		printf("     横幅 : %zu\n", model->meshes[i].texturesNormal[j]->width);
	//		printf("     縦幅 : %zu\n", model->meshes[i].texturesNormal[j]->height);
	//		printf("     GPUハンドル : %zu\n", model->meshes[i].texturesNormal[j]->gpuHandle.ptr);
	//		printf("    }\n");
	//	}
	//	printf("   }\n");

	//	printf("  }\n");
	//}

	//printf(" ノード = {\n");
	//printf("  数 : %zu\n", model->nodes.size());
	//for (size_t i = 0; i < model->nodes.size(); i++)
	//{
	//	printf("  %s = {\n", model->nodes[i].name.c_str());
	//	if (model->nodes[i].parent)
	//	{
	//		printf("   親 : %s\n", model->nodes[i].parent->name.c_str());
	//	}


	//	printf("  }\n");
	//}
	//printf(" }\n");

	//printf("}\n");
}

void FbxLoader::LoadAnimation(fbxAnimation* animation, const std::string& modelDirectory, const std::string& name)
{
	if (modelDirectory.empty())
	{
		assert(0);
	}

	std::vector<std::string> files;

	std::string filePath;

	files = AliceFunctionUtility::getFileNames(modelDirectory);

	//ディレクトリからFBXファイルを探す
	for (std::string file : files)
	{
		if (file.find(".fbx") != std::string::npos || file.find(".FBX") != std::string::npos)
		{
			filePath = file;
		}
	}

	UINT flag = 0;
	flag |= aiProcess_Triangulate;//三角面化
	flag |= aiProcess_CalcTangentSpace;//接線ベクトル生成
	flag |= aiProcess_GenSmoothNormals;//スムージングベクトル生成
	flag |= aiProcess_GenUVCoords;//非マッピングを適切なUV座標に変換
	flag |= aiProcess_RemoveRedundantMaterials;//冗長なマテリアルを削除
	flag |= aiProcess_OptimizeMeshes;//メッシュ数を最適化
	flag |= aiProcess_MakeLeftHanded;//ノードを左手座標系に

	animation->name = name;

	animation->mAnimation = aiImportFile(filePath.c_str(), flag);


	if (animation->mAnimation->mAnimations[0]->mTicksPerSecond != 0.0)
	{
		animation->ticksPerSecond = static_cast<float>(animation->mAnimation->mAnimations[0]->mTicksPerSecond);
	}
	else
	{
		animation->ticksPerSecond = 25.0f;
	}
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;

	// 区切り文字 '\\' が出てくる一番最後の部分を検索。
	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	// 区切り文字'/'が出てくる一番最後の部分を参照
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}
