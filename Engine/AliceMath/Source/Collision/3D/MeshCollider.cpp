#include "MeshCollider.h"

void MeshCollider::SetMeshs(const std::vector<PosColor>& modelMeshs_, const std::vector<uint32_t>& indexs_)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs_.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs_.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(AliceMathF::TrnsVec4(modelMeshs_[indexs_[i]].pos, true));
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(AliceMathF::TrnsVec4(modelMeshs_[indexs_[i]].pos, true));
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(AliceMathF::TrnsVec4(modelMeshs_[indexs_[i]].pos, true));
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNorm>& modelMeshs_, const std::vector<uint32_t>& indexs_)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs_.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs_.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(modelMeshs_[indexs_[i]].position);
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(modelMeshs_[indexs_[i]].position);
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(modelMeshs_[indexs_[i]].position);
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNormalUv>& modelMeshs_, const std::vector<uint32_t>& indexs_)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs_.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs_.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(AliceMathF::TrnsVec4(modelMeshs_[indexs_[i]].pos, true));
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(AliceMathF::TrnsVec4(modelMeshs_[indexs_[i]].pos, true));
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(AliceMathF::TrnsVec4(modelMeshs_[indexs_[i]].pos, true));
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNormSkin>& modelMeshs_, const std::vector<uint32_t>& indexs_)
{
		//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs_.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs_.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(modelMeshs_[indexs_[i]].position);
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(modelMeshs_[indexs_[i]].position);
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(modelMeshs_[indexs_[i]].position);
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNormUvTangeColSkin>& modelMeshs_, const std::vector<uint32_t>& indexs_)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs_.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs_.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(modelMeshs_[indexs_[i]].position);
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(modelMeshs_[indexs_[i]].position);
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(modelMeshs_[indexs_[i]].position);
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

const std::vector<TriangleCollider>& MeshCollider::GetTriangles()const
{
	return triangles;
}

size_t MeshCollider::GetTrianglesNum()
{
	return trianglesNum;
}

const TriangleCollider& MeshCollider::GetTriangles(size_t index_)const
{
	if (index_ <= trianglesNum)
	{
		return triangles[0];
	}

	return triangles[index_];
}
