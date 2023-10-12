#include<AliceFileStream.h>
#include<FileUtility.h>
#include<DefaultMaterial.h>

std::string AliceFileStream::sBbuff;
std::istringstream AliceFileStream::sLineData;
std::array<char,256>  AliceFileStream::cBuffC;
std::string AliceFileStream::sDirectoryPath;
float AliceFileStream::sMeshNum;
float AliceFileStream::sNodeNum;
#pragma region モデルデータ

#pragma region ASCII

bool AliceFileStream::SLoadAlicePolygonData(const std::string& path_,AliceModelData* model_)
{
	sDirectoryPath = AliceUtility::Fille::GetDirectoryPath(path_);

	//モデルデータ
	std::stringstream lModelData;
	//1行分も文字列を入れる変数
	std::string lLine;

	//ファイルを開く
	std::ifstream lFile(path_);
	if ( lFile.fail() )
	{
		return false;
	}

	//ファイルの内容を文字列をストリームにコピー
	lModelData << lFile.rdbuf();

	//ファイルを閉じる
	lFile.close();

	//一行取得
	getline(lModelData,lLine,'\n');

	if ( lLine == "<MODEL>" )
	{
		SPReadModelData(lModelData,lLine,model_);
	}

	return true;
}

void AliceFileStream::SPReadModelData(std::stringstream& data_,std::string& str_,AliceModelData* model_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}

		if ( str_ == lLine )
		{
			return;
		}

		if ( lLine == "<HEADER>" )
		{
			SPReadHesderData(data_,lLine,model_);
		}

		if ( lLine == "<NODES>" )
		{
			SPReadNodesData(data_,lLine,model_);
		}

		if ( lLine == "<MESHS>" )
		{
			SPReadMeshsData(data_,lLine,model_);
		}

	}

}

void AliceFileStream::SPReadMeshsData(std::stringstream& data_,std::string& str_,AliceModelData* model_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}

		std::istringstream lStream(lLine);

		for ( size_t i = 0; i < static_cast< size_t >( sMeshNum ); i++ )
		{
			std::unique_ptr<ModelMesh>lMesh = std::make_unique<ModelMesh>();

			SPReadMeshData(data_,lLine,lMesh,model_->nodes);

			//メッシュにテクスチャがなかった場合は白色のテクスチャを貼る
			if ( lMesh->textures.empty() )
			{
				//lMesh->textures.push_back(MaterialManager::SGetDefaultTexture());
			}

			model_->meshes.push_back(std::move(lMesh));

		}

		if ( str_ == lLine )
		{
			return;
		}
	}

}

void AliceFileStream::SPReadMeshData(std::stringstream& data_,std::string& str_,std::unique_ptr<ModelMesh>& mesh_,std::vector<Node>& nodes_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		std::istringstream lStream(lLine);


		if ( lLine == "" )
		{
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"") )
		{
			continue;
		}

		if ( str_ == lLine )
		{
			getline(data_,lLine,'\n');
			getline(data_,str_,'\n');
			return;
		}

		getline(lStream,lLine,':');

		if ( std::equal(lLine.begin(),lLine.end(),"Name") )
		{
			getline(lStream,lLine,':');

			SPReadString(lLine,mesh_->name);

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"NodeName") )
		{
			getline(lStream,lLine,':');

			SPReadString(lLine,mesh_->nodeName);

			std::vector<Node>::iterator lNodeItr;
			lNodeItr = std::find_if(nodes_.begin(),nodes_.end(),[ & ] (Node& node)
				{
					return node.name == mesh_->nodeName;
				});

			if ( lNodeItr != nodes_.end() )
			{
				mesh_->node = &*lNodeItr;
				continue;
			}
			else
			{
				mesh_->node = nullptr;
			}

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"VertexNum") )
		{
			getline(lStream,lLine,':');

			float lVertexNum;
			SPReadNumber(lLine,lVertexNum);
			mesh_->vertices.resize(static_cast< size_t >( lVertexNum ));

			for ( size_t i = 0; i < mesh_->vertices.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(8,lLine.size() - 9);

				SPReadVertex(lLine,mesh_->vertices[ i ]);
			}

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"IndexNum") )
		{
			getline(lStream,lLine,':');

			float lIndexNum;
			SPReadNumber(lLine,lIndexNum);
			mesh_->indices.resize(static_cast< size_t >( lIndexNum ));

			for ( size_t i = 0; i < mesh_->indices.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(6,lLine.size() - 6);

				SPReadNumber(lLine,mesh_->indices[ i ]);
			}

			continue;
		}
		if ( std::equal(lLine.begin(),lLine.end(),"TextureNum") )
		{
			getline(lStream,lLine,':');

			float lTextureNum;
			SPReadNumber(lLine,lTextureNum);
			mesh_->textures.resize(static_cast< size_t >( lTextureNum ));

			for ( size_t i = 0; i < mesh_->textures.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(8,lLine.size() - 8);

				std::string lFilepath;
				SPReadString(lLine,lFilepath);

				lFilepath = sDirectoryPath + lFilepath;

				mesh_->textures[ i ] = TextureManager::SGetTextureData(TextureManager::SLoad(lFilepath));
			}

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"TextureNormalNum") )
		{
			getline(lStream,lLine,':');

			float lTexturesNormalNum;
			SPReadNumber(lLine,lTexturesNormalNum);
			mesh_->texturesNormal.resize(static_cast< size_t >( lTexturesNormalNum ));

			for ( size_t i = 0; i < mesh_->texturesNormal.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(14,lLine.size() - 14);

				std::string lFilepath;
				SPReadString(lLine,lFilepath);

				lFilepath = sDirectoryPath + lFilepath;

				mesh_->texturesNormal[ i ] = TextureManager::SGetTextureData(TextureManager::SLoad(lFilepath));
			}

			continue;
		}


		if ( std::equal(lLine.begin(),lLine.end(),"BoneNum") )
		{
			getline(lStream,lLine,':');

			float lBoneNum;
			SPReadNumber(lLine,lBoneNum);
			mesh_->vecBones.resize(static_cast< size_t > ( lBoneNum ));

			for ( size_t i = 0; i < mesh_->vecBones.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(6,lLine.size() - 7);

				SPReadBoneData(lLine,mesh_->vecBones[ i ]);
			}

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"Material") )
		{
			getline(lStream,lLine,':');

			lLine = lLine.substr(1,lLine.size() - 2);

			SPReadMaterial(lLine,mesh_->material);

		}
	}
}

void AliceFileStream::SPReadHesderData(std::stringstream& data_,std::string& str_,AliceModelData* model_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}
		std::istringstream lStream(lLine);

		if ( str_ == lStream.str() )
		{
			return;
		}

		getline(lStream,lLine,':');

		if ( std::equal(lLine.begin(),lLine.end(),"Name") )
		{
			getline(lStream,lLine,':');

			SPReadString(lLine,model_->name);

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"CanAnimation") )
		{
			getline(lStream,lLine,'\n');
			SPReadFlag(lLine,model_->canAnimation);
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"IsAnime") )
		{
			getline(lStream,lLine,'\n');
			SPReadFlag(lLine,model_->IsAnime);
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"GlobalInverseTransform") )
		{
			getline(lStream,lLine,':');
			SPReadMatrix(lLine,model_->globalInverseTransform);
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"NodeNun") )
		{
			getline(lStream,lLine,':');

			float lNodeNun;
			SPReadNumber(lLine,lNodeNun);
			model_->nodes.resize(static_cast< size_t >( lNodeNun ));
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"MeshNum") )
		{
			getline(lStream,lLine,':');

			SPReadNumber(lLine,sMeshNum);

			model_->meshes.reserve(static_cast< size_t >( sMeshNum ));
			continue;
		}
	}
}

void AliceFileStream::SPReadVertex(const std::string& strVertex_,PosNormUvTangeColSkin& vertex_)
{
	std::istringstream lStream(strVertex_);
	std::string lString;

	getline(lStream,lString,'|');
	SPReadVector3(lString,vertex_.position);

	getline(lStream,lString,'|');
	SPReadVector3(lString,vertex_.normal);

	getline(lStream,lString,'|');
	SPReadVector2(lString,vertex_.uv);

	getline(lStream,lString,'|');
	SPReadVector3(lString,vertex_.tangent);

	getline(lStream,lString,'|');
	SPReadVector4(lString,vertex_.color);

	getline(lStream,lString,'|');
	SPReadBoneIndex(lString,vertex_.boneIndex);

	getline(lStream,lString,'|');
	SPReadBoneWeight(lString,vertex_.boneWeight);
}

void AliceFileStream::SPReadMaterial(const std::string& strMaterial_,ModelMaterial& modelMaterial_)
{
	std::istringstream lStream(strMaterial_);
	std::string lString;

	getline(lStream,lString,'|');
	SPReadString(lString,modelMaterial_.name);

	getline(lStream,lString,'|');
	SPReadVector3(lString,modelMaterial_.ambient);

	getline(lStream,lString,'|');
	SPReadVector3(lString,modelMaterial_.diffuse);

	getline(lStream,lString,'|');
	SPReadVector3(lString,modelMaterial_.specular);

	getline(lStream,lString,'|');
	SPReadVector3(lString,modelMaterial_.emission);

	getline(lStream,lString,'|');
	SPReadNumber(lString,modelMaterial_.shininess);

	getline(lStream,lString,'|');
	SPReadNumber(lString,modelMaterial_.alpha);

	getline(lStream,lString,'|');
	SPReadString(lString,modelMaterial_.textureFiename);
}

void AliceFileStream::SPReadNodesData(std::stringstream& data_,std::string& str_,AliceModelData* model_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}

		std::istringstream lStream(lLine);

		for ( size_t i = 0; i < model_->nodes.size(); i++ )
		{
			SPReadNodeData(data_,lLine,model_->nodes[ i ],model_);
		}

		if ( str_ == lLine )
		{
			return;
		}
	}
}

void AliceFileStream::SPReadNodeData(std::stringstream& data_,std::string& str_,Node& node_,AliceModelData* model_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}

		std::istringstream lStream(lLine);

		if ( str_ == lLine )
		{
			getline(data_,lLine,'\n');
			getline(data_,str_,'\n');
			return;
		}

		getline(lStream,lLine,':');

		if ( lLine == "Name" )
		{
			getline(lStream,lLine,'\n');

			SPReadString(lLine,node_.name);

			continue;
		}

		if ( lLine == "Transform" )
		{
			getline(lStream,lLine,':');
			SPReadMatrix(lLine,node_.transform);
			continue;
		}

		if ( lLine == "GlobalTransform" )
		{
			getline(lStream,lLine,':');
			SPReadMatrix(lLine,node_.globalTransform);
			continue;
		}

		if ( lLine == "Parent" )
		{
			getline(lStream,lLine,'\n');
			std::string lNodeName;
			SPReadString(lLine,lNodeName);

			SPReadParentData(lNodeName,node_,model_->nodes);
			continue;
		}
	}
}

void AliceFileStream::SPReadParentData(const std::string& name_,Node& node_,std::vector<Node>& nodes_)
{
	if ( name_ == "" )
	{
		node_.parent = nullptr;
		return;
	}

	std::vector<Node>::iterator lNodeItr;
	lNodeItr = std::find_if(nodes_.begin(),nodes_.end(),[ & ] (Node& node)
		{
			return node.name == name_;
		});

	if ( lNodeItr != nodes_.end() )
	{
		node_.parent = &*lNodeItr;
		return;
	}
	else
	{
		node_.parent = nullptr;
	}
}

void AliceFileStream::SPReadBoneData(const std::string& strBone_,Bone& bone_)
{
	std::istringstream lStream(strBone_);
	std::string lString;

	getline(lStream,lString,'|');
	SPReadString(lString,bone_.name);

	getline(lStream,lString,'|');
	SPReadMatrix(lString,bone_.offsetMatirx);

	getline(lStream,lString,'|');
	SPReadNumber(lString,bone_.index);
}

#pragma endregion

#pragma region バイナリ

bool AliceFileStream::SLoadAlicePolygonBinaryData(const std::string& path_,AliceModelData* model_)
{
	sDirectoryPath = AliceUtility::Fille::GetDirectoryPath(path_);

	//ファイルを開く
	FILE* lFilePtr = NULL;
	fopen_s(&lFilePtr,path_.data(),"rb");

	if ( !lFilePtr )
	{
		return false;
	}

	if ( !SPReadHesderBinaryData(model_,lFilePtr) )
	{
		fclose(lFilePtr);
		return false;

	}

	if ( !SPReadNodesBinaryData(model_,lFilePtr) )
	{
		fclose(lFilePtr);
		return false;

	}

	if ( !SPReadMeshBinaryData(model_,lFilePtr) )
	{
		fclose(lFilePtr);
		return false;

	}

	fclose(lFilePtr);
	return true;

}

bool AliceFileStream::SPReadHesderBinaryData(AliceModelData* model_,FILE* fp_)
{
	//名前
	{
		size_t lNameNum;
		fread(&lNameNum,sizeof(size_t),1,fp_);
		if ( lNameNum < 256 )
		{
			fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
			model_->name = std::string(cBuffC.data(),lNameNum);
		}
		else
		{
			return false;
		}

	}

	//アニメーションできるか
	{
		fread(&model_->canAnimation,sizeof(bool),1,fp_);
	}

	//アニメーションするか
	{
		fread(&model_->IsAnime,sizeof(bool),1,fp_);
	}

	//rootNodeの逆行列
	{
		fread(&model_->globalInverseTransform,sizeof(float),16,fp_);
	}

	//ノードの数
	{
		size_t nodeNum;
		fread(&nodeNum,sizeof(size_t),1,fp_);
		model_->nodes.resize(nodeNum);
	}

	//メッシュの数
	{
		size_t meshNum;
		fread(&meshNum,sizeof(size_t),1,fp_);
		model_->meshes.reserve(meshNum);
	}

	return true;
}

bool AliceFileStream::SPReadNodesBinaryData(AliceModelData* model_,FILE* fp_)
{

	for ( size_t i = 0; i < model_->nodes.size(); i++ )
	{
		Node& node = model_->nodes[ i ];

		//ノードの名前
		{
			size_t lNameNum;
			fread(&lNameNum,sizeof(size_t),1,fp_);
			if ( lNameNum < 256 )
			{
				fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
				node.name = std::string(cBuffC.data(),lNameNum);
			}
			else
			{
				return false;
			}
		}

		//ローカル変形行列
		{
			fread(&node.transform,sizeof(float),16,fp_);
		}

		//グローバル変形行列
		{
			fread(&node.globalTransform,sizeof(float),16,fp_);
		}

		//親ノード
		{
			size_t lNameNum;
			fread(&lNameNum,sizeof(size_t),1,fp_);
			if ( lNameNum < 256 )
			{
				if ( lNameNum != 0 )
				{
					fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
				}

			}
			else
			{
				return false;
			}

			std::string lParentName(cBuffC.data(),lNameNum);

			if ( lParentName.size() == 0 )
			{
				node.parent = nullptr;
				continue;
			}

			//親ノード探索
			std::vector<Node>::iterator lnodeItr;
			lnodeItr = std::find_if(model_->nodes.begin(),model_->nodes.end(),[ & ] (Node& p)
				{
					return p.name == lParentName;
				});

			if ( lnodeItr != model_->nodes.end() )
			{
				node.parent = &*lnodeItr;
				continue;
			}
			else
			{
				node.parent = nullptr;
			}
		}


	}

	return true;
}

bool AliceFileStream::SPReadMeshBinaryData(AliceModelData* model_,FILE* fp_)
{
	for ( size_t i = 0; i < static_cast< size_t >( sMeshNum ); i++ )
	{
		std::unique_ptr<ModelMesh>lMesh = std::make_unique<ModelMesh>();

		//名前
		{
			size_t lNameNum;
			fread(&lNameNum,sizeof(size_t),1,fp_);
			if ( lNameNum < 256 )
			{
				fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
				lMesh->name = std::string(cBuffC.data(),lNameNum);
			}
			else
			{
				return false;
			}
		}

		//ノードの名前
		{
			size_t lNameNum;
			fread(&lNameNum,sizeof(size_t),1,fp_);
			if ( lNameNum < 256 )
			{
				fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
				lMesh->nodeName = std::string(cBuffC.data(),lNameNum);
			}
			else
			{
				return false;
			}

			std::vector<Node>::iterator lnodeItr;
			lnodeItr = std::find_if(model_->nodes.begin(),model_->nodes.end(),[ & ] (Node& p)
				{
					return p.name == lMesh->nodeName;
				});

			if ( lnodeItr != model_->nodes.end() )
			{
				lMesh->node = &*lnodeItr;
			}
			else
			{
				lMesh->node = nullptr;
			}
		}

		//頂点データ
		{
			size_t lVertexNum;
			fread(&lVertexNum,sizeof(size_t),1,fp_);
			lMesh->vertices.resize(lVertexNum);

			fread(lMesh->vertices.data(),sizeof(lMesh->vertices[ 0 ]),lVertexNum,fp_);
		}

		//インデックスデータ
		{
			size_t lIndexNum;
			fread(&lIndexNum,sizeof(size_t),1,fp_);
			lMesh->indices.resize(lIndexNum);

			fread(lMesh->indices.data(),sizeof(lMesh->indices[ 0 ]),lIndexNum,fp_);
		}

		//テクスチャファイルパス
		{
			size_t lTextureNum;
			fread(&lTextureNum,sizeof(size_t),1,fp_);
			if ( lTextureNum != 0 )
			{
				lMesh->textures.resize(lTextureNum);
				for ( size_t j = 0; j < lTextureNum; j++ )
				{
					size_t lNameNum;
					fread(&lNameNum,sizeof(size_t),1,fp_);
					if ( lNameNum < 256 )
					{
						fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
						std::string lFilepath = std::string(cBuffC.data(),lNameNum);
						lFilepath = sDirectoryPath + lFilepath;
						lMesh->textures[ j ] = TextureManager::SGetTextureData(TextureManager::SLoad(lFilepath));
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				//メッシュにテクスチャがなかった場合は白色のテクスチャを貼る
			}
		}

		//ノーマルテクスチャファイルパス
		{
			size_t lTextureNum = 1;
			fread(&lTextureNum,sizeof(size_t),1,fp_);
			if ( lTextureNum != 0 )
			{
				lMesh->texturesNormal.resize(lTextureNum);
				for ( size_t j = 0; j < lTextureNum; j++ )
				{
					size_t lNameNum;
					fread(&lNameNum,sizeof(size_t),1,fp_);
					if ( lNameNum < 256 )
					{
						fread(cBuffC.data(),sizeof(char),lNameNum,fp_);

						std::string lFilepath = std::string(cBuffC.data(),lNameNum);
						lFilepath = sDirectoryPath + lFilepath;
						lMesh->texturesNormal[ j ] = TextureManager::SGetTextureData(TextureManager::SLoad(lFilepath));
					}
					else
					{
						return false;
					}
				}
			}
		}

		//ボーン
		{
			size_t lBonesNum = lMesh->vecBones.size();
			fread(&lBonesNum,sizeof(size_t),1,fp_);

			lMesh->vecBones.resize(lBonesNum);

			for ( size_t j = 0; j < lBonesNum; j++ )
			{
				size_t lNameNum;
				fread(&lNameNum,sizeof(size_t),1,fp_);
				if ( lNameNum < 256 )
				{
					fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
					lMesh->vecBones[ j ].name = std::string(cBuffC.data(),lNameNum);
				}
				else
				{
					return false;
				}

				fread(&lMesh->vecBones[ j ].offsetMatirx,sizeof(float),16,fp_);

				fread(&lMesh->vecBones[ j ].index,sizeof(uint32_t),1,fp_);
			}
		}

		//マテリアル
		{
			size_t lNameNum = lMesh->material.name.size();
			fread(&lNameNum,sizeof(size_t),1,fp_);
			if ( lNameNum < 256 )
			{
				fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
				lMesh->material.name = std::string(cBuffC.data(),lNameNum);
			}
			else
			{
				return false;
			}

			fread(&lMesh->material.ambient,sizeof(float),3,fp_);

			fread(&lMesh->material.diffuse,sizeof(float),3,fp_);

			fread(&lMesh->material.specular,sizeof(float),3,fp_);

			fread(&lMesh->material.emission,sizeof(float),3,fp_);

			fread(&lMesh->material.shininess,sizeof(float),1,fp_);

			fread(&lMesh->material.alpha,sizeof(float),1,fp_);

			fread(&lNameNum,sizeof(size_t),1,fp_);
			if ( lNameNum < 256 )
			{
				fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
				lMesh->material.textureFiename = std::string(cBuffC.data(),lNameNum);
			}
			else
			{
				return false;
			}
		}

		model_->meshes.push_back(std::move(lMesh));
	}



	return true;
}

#pragma endregion

#pragma endregion

#pragma region アニメーションデータ

#pragma region ASCII

bool AliceFileStream::SLoadAliceMotionData(const std::string& path_,MotionData* motion_)
{
	sDirectoryPath = AliceUtility::Fille::GetDirectoryPath(path_);

	//モデルデータ
	std::stringstream lMotionData;
	//1行分も文字列を入れる変数
	std::string lLine;

	//ファイルを開く
	std::ifstream lFile(path_);
	if ( lFile.fail() )
	{
		return false;
	}

	//ファイルの内容を文字列をストリームにコピー
	lMotionData << lFile.rdbuf();

	//ファイルを閉じる
	lFile.close();

	//一行取得
	getline(lMotionData,lLine,'\n');

	if ( lLine == "<MOTION>" )
	{
		SPReadMotionData(lMotionData,lLine,motion_);
	}

	return true;
}

void AliceFileStream::SPReadMotionData(std::stringstream& data_,std::string& str_,MotionData* motionData_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}

		if ( str_ == lLine )
		{
			return;
		}

		if ( lLine == "<HEADER>" )
		{
			SPReadMotionHesderData(data_,lLine,motionData_);
		}

		if ( lLine == "<MotionNodes>" )
		{
			SPReadMotionNodesData(data_,lLine,motionData_);
		}

	}
}

void AliceFileStream::SPReadMotionHesderData(std::stringstream& data_,std::string& str_,MotionData* motion_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}
		std::istringstream lStream(lLine);

		if ( str_ == lStream.str() )
		{
			return;
		}

		getline(lStream,lLine,':');

		if ( std::equal(lLine.begin(),lLine.end(),"Name") )
		{
			getline(lStream,lLine,':');

			SPReadString(lLine,motion_->name);

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"TicksPerSecond") )
		{
			getline(lStream,lLine,'\n');
			SPReadNumber(lLine,motion_->ticksPerSecond);
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"Duration") )
		{
			getline(lStream,lLine,'\n');
			SPReadNumber(lLine,motion_->duration);
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"NodeNum") )
		{
			getline(lStream,lLine,':');

			float lNodeNun;
			SPReadNumber(lLine,lNodeNun);
			motion_->channels.resize(static_cast< size_t >( lNodeNun ));
			continue;
		}
	}
}

void AliceFileStream::SPReadMotionNodesData(std::stringstream& data_,std::string& str_,MotionData* motion_)
{
	//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		if ( lLine == "" )
		{
			continue;
		}

		std::istringstream lStream(lLine);

		for ( size_t i = 0; i < motion_->channels.size(); i++ )
		{
			SPReadMotionNodeData(data_,lLine,motion_->channels[ i ]);
		}

		if ( str_ == lLine )
		{
			return;
		}
	}
}

void AliceFileStream::SPReadMotionNodeData(std::stringstream& data_,std::string& str_,MotionNode& node_)
{
		//1行分の文字列を入れる変数
	std::string lLine;

	//一行取得
	while ( getline(data_,lLine,'\n') )
	{
		std::istringstream lStream(lLine);


		if ( lLine == "" )
		{
			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"") )
		{
			continue;
		}

		if ( str_ == lLine )
		{
			getline(data_,lLine,'\n');
			getline(data_,str_,'\n');
			return;
		}

		getline(lStream,lLine,':');

		if ( std::equal(lLine.begin(),lLine.end(),"Name") )
		{
			getline(lStream,lLine,':');

			SPReadString(lLine,node_.name);

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"ScalingKeyNum") )
		{
			getline(lStream,lLine,':');

			float lScalingKeyNum;
			SPReadNumber(lLine,lScalingKeyNum);
			node_.scalingKeys.resize(static_cast< size_t >( lScalingKeyNum ));

			for ( size_t i = 0; i < node_.scalingKeys.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(10,lLine.size() - 11);

				SPReadVectorKey(lLine,node_.scalingKeys[ i ]);
			}

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"RotationKeyNum") )
		{
			getline(lStream,lLine,':');

			float lRotationKeyNum;
			SPReadNumber(lLine,lRotationKeyNum);
			node_.rotationKeys.resize(static_cast< size_t >( lRotationKeyNum ));

			for ( size_t i = 0; i < node_.rotationKeys.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(12,lLine.size() - 13);

				SPReadQuaternionKey(lLine,node_.rotationKeys[ i ]);
			}

			continue;
		}

		if ( std::equal(lLine.begin(),lLine.end(),"PositionKeysNum") )
		{
			getline(lStream,lLine,':');

			float lPositionKeysNum;
			SPReadNumber(lLine,lPositionKeysNum);
			node_.positionKeys.resize(static_cast< size_t >( lPositionKeysNum ));

			for ( size_t i = 0; i < node_.positionKeys.size(); i++ )
			{
				getline(data_,lLine,'\n');
				lLine = lLine.substr(13,lLine.size() - 14);

				SPReadVectorKey(lLine,node_.positionKeys[ i ]);
			}

			continue;
		}
	}
}

#pragma endregion

#pragma region バイナリ

bool AliceFileStream::SLoadAliceMotionBinaryData(const std::string& path_,MotionData* motion_)
{
	sDirectoryPath = AliceUtility::Fille::GetDirectoryPath(path_);

	//ファイルを開く
	FILE* lFilePtr = nullptr;
	fopen_s(&lFilePtr,path_.data(),"rb");

	if ( !lFilePtr )
	{
		return false;
	}

	//名前
	{
		size_t lNameNum;
		fread(&lNameNum,sizeof(size_t),1,lFilePtr);
		if ( lNameNum < 256 )
		{
			fread(cBuffC.data(),sizeof(char),lNameNum,lFilePtr);
			motion_->name = std::string(cBuffC.data(),lNameNum);
		}
		else
		{
			return false;
		}

	}

	//一秒あたりのアニメーション継続時間
	{
		float lTicks;
		fread(&lTicks,sizeof(float),1,lFilePtr);
		motion_->ticksPerSecond = lTicks;
	}

	//アニメーション時間
	{
		float lDuration;
		fread(&lDuration,sizeof(float),1,lFilePtr);
		motion_->duration = lDuration;
	}

	//チャンネル数
	{
		size_t lChannelNum;
		fread(&lChannelNum,sizeof(size_t),1,lFilePtr);
		motion_->channels.resize(lChannelNum);
	}

	//チャンネル
	for ( size_t i = 0; i < motion_->channels.size(); i++ )
	{
		if ( !SPReadMotionNodeBinaryData(&motion_->channels[ i ],lFilePtr) )
		{
			fclose(lFilePtr);
			return false;
		}
	}

	return true;
}

bool AliceFileStream::SPReadMotionNodeBinaryData(MotionNode* node_,FILE* fp_)
{
	//名前
	{
		size_t lNameNum;
		fread(&lNameNum,sizeof(size_t),1,fp_);
		if ( lNameNum < 256 )
		{
			fread(cBuffC.data(),sizeof(char),lNameNum,fp_);
			node_->name = std::string(cBuffC.data(),lNameNum);
		}
		else
		{
			return false;
		}
	}

	//スケーリング
	{
		size_t lScalingNum;
		fread(&lScalingNum,sizeof(size_t),1,fp_);
		node_->scalingKeys.resize(lScalingNum);

		for ( size_t i = 0; i < lScalingNum; i++ )
		{
			fread(&node_->scalingKeys[ i ],sizeof(VectorKey),1,fp_);
		}

	}

	//回転角
	{
		size_t lRotationNum;
		fread(&lRotationNum,sizeof(size_t),1,fp_);
		node_->rotationKeys.resize(lRotationNum);

		for ( size_t i = 0; i < lRotationNum; i++ )
		{
			fread(&node_->rotationKeys[ i ],sizeof(QuaternionKey),1,fp_);
		}
	}

	//位置
	{
		size_t lPositionNum;
		fread(&lPositionNum,sizeof(size_t),1,fp_);
		node_->positionKeys.resize(lPositionNum);

		for ( size_t i = 0; i < lPositionNum; i++ )
		{
			fread(&node_->positionKeys[ i ],sizeof(VectorKey),1,fp_);
		}

	}

	return true;
}

#pragma endregion

#pragma endregion

#pragma region その他

float AliceFileStream::SPAtof(const std::string& str_)
{
	return static_cast< float >( atof(str_.c_str()) );
}
void AliceFileStream::SPReadFlag(const std::string& strFlag_,bool& flag_)
{
	flag_ = std::equal(strFlag_.begin(),strFlag_.end(),"true") ? true : false;
}
void AliceFileStream::SPReadNumber(const std::string& strNum_,float& num_)
{
	num_ = static_cast< float >( std::atof(strNum_.substr(1,strNum_.size() - 2).c_str()) );
}
void AliceFileStream::SPReadNumber(const std::string& strNum_,uint32_t& num_)
{
	num_ = static_cast< uint32_t >( std::atof(strNum_.substr(1,strNum_.size() - 2).c_str()) );
}
void AliceFileStream::SPReadVector2(const std::string& strVec_,AliceMathF::Vector2& vec_)
{
	sLineData.str(strVec_.substr(1,strVec_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	vec_.x = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.y = SPAtof(sBbuff);
}
void AliceFileStream::SPReadVector3(const std::string& strVec_,AliceMathF::Vector3& vec_)
{
	sLineData.str(strVec_.substr(1,strVec_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	vec_.x = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.y = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.z = SPAtof(sBbuff);
}
void AliceFileStream::SPReadVector3(const std::string& strVec_,AliceMathF::Vector4& vec_)
{
	sLineData.str(strVec_.substr(1,strVec_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	vec_.x = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.y = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.z = SPAtof(sBbuff);

	vec_.w = 1.0f;
}
void AliceFileStream::SPReadVector4(const std::string& strVec_,AliceMathF::Vector4& vec_)
{
	sLineData.str(strVec_.substr(1,strVec_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	vec_.x = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.y = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.z = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	vec_.w = SPAtof(sBbuff);
}
void AliceFileStream::SPReadQuaternion(const std::string& strVec_,AliceMathF::Quaternion& quaternion_)
{
	sLineData.str(strVec_.substr(1,strVec_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	quaternion_.x = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	quaternion_.y = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	quaternion_.z = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	quaternion_.w = SPAtof(sBbuff);
}
void AliceFileStream::SPReadMatrix(const std::string& strMat_,AliceMathF::Matrix4& mat_)
{
	sLineData.str(strMat_.substr(1,strMat_.size() - 2));
	sLineData.clear();

	for ( size_t i = 0; i < 4; i++ )
	{
		for ( size_t j = 0; j < 4; j++ )
		{
			getline(sLineData,sBbuff,',');
			mat_.m[ i ][ j ] = SPAtof(sBbuff);
		}
	}
}
void AliceFileStream::SPReadVectorKey(const std::string& strVec_,VectorKey& vectorKey_)
{
	std::istringstream lData(strVec_.substr(2,strVec_.size()));

	getline(lData,sBbuff,'|');
	SPReadVector3(sBbuff,vectorKey_.value);

	getline(lData,sBbuff,'\n');
	SPReadNumber(sBbuff,vectorKey_.time);
}
void AliceFileStream::SPReadQuaternionKey(const std::string& strVer,QuaternionKey& quaternionKey_)
{
	std::istringstream lData(strVer.substr(1,strVer.size() - 2));
	lData.clear();
	lData.clear();

	getline(lData,sBbuff,'|');
	SPReadQuaternion(sBbuff,quaternionKey_.value);

	getline(lData,sBbuff,'|');
	SPReadNumber(sBbuff,quaternionKey_.time);
}
void AliceFileStream::SPReadString(const std::string& strData_,std::string& str_)
{
	str_ = strData_.substr(1,strData_.size() - 2);

}
void AliceFileStream::SPReadBoneWeight(const std::string& strWeight_,std::array<float,MAX_BONE_INDICES>& boneWeight_)
{
	sLineData.str(strWeight_.substr(1,strWeight_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	boneWeight_[ 0 ] = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	boneWeight_[ 1 ] = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	boneWeight_[ 2 ] = SPAtof(sBbuff);

	getline(sLineData,sBbuff,',');
	boneWeight_[ 3 ] = SPAtof(sBbuff);
}
void AliceFileStream::SPReadBoneIndex(const std::string& strIndex_,std::array<uint32_t,MAX_BONE_INDICES>& boneIndex_)
{
	sLineData.str(strIndex_.substr(1,strIndex_.size() - 2));
	sLineData.clear();

	getline(sLineData,sBbuff,',');
	boneIndex_[ 0 ] = static_cast< uint32_t >( SPAtof(sBbuff) );

	getline(sLineData,sBbuff,',');
	boneIndex_[ 1 ] = static_cast< uint32_t >( SPAtof(sBbuff) );

	getline(sLineData,sBbuff,',');
	boneIndex_[ 2 ] = static_cast< uint32_t >( SPAtof(sBbuff) );

	getline(sLineData,sBbuff,',');
	boneIndex_[ 3 ] = static_cast< uint32_t >( SPAtof(sBbuff) );
}

#pragma endregion