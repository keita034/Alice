#include "TriangleCollider.h"

void TriangleCollider::ComputeNormal()
{
	AliceMathF::Vector4 lP0_P1 = p1 - p0;
	AliceMathF::Vector4 lP0_P2 = p2 - p0;

	//外積により、2辺に垂直なベクトルを算出する
	normal = lP0_P1.Cross(lP0_P2);
	normal = normal.Normal();
}

const AliceMathF::Vector4& TriangleCollider::GetP0()const
{
	return p0;
}

void TriangleCollider::SetP0(const AliceMathF::Vector4& p0_)
{
	p0 = p0_;
}

const AliceMathF::Vector4& TriangleCollider::GetP1()const
{
	return p1;
}

void TriangleCollider::SetP1(const AliceMathF::Vector4& p1_)
{
	p1 = p1_;
}

const AliceMathF::Vector4& TriangleCollider::GetP2()const
{
	return p2;
}

void TriangleCollider::SetP2(const AliceMathF::Vector4& p2_)
{
	p2 = p2_;
}

const AliceMathF::Vector4& TriangleCollider::GetNormal()const
{
	return normal;
}

void TriangleCollider::SetNormal(const AliceMathF::Vector4& normal_)
{
	normal = normal_;
}
