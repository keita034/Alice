#include"MaterialManager.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

#include<DefaultTexture.h>
#include<DefaultLambert.h>
#include<DefaultPhong.h>
#include<Sprite2DNoblend.h>
#include<Sprite2DAlpha.h>
#include<Sprite2DAdd.h>
#include<Sprite2DSub.h>
#include<Sprite2DMula.h>
#include<Sprite2DInvsrc.h>
#include<Sprite3DNoblend.h>
#include<Sprite3DAlpha.h>
#include<Sprite3DAdd.h>
#include<Sprite3DSub.h>
#include<Sprite3DMula.h>
#include<Sprite3DInvsrc.h>
#include<DefaultParticle.h>
#include<RainParticle.h>
#include<IcosahedronParticle.h>
#include<TriangleNoblend.h>
#include<TriangleAlpha.h>
#include<TriangleAdd.h>
#include<TriangleSub.h>
#include<TriangleMula.h>
#include<TriangleInvsrc.h>
#include<LineNoblend.h>
#include<LineAlpha.h>
#include<LineAdd.h>
#include<LineSub.h>
#include<LineMula.h>
#include<LineInvsrc.h>
#include<DefaultPostEffect.h>
#include<ToonModel.h>
#include<Basic.h>
#include<Test.h>
#include<Fbx.h>

std::unique_ptr<MaterialManager> MaterialManager::materialManager;

void MaterialManager::Initialize(IMultiAdapters* multiAdapters_)
{
	sMultiAdapters = multiAdapters_;
	CreateFbxMaterial(this,sMultiAdapters->GetMainAdapter());
	CreateFbxAnimationMaterial(this,sMultiAdapters->GetMainAdapter());
}

MaterialManager* MaterialManager::SGetInstance()
{
	if ( !materialManager )
	{
		MaterialManager* lInstance = new MaterialManager();
		materialManager.reset(lInstance);
	}

	return materialManager.get();
}

Material* MaterialManager::GetMaterialData(const std::string& name_,AdaptersIndex index_)
{
	if ( name_.find("Compute") != std::string::npos )
	{
		return nullptr;
	}
	else
	{
		//一回読み込んだことがあるファイルはそのまま返す
		auto itr = find_if(materials.begin(),materials.end(),[ & ] (std::pair<const std::string,std::unique_ptr<Material,std::default_delete<Material>>>& p)
			{
					return p.second->name == name_;
			});

		if ( itr != materials.end() )
		{
			return itr._Ptr->_Myval.second.get();
		}

		if ( itr == materials.end() )
		{
			if ( name_ == "DefaultTexture" )
			{
				CreateDefaultTextureMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultLambert" )
			{
				CreateDefaultLambertMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultPhong" )
			{
				CreateDefaultPhongMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite2DNoblend" )
			{
				CreateSprite2DNoblendMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite2DAlpha" )
			{
				CreateSprite2DAlphaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite2DAdd" )
			{
				CreateSprite2DAddMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite2DSub" )
			{
				CreateSprite2DSubMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite2DMula" )
			{
				CreateSprite2DMulaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite2DInvsrc" )
			{
				CreateSprite2DInvsrcMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite3DNoblend" )
			{
				CreateSprite3DNoblendMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite3DAlpha" )
			{
				CreateSprite3DAlphaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite3DAdd" )
			{
				CreateSprite3DAddMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite3DSub" )
			{
				CreateSprite3DSubMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite3DMula" )
			{
				CreateSprite3DMulaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "Sprite3DInvsrc" )
			{
				CreateSprite3DInvsrcMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultParticle" )
			{
				CreateDefaultParticleMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultRainParticle" )
			{
				CreateRainParticleMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "IcosahedronParticle" )
			{
				CreateIcosahedronParticleMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashTriangleNoblend" )
			{
				CreateTriangleNoblendMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashTriangleAlpha" )
			{
				CreateTriangleAlphaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashTriangleAdd" )
			{
				CreateTriangleAddMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashTriangleSub" )
			{
				CreateTriangleSubMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashTriangleMula" )
			{
				CreateTriangleMulaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashTriangleInvsrc" )
			{
				CreateTriangleInvsrcMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashLineNoblend" )
			{
				CreateLineNoblendMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashLineAlpha" )
			{
				CreateLineAlphaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashLineAdd" )
			{
				CreateLineAddMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashLineSub" )
			{
				CreateLineSubMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashLineSub" )
			{
				CreateLineSubMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MashLineMula" )
			{
				CreateLineMulaMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if (name_ == "MashLineInvsrc" )
			{
				CreateLineInvsrcMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultPostEffect" )
			{
				CreateDefaultPostEffectMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultToonModel" )
			{
				CreateToonModelMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultToonModelAnimation" )
			{
				CreateToonModelAnimationMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultToonModelOutLine" )
			{
				CreateToonModelOutLineMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultToonModelAnimationOutLine" )
			{
				CreateToonModelOutLineAnimationMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultZeldaToonModel" )
			{
				CreateZeldaToonModelMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "DefaultZeldaToonModelAnimation" )
			{
				CreateZeldaToonModelAnimationMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "BasicGPUParticleDraw" )
			{
				CreateBasicDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if (name_ == "MultiGPUTest")
			{
				CreateTestMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			return materials[ name_ ].get();
		}

		return nullptr;
	}
}

ComputeMaterial* MaterialManager::GetComputeMaterialData(const std::string& name_,AdaptersIndex index_)
{
	if ( name_.find("Compute") == std::string::npos )
	{
		return nullptr;
	}
	else
	{
		//一回読み込んだことがあるファイルはそのまま返す
		auto itr = find_if(computeMaterials.begin(),computeMaterials.end(),[ & ] (std::pair<const std::string,std::unique_ptr<ComputeMaterial,std::default_delete<ComputeMaterial>>>& p)
			{
					return p.second->name == name_;
			});

		if ( itr != computeMaterials.end() )
		{
			return itr._Ptr->_Myval.second.get();
		}

		if ( itr == computeMaterials.end() )
		{
			if ( name_ == "ComputeBasicGPUParticleEmit" )
			{
				CreateBasicEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeBasicGPUParticleUpdate" )
			{
				CreateBasicUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeBasicGPUParticleDrawArgumentUpdate" )
			{
				CreateBasicDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeBasicGPUParticleFreeListInit" )
			{
				CreateBasicFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeMultiGPUTest" )
			{
				CreateTestComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			return computeMaterials[ name_ ].get();
		}

		return nullptr;
	}
}

void MaterialManager::AddMaterial(std::unique_ptr <Material>& material_,const std::string& name_)
{
	material_->name = name_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMaterialItr = find_if(materials.begin(),materials.end(),[ & ] (std::pair<const std::string,std::unique_ptr<Material,std::default_delete<Material>>>& material)
		{
			return material.second->name == name_;
		});

	if ( lMaterialItr == materials.end() )
	{
		materials[ name_ ] = std::move(material_);
	}
}

void MaterialManager::AddMaterial(std::unique_ptr <ComputeMaterial>& material_,const std::string& name_)
{
	material_->name = name_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMaterialItr = find_if(computeMaterials.begin(),computeMaterials.end(),[ & ] (std::pair<const std::string,std::unique_ptr<ComputeMaterial,std::default_delete<ComputeMaterial>>>& material)
		{
			return material.second->name == name_;
		});

	if ( lMaterialItr == computeMaterials.end() )
	{
		computeMaterials[ name_ ] = std::move(material_);
	}
}

void MaterialManager::AddMaterial(Material* material,const std::string& name_)
{
	material->name = name_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMaterialItr = find_if(materials.begin(),materials.end(),[ & ] (std::pair<const std::string,std::unique_ptr<Material,std::default_delete<Material>>>& material)
		{
			return material.second->name == name_;
		});

	if ( lMaterialItr == materials.end() )
	{
		std::unique_ptr<Material> lMaterial;
		lMaterial.reset(material);
		materials[ name_ ] = std::move(lMaterial);
	}
}

void MaterialManager::Finalize()
{
	sMultiAdapters = nullptr;
	materials.clear();
	computeMaterials.clear();
	materialManager.release();
}

Material* MaterialManager::SGetMaterial(const std::string& name_,AdaptersIndex index_)
{
	return MaterialManager::SGetInstance()->GetMaterialData(name_,index_);
}

ComputeMaterial* MaterialManager::SGetComputeMaterial(const std::string& name_,AdaptersIndex index_)
{
	return MaterialManager::SGetInstance()->GetComputeMaterialData(name_,index_);
}

MaterialManager::~MaterialManager()
{
}