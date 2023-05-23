#pragma once
#include"BaseCollider.h"

class TriangleCollider : public BaseCollider
{
protected:

	//頂点座標3つ
	AliceMathF::Vector4 p0;
	AliceMathF::Vector4 p1;
	AliceMathF::Vector4 p2;
	//法線ベクトル
	AliceMathF::Vector4 normal;

public:

	TriangleCollider() = default;
	virtual ~TriangleCollider() = default;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();

	//頂点座標1つ目取得
	const AliceMathF::Vector4& GetP0();
	//頂点座標1つ目を設定
	void SetP0(const AliceMathF::Vector4& p0_);

	//頂点座標2つ目を取得
	const AliceMathF::Vector4& GetP1();
	//頂点座標2つ目を設定
	void SetP1(const AliceMathF::Vector4& p1_);

	//頂点座標3つ目を取得
	const AliceMathF::Vector4& GetP2();
	//頂点座標3つ目を設定
	void SetP2(const AliceMathF::Vector4& p2_);

	//法線ベクトルを取得
	const AliceMathF::Vector4& GetNormal();
	//法線ベクトルを設定
	void SetNormal(const AliceMathF::Vector4& normal_);

private:
};

