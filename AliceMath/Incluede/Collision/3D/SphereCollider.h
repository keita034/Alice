#pragma once
#include"BaseCollider.h"

/// <summary>
/// 球
/// </summary>
class SphereCollider : public BaseCollider
{
protected:

	//中心座標
	AliceMathF::Vector4 center = { 0.0f,0.0f,0.0f,1.0f };
	//半径
	float radius = 1.0f;
private:
	char PADING[4]{};

public:

	//半径を取得
	const float GetRadius();
	//半径をを設定
	void SetRadius(float radius_);

	//中心座標を取得
	const AliceMathF::Vector4& GetCenter();
	//中心座標を設定
	void SetCenter(const AliceMathF::Vector4& center_);

};

