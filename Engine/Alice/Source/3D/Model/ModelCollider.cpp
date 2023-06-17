#include<ModelCollider.h>
#include <ModelMesh.h>

void ModelCollider::SetMeshs(std::vector<ModelMesh> modelMeshs_)
{
	meshsNum = modelMeshs_.size();
	meshs.resize(meshsNum);

	for (size_t i = 0; i < meshsNum; i++)
	{
		meshs[i].SetMeshs(modelMeshs_[i].GetVertices(), modelMeshs_[i].GetIndices());
	}
}

const std::vector<MeshCollider>& ModelCollider::GetMeshs()const
{
	return meshs;
}

size_t ModelCollider::GetMeshNum()
{
	return meshsNum;
}

const MeshCollider& ModelCollider::GetMeshs(size_t index_)const
{
	if (index_ <= meshsNum)
	{
		return meshs[0];
	}

	return meshs[index_];
}
