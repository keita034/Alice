#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<DirectXMath.h>
#include<Windows.h>
#pragma warning(pop)

template <typename TReal>
class aiVector3t;
typedef float ai_real;
typedef aiVector3t<ai_real> aiVector3D;

namespace AliceMathF
{
	class Vector2;

	//3次元ベクトル
	class Vector3
	{
	public:
		float x;//x成分
		float y;//y成分
		float z;//z成分

	public:
		//コンストラクタ
		Vector3();//零ベクトル
		Vector3(float x_, float y_, float z_);//成分を指定して生成
		Vector3(uint32_t x_, uint32_t y_, uint32_t z_);//成分を指定して生成
		Vector3(int32_t x_, int32_t y_, int32_t z_);//成分を指定して生成
		Vector3(int64_t x_, int64_t y_, int64_t z_);//成分を指定して生成
		Vector3(aiVector3D& aiVector_);
		Vector3(const Vector2& vec_,float z_);

		float Length()const;//ノルムを求める
		Vector3 Normal()const;//正規化する
		Vector3& Normal();//正規化する
		float Dot(const Vector3& v_)const;//内積を求める
		Vector3 Cross(const Vector3& v_)const;//外積を求める

		//単項演算子オーバーロード
		Vector3 operator+() const;
		Vector3 operator-() const;

		// 代入演算子オーバーロード
		Vector3& operator+=(const Vector3& v_);
		Vector3& operator-=(const Vector3& v_);
		Vector3& operator*=(const Vector3& v_);
		Vector3& operator-=(float s_);
		Vector3& operator*=(float s_);
		Vector3& operator/=(float s_);
		bool operator==(const Vector3& v_)const;
		bool operator!=(const Vector3& v_) const;
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector3 operator+(const Vector3& v1_, const Vector3& v2_);
	const Vector3 operator-(const Vector3& v1_, const Vector3& v2_);
	const Vector3 operator-(const Vector3& v_, float s_);
	const Vector3 operator*(const Vector3& v_, float s_);
	const Vector3 operator*(const Vector3& v_, const Vector3& v2_);
	const Vector3 operator*(float s_, const Vector3& v_);
	const Vector3 operator/(const Vector3& v_, float s_);

	/// <summary>
	/// 符号を反転
	/// </summary>
	Vector3 Negate(const Vector3& vec_);


}

