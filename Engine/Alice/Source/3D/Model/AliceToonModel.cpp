#include "AliceToonModel.h"

void AliceToonModel::Draw(const Transform& transform_,const AliceMotionData* animation_,const Material* material_)
{
	if ( modelData->isToon )
	{
		outLineMaterialData = MaterialManager::SGetMaterial("DefaultToonModelOutLine");

		if ( animation_ )
		{
			if ( !material_ )
			{
				modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModelAnimation");
			}
			else
			{
				modelMaterialData = material_;
			}

			AnimationUpdate(animation_);

			PToonModelAnimationDraw(transform_);
		}
		else
		{
			if ( !material_ )
			{
				modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModel");;
			}
			else
			{
				modelMaterialData = material_;
			}

			PToonModelDraw(transform_);
		}

		modelData->IsAnime = false;
	}
	else
	{
		AliceModel::Draw(transform_,animation_,material_);
	}
}

void AliceToonModel::Draw(const Transform& transform_,AliceBlendTree* blendTree_,const Material* material_)
{
	if ( modelData->isToon )
	{
		outLineMaterialData = MaterialManager::SGetMaterial("DefaultToonModelOutLine");

		if ( blendTree_ )
		{
			if ( !material_ )
			{
				modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModelAnimation");
			}
			else
			{
				modelMaterialData = material_;
			}

			AnimationUpdate(blendTree_);

			PToonModelAnimationDraw(transform_);
		}
		else
		{
			if ( !material_ )
			{
				modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModel");;
			}
			else
			{
				modelMaterialData = material_;
			}

			PToonModelDraw(transform_);
		}

		modelData->IsAnime = false;
	}
	else
	{
		AliceModel::Draw(transform_,blendTree_,material_);
	}
}

void AliceToonModel::Draw(const Transform& transform_)
{
	if ( modelData->isToon )
	{
		outLineMaterialData = MaterialManager::SGetMaterial("DefaultToonModelOutLine");

		modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModel");;

		PToonModelDraw(transform_);

		modelData->IsAnime = false;
	}
	else
	{
		AliceModel::Draw(transform_);
	}
}

void AliceToonModel::Draw(const Transform& transform_,const Material* material_)
{
	if ( modelData->isToon )
	{
		outLineMaterialData = MaterialManager::SGetMaterial("DefaultToonModelOutLine");

		if ( !material_ )
		{
			modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModel");;
		}
		else
		{
			modelMaterialData = material_;
		}

		PToonModelDraw(transform_);

		modelData->IsAnime = false;
	}
	else
	{
		AliceModel::Draw(transform_,material_);
	}
}

void AliceToonModel::ZeldaDraw(const Transform& transform_,const AliceMotionData* animation_,const Material* material_)
{
	if ( animation_ )
	{
		if ( !material_ )
		{
			modelMaterialData = MaterialManager::SGetMaterial("DefaultZeldaToonModelAnimation");
		}
		else
		{
			modelMaterialData = material_;
		}

		AnimationUpdate(animation_);

		PModelAnimationDraw(transform_);
	}
	else
	{
		if ( !material_ )
		{
			modelMaterialData = MaterialManager::SGetMaterial("DefaultZeldaToonModel");;
		}
		else
		{
			modelMaterialData = material_;
		}

		PModelDraw(transform_);
	}

	modelData->IsAnime = false;
}

void AliceToonModel::SetRampTexture(const std::string& rampFilePath_)
{
	if ( modelData )
	{
		uint32_t lRampHnadel = TextureManager::SLoad(rampFilePath_);
		modelData->rampTex = TextureManager::SGetTextureData(lRampHnadel);
	}
}

void AliceToonModel::PToonModelDraw(const Transform& transform_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	for ( size_t i = 0; i < modelData->meshes.size(); i++ )
	{
		if ( modelData->meshes[ i ]->node )
		{
			modelData->postureMatBuff->Update(&modelData->meshes[ i ]->node->globalTransform);
		}

		// プリミティブ形状の設定コマンド
		lCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		lCmdList->SetPipelineState(outLineMaterialData->pipelineState->GetPipelineState());
		lCmdList->SetGraphicsRootSignature(outLineMaterialData->rootSignature->GetRootSignature());

		lCmdList->SetGraphicsRootConstantBufferView(1,modelData->postureMatBuff->GetAddress());

		modelData->meshes[ i ]->OutLineDraw(lCmdList,transform_);

		// パイプラインステートとルートシグネチャの設定コマンド
		lCmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		lCmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		lCmdList->SetGraphicsRootConstantBufferView(3,modelData->postureMatBuff->GetAddress());
		modelData->meshes[ i ]->ToonDraw(lCmdList,transform_,modelData->rampTex->gpuHandle,sLight);
	}
}

void AliceToonModel::PToonModelAnimationDraw(const Transform& transform_)
{
	ID3D12GraphicsCommandList* lCmdList = sCmdList->GetGraphicCommandList();

	for ( size_t i = 0; i < modelData->meshes.size(); i++ )
	{
		// プリミティブ形状の設定コマンド
		lCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		lCmdList->SetPipelineState(outLineMaterialData->pipelineState->GetPipelineState());
		lCmdList->SetGraphicsRootSignature(outLineMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[ i ]->AnimOutLineDraw(lCmdList,transform_);

		// パイプラインステートとルートシグネチャの設定コマンド
		lCmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		lCmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[ i ]->AnimToonDraw(lCmdList,transform_,modelData->rampTex->gpuHandle,sLight);
	}
}
