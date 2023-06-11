#include<Transform2D.h>

void Transform2D::Initialize()
{
	//定数バッファ生成(3D座標変換行列)
	constBuff = CreateSharedConstantBuffer(sizeof(worldViewpojCamera));

	AliceMathF::Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale.MakeScaling(scale);
	matRot.MakeRotation(rotation);
	matTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= matScale;
	//ワールド行列に回転を反映
	matWorld *= matRot;
	//ワールド行列に平行移動を反映
	matWorld *= matTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	//定数バッファに書き込み
	constBuffMap.matWorld = matWorld * defaultViewMat * defaultProjectionMat;
	constBuffMap.world = matWorld;
	constBuffMap.cameraPos = { 0.0f,0.0f,-50.0f };

	Update();
}

void Transform2D::TransUpdate(Camera* camera)
{
	AliceMathF::Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale.MakeScaling(scale);
	matRot.MakeRotation(rotation);
	matTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= matScale;
	//ワールド行列に回転を反映
	matWorld *= matRot;
	//ワールド行列に平行移動を反映
	matWorld *= matTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

	//定数バッファに書き込み
	constBuffMap.matWorld = matWorld * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();
	constBuffMap.world = matWorld;
	constBuffMap.cameraPos = camera->GetEye();

	Update();
}

void Transform2D::Update()
{
	constBuff->Update(&constBuffMap);
}

void Transform2D::MakeWorldMatrix()
{
	AliceMathF::Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale.MakeScaling(scale);
	matRot.MakeRotation(rotation);
	matTrans.MakeTranslation(translation);

	//ワールド行列の合成
	//変形をリセット
	matWorld = AliceMathF::MakeIdentity();
	//ワールド行列にスケーリングを反映
	matWorld *= matScale;
	//ワールド行列に回転を反映
	matWorld *= matRot;
	//ワールド行列に平行移動を反映
	matWorld *= matTrans;
	//親行列の指定がある場合は、掛け算する
	if (parent)
	{
		matWorld *= parent->matWorld;
	}

}

const ID3D12Resource* Transform2D::GetconstBuff()
{
	return constBuff->GetResource();
}

worldViewpojCamera* Transform2D::GetWorldViewpojCamera()
{
	return &constBuffMap;
}

D3D12_GPU_VIRTUAL_ADDRESS Transform2D::GetAddress()
{
	return constBuff->GetAddress();
}
