#pragma once
#include<ErrorException.h>

//2次元ベクトル
namespace AliceMathF
{
	class Vector2
	{
	public:
		float x;//x成分
		float y;//y成分

	public:
		// コンストラクタ
		Vector2();//ゼロベクトルとして生成
		Vector2(float x, float y);//x成分、y成分を指定しての生成
		Vector2(size_t x, size_t y);//x成分、y成分を指定しての生成
		Vector2(SHORT x, SHORT y);//x成分、y成分を指定しての生成
		Vector2(int32_t x, int32_t y);//x成分、y成分を指定しての生成

		//メンバ関数
		float Length()const;//ノルムを求める
		Vector2& Normalization();//正規化する
		float Dot(const Vector2& v)const;//内積を求める
		float Cross(const Vector2& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector2 operator+()const;
		Vector2 operator-()const;

		//代入演算子オーバーロード
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
		Vector2& operator/=(const Vector2& v);

		bool operator==(const Vector2& v);

	};

	//2項演算子
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator/(const Vector2& v, float s);
}

//点
typedef AliceMathF::Vector2 Point2D;