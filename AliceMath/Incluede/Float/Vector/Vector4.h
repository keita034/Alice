#pragma once
#include"ErrorException.h"

namespace AliceMathF
{
	//4次元ベクトル
	class Vector4
	{
	public:
		float x;//x成分
		float y;//y成分
		float z;//z成分
		float w;//w

	public:
		//コンストラクタ
		Vector4();//零ベクトル
		Vector4(float x, float y, float z, float w);//成分を指定して生成

		Vector4(aiQuaternion& aiQuaternion);//成分を指定して生成

		Vector4(const DirectX::XMVECTOR& vec);//成分を指定して生成

		float Length()const;//ノルムを求める
		float LengthSquared()const;
		Vector4 Normalization()const;//正規化する
		Vector4& Normal();//正規化する
		float Dot(const Vector4& v)const;//内積を求める
		Vector4 Cross(const Vector4& v)const;//外積を求める

		float Vector3Length()const;//ノルムを求める
		float Vector3LengthSquared()const;
		Vector4 Vector3Normalization()const;//正規化する
		Vector4& Vector3Normal();//正規化する
		float Vector3Dot(const Vector4& v)const;//内積を求める
		Vector4 Vector3Cross(const Vector4& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector4 operator+() const;
		Vector4 operator-() const;

		// 代入演算子オーバーロード
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(float s);
		Vector4& operator/=(float s);
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector4 operator+(const Vector4& v1, const Vector4& v2);

	const Vector4 operator-(const Vector4& v1, const Vector4& v2);
	const Vector4 operator*(const Vector4& v, float s);
	const Vector4 operator*(float s, const Vector4& v);
	const Vector4 operator/(const Vector4& v, float s);

	void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest);

	Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t);
}

