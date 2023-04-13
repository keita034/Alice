#pragma once
#include"ErrorException.h"

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
		Vector3(float x, float y, float z);//成分を指定して生成
		Vector3(aiVector3D& aiVector);
		Vector3(const Vector2& vec,float z);

		float Length()const;//ノルムを求める
		Vector3 Normalization()const;//正規化する
		Vector3& Normal();//正規化する
		float Dot(const Vector3& v)const;//内積を求める
		Vector3 Cross(const Vector3& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector3 operator+() const;
		Vector3 operator-() const;

		// 代入演算子オーバーロード
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator-=(float s);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
		bool operator==(const Vector3& v)const;
		bool operator!=(const Vector3& v) const;
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector3 operator+(const Vector3& v1, const Vector3& v2);
	const Vector3 operator-(const Vector3& v1, const Vector3& v2);
	const Vector3 operator-(const Vector3& v, float s);
	const Vector3 operator*(const Vector3& v, float s);
	const Vector3 operator*(float s, const Vector3& v);
	const Vector3 operator/(const Vector3& v, float s);

	/// <summary>
	/// 符号を反転
	/// </summary>
	Vector3 Negate(const Vector3& vec);


}

