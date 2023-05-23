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
	//原点点(0,0,0)からの距離を取得
	float GetDistance();
	//原点点(0,0,0)からの距離を設定
	void SetDistance(float distance_);
	//法線ベクトルを取得
	const AliceMathF::Vector4& GetNormal();
	//法線ベクトルを設定
	void SetNormal(const AliceMathF::Vector4& normal_);
};

