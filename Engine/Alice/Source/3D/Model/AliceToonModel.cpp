#include "AliceToonModel.h"

void AliceToonModel::Draw(Transform& transform, const AliceMotionData* animation, float frame, Material* material)
{
	if (modelData->isToon)
	{
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
		modelData->meshes[i].ToonDraw(cmdList, transform, modelData->rampTex->gpuHandle, light);
	}
}

void AliceToonModel::ToonModelAnimationDraw(Transform& transform)
{
	for (size_t i = 0; i < modelData->meshes.size(); i++)
	{
		// プリミティブ形状の設定コマンド
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// パイプラインステートとルートシグネチャの設定コマンド
		cmdList->SetPipelineState(modelMaterialData->pipelineState->GetPipelineState());
		cmdList->SetGraphicsRootSignature(modelMaterialData->rootSignature->GetRootSignature());

		modelData->meshes[i].AnimToonDraw(cmdList, transform, modelData->rampTex->gpuHandle, light);
	}
}
