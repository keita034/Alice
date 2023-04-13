#include "BaseTransform.h"

AliceMathF::Matrix4 BaseTransform::defaultProjectionMat = { 1.3579f, 0.0f, 0.0f, 0.0f, 0.0f, 2.4142f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0001f, 1.0f, 0.0f, 0.0f, -0.1000f, 0.0f };
AliceMathF::Matrix4 BaseTransform::defaultViewMat = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 50.0f, 0.0f };


namespace AliceMathF
{
	Matrix4 MakeWorldMatrix4(BaseTransform& transform)
	{

		Matrix4 matWorld = MakeIdentity();

		Matrix4 matScal, matRot, matTrans;

		//スケーリング倍率
		matScal.MakeScaling(transform.scale);

		//回転行列
		matRot.MakeRotation(transform.rotation);

		// matWorld_に移動量を掛け算
		matTrans.MakeTranslation(transform.translation);

		matWorld = matScal * matRot * matTrans;

		if (transform.parent)
		{
			matWorld *= transform.parent->matWorld;
		}

		return matWorld;
	}

	Vector3 GetWorldPosition(BaseTransform& transform)
	{
		//ワールド座標を入れる変数
		Vector3 worldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = transform.matWorld.m[3][0];
		worldPos.y = transform.matWorld.m[3][1];
		worldPos.z = transform.matWorld.m[3][2];

		return worldPos;
	}
}
