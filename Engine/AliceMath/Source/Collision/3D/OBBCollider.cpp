#include "OBBCollider.h"

void OBBCollider::CreateOBB(const std::vector<PosNormalUv>& vertexs_, const  AliceMathF::Vector3& translation_, const AliceMathF::Vector3& rotation_, const AliceMathF::Vector3& scale_)
{
	//最大値、最小値の初期値設定
	AliceMathF::Vector3 lMax = AliceMathF::Vector3(-10000.0f, -10000.0f, -10000.0f);
	AliceMathF::Vector3 lMin = AliceMathF::Vector3(10000.0f, 10000.0f, 10000.0f);

	//最大値、最小値取得ループ
	for (const PosNormalUv& vertex : vertexs_)
	{
		const AliceMathF::Vector3& lPos = vertex.pos;

		if (lPos.x < lMin.x)
		{
			lMin.x = lPos.x;
		}
		if (lPos.x > lMax.x)
		{
			lMax.x = lPos.x;
		}
		
		if (lPos.y < lMin.y)
		{
			lMin.y = lPos.y;
		}
		if (lPos.y > lMax.y)
		{
			lMax.y = lPos.y;
		}

		if (lPos.z < lMin.z)
		{
			lMin.z = lPos.z;
		}
		if (lPos.z > lMax.z)
		{
			lMax.z = lPos.z;
		}
	}

	//中心点取得
	center = translation_;

	AliceMathF::Matrix4	lMatRot;

	//方向ベクトル取得
	lMatRot.MakeRotation(rotation_);
	normaDirect[0] = { lMatRot.m[0][0], lMatRot.m[0][1], lMatRot.m[0][2] };
	normaDirect[1] = { lMatRot.m[1][0], lMatRot.m[1][1], lMatRot.m[1][2] };
	normaDirect[2] = { lMatRot.m[2][0], lMatRot.m[2][1], lMatRot.m[2][2] };

	//長さ取得
	length[0] = AliceMathF::Abs(lMax.x - lMin.x) * 0.5f;
	length[1] = AliceMathF::Abs(lMax.y - lMin.y) * 0.5f;
	length[2] = AliceMathF::Abs(lMax.z - lMin.z) * 0.5f;

	length[0] *= scale_.x;
	length[1] *= scale_.y;
	length[2] *= scale_.z;

}

void OBBCollider::UpdateOBB(const AliceMathF::Matrix4& worldMatrix_, const AliceMathF::Vector3& rotation_)
{
	AliceMathF::Matrix4	lMatRot;

	//中心点取得
	center = AliceMathF::GetWorldPosition(worldMatrix_);

	//方向ベクトル取得
	lMatRot.MakeRotation(rotation_);
	normaDirect[0] = { lMatRot.m[0][0], lMatRot.m[0][1], lMatRot.m[0][2] };
	normaDirect[1] = { lMatRot.m[1][0], lMatRot.m[1][1], lMatRot.m[1][2] };
	normaDirect[2] = { lMatRot.m[2][0], lMatRot.m[2][1], lMatRot.m[2][2] };
}

const AliceMathF::Vector3& OBBCollider::GetDirect(uint16_t index_)const
{
	return normaDirect[index_];
}

float OBBCollider::GetLen(uint16_t index_)const
{
	return length[index_];
}

const AliceMathF::Vector3& OBBCollider::GetCenter()const
{
	return center;
}