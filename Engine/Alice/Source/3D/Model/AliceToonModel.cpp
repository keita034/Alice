#include "AliceToonModel.h"

void AliceToonModel::Draw(const Transform& transform_, const AliceMotionData* animation_, float frame_, const Material* material_)
{
	if (modelData->isToon)
	{
		outLineMaterialData = MaterialManager::SGetMaterial("DefaultToonModelOutLine");

		if (animation_)
		{
			if (!material_)
			{
				modelMaterialData = MaterialManager::SGetMaterial("DefaultToonModelAnimation");
			}
			else
			{
				modelMaterialData = material_;
			}

			AnimationUpdate(animation_, frame_);

			PToonModelAnimationDraw(transform_);
		}
		else
		{
			if (!material_)
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
		AliceModel::Draw(transform_, animation_, frame_, material_);
	}
}

void AliceToonModel::ZeldaDraw(const Transform& transform_, const AliceMotionData* animation_, float frame_, const Material* material_)
{
	if (animation_)
	{
		if (!material_)
		{
			modelMaterialData = MaterialManager::SGetMaterial("DefaultZeldaToonModelAnimation");
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
	if (modelData)
	{
		uint32_t lRampHnadel = TextureManager::SLoad(rampFilePath_);
		modelData->rampTex = TextureManager::SGetTextureData(lRampHnadel);
	}
}

void AliceToonModel::PToonModelDraw(const Transform& transform_)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		if (modelData->meshes[i]->node)
		{
			modelData->postureMatBuff->Update(&modelData->meshes[i]->node->globalTransform);
		}

		// プリミティブ形状の設定コマンド
		sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		sCmdList->SetPipelineState(outLineMaterialData->pipelineState->GetPipelineState());
		sCmdList->SetGraphicsRootSignature(outLineMaterialData->rootSignature->GetRootSignature());

		sCmdList->SetGraphicsRootConstantBufferView(1, modelData->postureMatBuff->GetAddress());

		modelData->meshes[i]->OutLineDraw(sCmdList, transform_);

		// パイプラインステートとルートシグネチャの設定コマンド
		sCmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		sCmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		sCmdList->SetGraphicsRootConstantBufferView(3, modelData->postureMatBuff->GetAddress());
		modelData->meshes[i]->ToonDraw(sCmdList, transform_, modelData->rampTex->gpuHandle, sLight);
	}
}

void AliceToonModel::PToonModelAnimationDraw(const Transform& transform_)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		sCmdList->SetPipelineState(outLineMaterialData->pipelineState->GetPipelineState());
		sCmdList->SetGraphicsRootSignature(outLineMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[i]->AnimOutLineDraw(sCmdList, transform_);

		// パイプラインステートとルートシグネチャの設定コマンド
		sCmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		sCmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[i]->AnimToonDraw(sCmdList, transform_, modelData->rampTex->gpuHandle, sLight);
	}
}
