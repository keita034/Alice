#include "ModelCollider.h"
#include <ModelMesh.h>

void ModelCollider::SetMeshs(std::vector<ModelMesh> modelMeshs)
{
	meshsNum = modelMeshs.size();
	meshs.resize(meshsNum);

	for (size_t i = 0; i < meshsNum; i++)
	{
		meshs[i].SetMeshs(modelMeshs[i].GetVertices(), modelMeshs[i].GetIndices());
	}
}

const std::vector<MeshCollider>& ModelCollider::GetMeshs()
{
	return meshs;
}

size_t ModelCollider::GetMeshNum()
{
	return meshsNum;
}

const MeshCollider& ModelCollider::GetMeshs(size_t index)
{
	if (index <= meshsNum)
	{
		return meshs[0];
	}

	return meshs[index];
}
