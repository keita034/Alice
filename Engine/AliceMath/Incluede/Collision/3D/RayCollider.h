#pragma once
#include"BaseCollider.h"

class RayCollider : public BaseCollider
{
protected:

	//始点座標
	AliceMathF::Vector4 start = { 0,0,0,1 };
	//方向
	AliceMathF::Vector4 dir = { 1,0,0,0 };

public:

	//始点座標を取得
	const AliceMathF::Vector4& GetStart()const;
	//始点座標を設定
	void SetStart(const AliceMathF::Vector4& start_);

	//方向を取得
	const AliceMathF::Vector4& GetDir()const;
	//方向を設定
	void SetDir(const AliceMathF::Vector4& dir_);

};

