#include "Matrix.h"

namespace AliceMathF
{
	Vector3 GetWorldPosition(const AliceMathF::Matrix4& worldMat_)
	{
		//ワールド座標を入れる変数
		Vector3 lWorldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		lWorldPos.x = worldMat_.m[3][0];
		lWorldPos.y = worldMat_.m[3][1];
		lWorldPos.z = worldMat_.m[3][2];

		return lWorldPos;
	}

}