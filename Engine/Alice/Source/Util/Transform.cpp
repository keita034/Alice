#include<Transform.h>

Transform::~Transform()
{
}

void Transform::Initialize()
{
	//定数バッファ生成(3D座標変換行列)
	constBuff = CreateUniqueConstantBuffer(sizeof(worldViewpojCamera));

	AliceMathF::Matrix4 lMatScale, lMatRot, lMatTrans;

	//スケール、回転、平行移動行列の計算
	lMatScale.MakeScaling(scale);
	lMatRot.MakeRotation(rotation);
	lMatTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= lMatScale;
	//ワールド行列に回転を反映
	matWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	matWorld *= lMatTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	//定数バッファに書き込み
	constBuffMap.matWorld = matWorld * sDefaultViewMat * sDefaultProjectionMat;
	constBuffMap.world = matWorld;
	constBuffMap.cameraPos = { 0.0f,0.0f,-50.0f };

	Update();
}

void Transform::TransUpdate(Camera* camera_,AliceMathF::Matrix4* mat_)
{
	AliceMathF::Matrix4 lMatScale, lMatRot, lMatTrans;

	//スケール、回転、平行移動行列の計算
	lMatScale.MakeScaling(scale);
	lMatRot.MakeRotation(rotation);
	lMatTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= lMatScale;
	//ワールド行列に回転を反映
	matWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	matWorld *= lMatTrans;
	//親行列の指定がある場合は、掛け算する
	if ( mat_ )
	{
		matWorld *= *mat_;
	}
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	rigidBodyMatWorld = AliceMathF::MakeIdentity();
	//ワールド行列に回転を反映
	rigidBodyMatWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	rigidBodyMatWorld *= lMatTrans;
	//親行列の指定がある場合は、掛け算する
	if ( mat_ )
	{
		rigidBodyMatWorld *= *mat_;
	}
	if ( parent )
	{
		rigidBodyMatWorld *= parent->rigidBodyMatWorld;
	}

	//定数バッファに書き込み
	constBuffMap.matWorld = matWorld * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
  	constBuffMap.world = matWorld;
	constBuffMap.cameraPos = camera_->GetEye();
	Update();
}

void Transform::Update()
{
	constBuff->Update(&constBuffMap);
}

void Transform::MakeWorldMatrix(AliceMathF::Matrix4* mat_)
{
	AliceMathF::Matrix4 lMatScale, lMatRot, lMatTrans;

	//スケール、回転、平行移動行列の計算
	lMatScale.MakeScaling(scale);
	lMatRot.MakeRotation(rotation);
	lMatTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= lMatScale;
	//ワールド行列に回転を反映
	matWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	matWorld *= lMatTrans;
	if ( mat_ )
	{
		matWorld *= *mat_;
	}
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	rigidBodyMatWorld = AliceMathF::MakeIdentity();
	//ワールド行列に回転を反映
	rigidBodyMatWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	rigidBodyMatWorld *= lMatTrans;
	//親行列の指定がある場合は、掛け算する
	if ( mat_ )
	{
		rigidBodyMatWorld *= *mat_;
	}
	if ( parent )
	{
		rigidBodyMatWorld *= parent->rigidBodyMatWorld;
	}

}

void Transform::LookAtMatrixAxisFix(const AliceMathF::Vector3& target_, const AliceMathF::Vector3& up_, Camera* camera_)
{
	AliceMathF::Matrix4 lMatScale, lMatRot, lMatTrans;
	AliceMathF::Vector3 lTarget = translation + target_;
	//スケール、回転、平行移動行列の計算
	lMatScale.MakeScaling(scale);
	lMatTrans.MakeTranslation(translation);

	AliceMathF::Vector3 X, Y, Z, D;
	D = lTarget - translation;
	D = D.Normal();
	Y = up_.Normal();
	X = Y.Cross(D);
	X=X.Normal();
	Z = X.Cross(Y);
	Z = Z.Normal();

	lMatRot.m[0][0] = X.x;
	lMatRot.m[0][1] = X.y;
	lMatRot.m[0][2] = X.z;
	lMatRot.m[0][3] = 0.0f;

	lMatRot.m[1][0] = Y.x;
	lMatRot.m[1][1] = Y.y;
	lMatRot.m[1][2] = Y.z;
	lMatRot.m[0][3] = 0.0f;

	lMatRot.m[2][0] = Z.x;
	lMatRot.m[2][1] = Z.y;
	lMatRot.m[2][2] = Z.z;
	lMatRot.m[2][3] = 0.0f;

	lMatRot.m[3][0] = 0.0f;
	lMatRot.m[3][1] = 0.0f;
	lMatRot.m[3][2] = 0.0f;
	lMatRot.m[3][3] = 1.0f;

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= lMatScale;
	//ワールド行列に回転を反映
	matWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	matWorld *= lMatTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	rigidBodyMatWorld = AliceMathF::MakeIdentity();
	//ワールド行列に回転を反映
	rigidBodyMatWorld *= lMatRot;
	//ワールド行列に平行移動を反映
	rigidBodyMatWorld *= lMatTrans;
	//親行列の指定がある場合は、掛け算する
	if ( parent )
	{
		rigidBodyMatWorld *= parent->rigidBodyMatWorld;
	}

	//定数バッファに書き込み
	constBuffMap.matWorld = matWorld * camera_->GetViewMatrixInv() * camera_->GetProjectionMatrix();
	constBuffMap.world = matWorld;
	constBuffMap.cameraPos = camera_->GetEye();
	Update();
}


const ID3D12Resource* Transform::GetconstBuff()const
{
	return constBuff->GetResource();
}

worldViewpojCamera* Transform::GetWorldViewpojCamera()
{
	return &constBuffMap;
}

const D3D12_GPU_VIRTUAL_ADDRESS& Transform::GetAddress()const
{
	return constBuff->GetAddress();
}
