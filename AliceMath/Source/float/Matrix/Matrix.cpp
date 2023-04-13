#include "Matrix.h"

namespace AliceMathF
{
	Vector3 GetWorldPosition(const AliceMathF::Matrix4& worldMat)
	{
		//ワールド座標を入れる変数
		Vector3 worldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = worldMat.m[3][0];
		worldPos.y = worldMat.m[3][1];
		worldPos.z = worldMat.m[3][2];

		return worldPos;
	}

}