#pragma once
#include<BaseTransform.h>


class Transform : public BaseTransform
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransUpdate(Camera* camera_);

	void Update();

	void MakeWorldMatrix();

	const ID3D12Resource* GetconstBuff()const;

	worldViewpojCamera* GetWorldViewpojCamera();

	const D3D12_GPU_VIRTUAL_ADDRESS& GetAddress()const;
};

