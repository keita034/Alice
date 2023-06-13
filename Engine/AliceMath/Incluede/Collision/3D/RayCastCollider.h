#pragma once
#include"RayCollider.h"

class RayCastCollider :public RayCollider
{
protected:
	//距離
	float range = 0.0f;
private:
	char PADING[4]{};
public:
	//半径を取得
	float GetRange()const;
	//半径をを設定
	void SetRange(float range_);

};
