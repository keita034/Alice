#pragma once
#include<BaseTransform.h>

class Transform2D : public BaseTransform
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransUpdate(Camera* camera);

	void Update();

	void MakeWorldMatrix();

	const ID3D12Resource* GetconstBuff();

	worldViewpojCamera* GetWorldViewpojCamera();

	D3D12_GPU_VIRTUAL_ADDRESS GetAddress();
};

