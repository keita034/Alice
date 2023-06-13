#pragma once
#include"BaseCollider.h"

class PlaneCollider : public BaseCollider
{
protected:
	//法線ベクトル
	AliceMathF::Vector4 normal = { 0.0f,1.0f,0.0f,0.0f };
	//原点点(0,0,0)からの距離
	float distance = 0.0f;
private:
	char PADING[4]{};
public:

	/// <summary>
	/// 原点点(0,0,0)からの距離を取得
	/// </summary>
	float GetDistance()const;

	/// <summary>
	/// 原点点(0,0,0)からの距離を設定
	/// </summary>
	void SetDistance(float distance_);

	/// <summary>
	/// 法線ベクトルを取得
	/// </summary>
	const AliceMathF::Vector4& GetNormal()const;

	/// <summary>
	/// 法線ベクトルを設定
	/// </summary>
	void SetNormal(const AliceMathF::Vector4& normal_);
};

