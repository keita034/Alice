#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)

#include<DirectXMath.h>
#pragma warning(pop)

template <typename TReal>
class aiQuaterniont;
typedef float ai_real;
typedef aiQuaterniont<ai_real> aiQuaternion;

namespace JPH
{
	class Vec4;
	class Color;
}

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
		Vector4(float x_, float y_, float z_, float w_);//成分を指定して生成
		Vector4(aiQuaternion& quaternion_);//成分を指定して生成
		Vector4(const DirectX::XMVECTOR& vec_);//成分を指定して生成
		Vector4(const JPH::Vec4& vec_);//成分を指定して生成

		float length()const;//ノルムを求める
		float lengthSquared()const;
		Vector4 Normal()const;//正規化する
		Vector4& Normal();//正規化する
		float Dot(const Vector4& v_)const;//内積を求める
		Vector4 Cross(const Vector4& v_)const;//外積を求める

		float Vector3length_()const;//ノルムを求める
		float Vector3length_Squared()const;
		Vector4 Vector3Normalization()const;//正規化する
		Vector4& Vector3Normal();//正規化する
		float Vector3Dot(const Vector4& v_)const;//内積を求める
		Vector4 Vector3Cross(const Vector4& v_)const;//外積を求める

		//単項演算子オーバーロード
		Vector4 operator+() const;
		Vector4 operator-() const;

		// 代入演算子オーバーロード
		Vector4& operator+=(const Vector4& v_);
		Vector4& operator-=(const Vector4& v_);
		Vector4& operator*=(float s_);
		Vector4& operator/=(float s_);

		operator JPH::Vec4()const;
		operator JPH::Color()const;
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector4 operator+(const Vector4& v1_, const Vector4& v2_);

	const Vector4 operator-(const Vector4& v1_, const Vector4& v2_);
	const Vector4 operator*(const Vector4& v_, float s_);
	const Vector4 operator*(float s_, const Vector4& v_);
	const Vector4 operator/(const Vector4& v_, float s_);

	void Vector4Lerp(const Vector4& src1_, const Vector4& src2_, float t_, Vector4& dest_);

	Vector4 Vector4Lerp(const Vector4& src1_, const Vector4& src2_, float t_);
}

