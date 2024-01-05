#include"MaterialManager.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

#include<DefaultTexture.h>
#include<DefaultLambert.h>
#include<DefaultPhong.h>
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
#include<Fbx.h>

#include<Sprite.h>
#include<GPUParticleh.h>
#include<AliceAssert.h>

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
			else if ( name_ == "MashLineInvsrc" )
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
			else if ( name_ == "MultiGPUTest" )
			{
				CreateTestMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "FireGPUParticleDraw" )
			{
				CreateFireDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ShockWaveGPUParticleDraw" )
			{
				CreateShockWaveDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "LaserGPUParticleDraw" )
			{
				CreateLaserDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "AnimationMeshGPUParticleDraw" )
			{
				CreateAnimationMeshDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "MeshGPUParticleDraw" )
			{
				CreateMeshDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "AnimationModelGPUParticleDraw" )
			{
				CreateAnimationModelDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ModelGPUParticleDraw" )
			{
				CreateModelDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "BloodGushGPUParticleDraw" )
			{
				CreateBloodGushDrawMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			return materials[ name_ ].get();
		}

		AliceAssert(false,"その名前のマテリアルは存在しません\n");

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

			else if ( name_ == "ComputeFireGPUParticleEmit" )
			{
				CreateFireEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeFireGPUParticleFreeListInit" )
			{
				CreateFireFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeFireGPUParticleUpdate" )
			{
				CreateFireUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeFireGPUParticleDrawArgumentUpdate" )
			{
				CreateFireDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeShockWaveGPUParticleFreeListInit" )
			{
				CreateShockWaveFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeShockWaveGPUParticleUpdate" )
			{
				CreateShockWaveUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeShockWaveGPUParticleEmit" )
			{
				CreateShockWaveEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeShockWaveGPUParticleDrawArgumentUpdate" )
			{
				CreateShockWaveDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeLaserGPUParticleFreeListInit" )
			{
				CreateLaserFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeLaserGPUParticleEmit" )
			{
				CreateLaserEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeLaserGPUParticleUpdate" )
			{
				CreateLaserUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeLaserGPUParticleDrawArgumentUpdate" )
			{
				CreateLaserDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeAnimationMeshGPUParticleFreeListInit" )
			{
				CreateAnimationMeshFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeAnimationMeshGPUParticleEmit" )
			{
				CreateAnimationMeshEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeAnimationMeshGPUParticleUpdate" )
			{
				CreateAnimationMeshUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeAnimationMeshGPUParticleDrawArgumentUpdate" )
			{
				CreateAnimationMeshDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeMeshGPUParticleFreeListInit" )
			{
				CreateMeshFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeMeshGPUParticleEmit" )
			{
				CreateMeshEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeMeshGPUParticleUpdate" )
			{
				CreateMeshUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeMeshGPUParticleDrawArgumentUpdate" )
			{
				CreateMeshDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeAnimationModelGPUParticleFreeListInit" )
			{
				CreateAnimationModelFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeAnimationModelGPUParticleEmit" )
			{
				CreateAnimationModelEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeAnimationModelGPUParticleUpdate" )
			{
				CreateAnimationModelUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeAnimationModelGPUParticleDrawArgumentUpdate" )
			{
				CreateAnimationModelDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if( name_ == "ComputeAnimationModelGPUParticleDrawListRelease" )
			{
				CreateAnimationModelDrawListReleaseComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeModelGPUParticleFreeListInit" )
			{
				CreateModelFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeModelGPUParticleEmit" )
			{
				CreateModelEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeModelGPUParticleUpdate" )
			{
				CreateModelUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeModelGPUParticleDrawArgumentUpdate" )
			{
				CreateModelDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeModelGPUParticleDrawListRelease" )
			{
				CreateModelDrawListReleaseComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}

			else if ( name_ == "ComputeBloodGushGPUParticleFreeListInit" )
			{
				CreateBloodGushFreeListInitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeBloodGushGPUParticleEmit" )
			{
				CreateBloodGushEmitComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeBloodGushGPUParticleUpdate" )
			{
				CreateBloodGushUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
			}
			else if ( name_ == "ComputeBloodGushGPUParticleDrawArgumentUpdate" )
			{
				CreateBloodGushDrawArgumentUpdateComputeMaterial(this,sMultiAdapters->GetAdapter(index_));
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

	for ( std::pair<const std::string,std::unique_ptr<Material>>& material : materials )
	{
		material.second.reset();
	}

	for ( std::pair<const std::string,std::unique_ptr<ComputeMaterial>>& material : computeMaterials )
	{
		material.second.reset();
	}

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