#include <BaseTransform.h>

AliceMathF::Matrix4 BaseTransform::sDefaultProjectionMat = { 1.3579f, 0.0f, 0.0f, 0.0f, 0.0f, 2.4142f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0001f, 1.0f, 0.0f, 0.0f, -0.1000f, 0.0f };
AliceMathF::Matrix4 BaseTransform::sDefaultViewMat = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 50.0f, 0.0f };


namespace AliceMathF
{
	Matrix4 MakeWorldMatrix4(BaseTransform& transform_, const Quaternion& quaternion_)
	{
		Matrix4 lMatWorld = MakeIdentity();

		Matrix4 lMatScal, lMatRot, lMatTrans;

		//スケーリング倍率
		lMatScal.MakeScaling(transform_.scale);

		//回転行列
		lMatRot = quaternion_.Rotate();

		// matWorld_に移動量を掛け算
		lMatTrans.MakeTranslation(transform_.translation);

		lMatWorld = lMatScal * lMatRot * lMatTrans;

		if (transform_.parent)
		{
			lMatWorld *= transform_.parent->matWorld;
		}

		return lMatWorld;
	}
	Matrix4 MakeWorldMatrix4(BaseTransform& transform_)
	{

		Matrix4 lMatWorld = MakeIdentity();

		Matrix4 lMatScal, lMatRot, lMatTrans;

		//スケーリング倍率
		lMatScal.MakeScaling(transform_.scale);

		//回転行列
		lMatRot.MakeRotation(transform_.rotation);

		// matWorld_に移動量を掛け算
		lMatTrans.MakeTranslation(transform_.translation);

		lMatWorld = lMatScal * lMatRot * lMatTrans;

		if (transform_.parent)
		{
			lMatWorld *= transform_.parent->matWorld;
		}

		return lMatWorld;
	}

	Matrix4 MakeWorldMatrix4(BaseTransform& transform_, const AliceMathF::Matrix4& rotMat_)
	{
		Matrix4 lMatWorld = MakeIdentity();

		Matrix4 lMatScal, lMatTrans;

		//スケーリング倍率
		lMatScal.MakeScaling(transform_.scale);

		// matWorld_に移動量を掛け算
		lMatTrans.MakeTranslation(transform_.translation);

		lMatWorld = lMatScal * rotMat_ * lMatTrans;

		if (transform_.parent)
		{
			lMatWorld *= transform_.parent->matWorld;
		}

		return lMatWorld;
	}

	Vector3 GetWorldPosition(BaseTransform& transform_)
	{
		//ワールド座標を入れる変数
		Vector3 lWorldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		lWorldPos.x = transform_.matWorld.m[3][0];
		lWorldPos.y = transform_.matWorld.m[3][1];
		lWorldPos.z = transform_.matWorld.m[3][2];

		return lWorldPos;
	}
}
