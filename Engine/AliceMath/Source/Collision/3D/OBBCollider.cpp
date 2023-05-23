#include "OBBCollider.h"

void OBBCollider::CreateOBB(const std::vector<PosNormalUv>& vertex,const  AliceMathF::Vector3& trans,const AliceMathF::Vector3& rot,const AliceMathF::Vector3& scal)
{
	AliceMathF::Matrix4	matRot;

	//最大値、最小値の初期値設定
	AliceMathF::Vector3 max = AliceMathF::Vector3(-10000.0f, -10000.0f, -10000.0f);
	AliceMathF::Vector3 min = AliceMathF::Vector3(10000.0f, 10000.0f, 10000.0f);

	//最大値、最小値取得ループ
	for (size_t i = 0; i < vertex.size(); i++)
	{
		AliceMathF::Vector3 pos = vertex[i].pos;
		if (pos.x < min.x)min.x = pos.x;
		if (pos.x > max.x)max.x = pos.x;
		if (pos.y < min.y)min.y = pos.y;
		if (pos.y > max.y)max.y = pos.y;
		if (pos.z < min.z)min.z = pos.z;
		if (pos.z > max.z)max.z = pos.z;
	}

	//中心点取得
	center = trans;

	//方向ベクトル取得
	matRot.MakeRotation(rot);
	normaDirect[0] = { matRot.m[0][0], matRot.m[0][1], matRot.m[0][2] };
	normaDirect[1] = { matRot.m[1][0], matRot.m[1][1], matRot.m[1][2] };
	normaDirect[2] = { matRot.m[2][0], matRot.m[2][1], matRot.m[2][2] };

	//長さ取得
	Length[0] = AliceMathF::Abs(max.x - min.x) * 0.5f;
	Length[1] = AliceMathF::Abs(max.y - min.y) * 0.5f;
	Length[2] = AliceMathF::Abs(max.z - min.z) * 0.5f;

	Length[0] *= scal.x;
	Length[1] *= scal.y;
	Length[2] *= scal.z;
}

void OBBCollider::UpdateOBB(const AliceMathF::Matrix4& worldMat, const AliceMathF::Vector3& rot)
{
	AliceMathF::Matrix4	matRot;

	//中心点取得
	center = AliceMathF::GetWorldPosition(worldMat);

	//方向ベクトル取得
	matRot.MakeRotation(rot);
	normaDirect[0] = { matRot.m[0][0], matRot.m[0][1], matRot.m[0][2] };
	normaDirect[1] = { matRot.m[1][0], matRot.m[1][1], matRot.m[1][2] };
	normaDirect[2] = { matRot.m[2][0], matRot.m[2][1], matRot.m[2][2] };
}

const AliceMathF::Vector3& OBBCollider::GetDirect(uint16_t elem)
{
	return normaDirect[elem];
}

float OBBCollider::GetLen(uint16_t elem)
{
	return Length[elem];
}

const AliceMathF::Vector3& OBBCollider::GetCenter()
{
	return center;
}