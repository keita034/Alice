#include "AliceToonModel.h"

void AliceToonModel::Draw(Transform& transform, const AliceMotionData* animation, float frame, Material* material)
{
	if (modelData->isToon)
	{
		outLineMaterialData = MaterialManager::GetMaterial("DefaultToonModelOutLine");
		if (animation)
		{
			if (!material)
			{
				modelMaterialData = MaterialManager::GetMaterial("DefaultToonModelAnimation");
			}
			else
			{
				modelMaterialData = material;
			}

			AnimationUpdate(animation, frame);

			ToonModelAnimationDraw(transform);
		}
		else
		{
			if (!material)
			{
				modelMaterialData = MaterialManager::GetMaterial("DefaultToonModel");;
			}
			else
			{
				modelMaterialData = material;
			}

			ToonModelDraw(transform);
		}

		modelData->IsAnime = false;
	}
	else
	{
		AliceModel::Draw(transform, animation, frame, material);
	}
}

void AliceToonModel::ZeldaDraw(Transform& transform, const AliceMotionData* animation, float frame, Material* material)
{
	if (animation)
	{
		if (!material)
		{
			modelMaterialData = MaterialManager::GetMaterial("DefaultZeldaToonModelAnimation");
		}
		else
		{
			modelMaterialData = material;
		}

		AnimationUpdate(animation, frame);

		ModelAnimationDraw(transform);
	}
	else
	{
		if (!material)
		{
			modelMaterialData = MaterialManager::GetMaterial("DefaultZeldaToonModel");;
		}
		else
		{
			modelMaterialData = material;
		}

		ModelDraw(transform);
	}

	modelData->IsAnime = false;
}

void AliceToonModel::SetRampTexture(const std::string& rampFilePath)
{
	if (modelData)
	{
		uint32_t rampHnadel = TextureManager::Load(rampFilePath);
		modelData->rampTex = TextureManager::GetTextureData(rampHnadel);
	}
}

void AliceToonModel::ToonModelDraw(Transform& transform)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		if (modelData->meshes[i]->node)
		{
			modelData->postureMatBuff->Update(&modelData->meshes[i]->node->globalTransform);
		}

		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(outLineMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(outLineMaterialData->rootSignature->GetRootSignature());

		cmdList->SetGraphicsRootConstantBufferView(1, modelData->postureMatBuff->GetAddress());

		modelData->meshes[i]->OutLineDraw(cmdList, transform);

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		cmdList->SetGraphicsRootConstantBufferView(3, modelData->postureMatBuff->GetAddress());
		modelData->meshes[i]->ToonDraw(cmdList, transform, modelData->rampTex->gpuHandle, light);
	}
}

void AliceToonModel::ToonModelAnimationDraw(Transform& transform)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(outLineMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(outLineMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[i]->AnimOutLineDraw(cmdList, transform);

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[i]->AnimToonDraw(cmdList, transform, modelData->rampTex->gpuHandle, light);
	}
}
