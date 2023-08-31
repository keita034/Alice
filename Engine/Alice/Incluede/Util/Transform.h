#pragma once
#include<BaseTransform.h>


class Transform : public BaseTransform
{
public:

	Transform() = default;
	~Transform();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransUpdate(Camera* camera_,AliceMathF::Matrix4* mat_ = nullptr);

	void Update();

	void MakeWorldMatrix(AliceMathF::Matrix4* mat_ = nullptr);

	void LookAtMatrixAxisFix(const AliceMathF::Vector3& target_,const AliceMathF::Vector3& up_,Camera* camera_);

	const ID3D12Resource* GetconstBuff()const;

	worldViewpojCamera* GetWorldViewpojCamera();

	const D3D12_GPU_VIRTUAL_ADDRESS& GetAddress()const;

	void MakeRigidBodyMatWorld(AliceMathF::Matrix4* mat_ = nullptr);
};

