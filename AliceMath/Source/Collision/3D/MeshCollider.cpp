#include "MeshCollider.h"

void MeshCollider::SetMeshs(const std::vector<PosColor>& modelMeshs, const std::vector<uint32_t>& indexs)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(AliceMathF::TrnsVec4(modelMeshs[indexs[i]].pos, true));
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(AliceMathF::TrnsVec4(modelMeshs[indexs[i]].pos, true));
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(AliceMathF::TrnsVec4(modelMeshs[indexs[i]].pos, true));
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNorm>& modelMeshs, const std::vector<uint32_t>& indexs)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(modelMeshs[indexs[i]].position);
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(modelMeshs[indexs[i]].position);
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(modelMeshs[indexs[i]].position);
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNormalUv>& modelMeshs, const std::vector<uint32_t>& indexs)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(AliceMathF::TrnsVec4(modelMeshs[indexs[i]].pos, true));
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(AliceMathF::TrnsVec4(modelMeshs[indexs[i]].pos, true));
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(AliceMathF::TrnsVec4(modelMeshs[indexs[i]].pos, true));
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNormSkin>& modelMeshs, const std::vector<uint32_t>& indexs)
{
		//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(modelMeshs[indexs[i]].position);
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(modelMeshs[indexs[i]].position);
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(modelMeshs[indexs[i]].position);
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

void MeshCollider::SetMeshs(const std::vector<PosNormUvTangeColSkin>& modelMeshs, const std::vector<uint32_t>& indexs)
{
	//メッシュ配列のサイズを得る
	triangles.resize(modelMeshs.size() / 3);

	trianglesNum = 0;

	for (size_t i = 0; i < indexs.size(); i++)
	{
		//座標0のセット
		triangles[trianglesNum].SetP0(modelMeshs[indexs[i]].position);
		i++;

		//座標1のセット
		triangles[trianglesNum].SetP1(modelMeshs[indexs[i]].position);
		i++;

		//座標2のセット
		triangles[trianglesNum].SetP2(modelMeshs[indexs[i]].position);
		i++;

		//法線の計算
		triangles[trianglesNum].ComputeNormal();

		trianglesNum++;
	}
}

const std::vector<TriangleCollider>& MeshCollider::GetTriangles()
{
	return triangles;
}

size_t MeshCollider::GetTrianglesNum()
{
	return trianglesNum;
}

const TriangleCollider& MeshCollider::GetTriangles(size_t index)
{
	if (index<= trianglesNum)
	{
		return triangles[0];
	}

	return triangles[index];
}
