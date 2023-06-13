#pragma once
#include"BaseCollider.h"

class AABBCollider : public BaseCollider
{
private:
	//中心座標
	AliceMathF::Vector3 center;
	//サイズ
	AliceMathF::Vector3 size;
public:
	void SetCenter(const AliceMathF::Vector3& center_);
	const AliceMathF::Vector3& GetCenter()const;

	void SetSize(const AliceMathF::Vector3& size_);
	const AliceMathF::Vector3& GetSize()const;
};

