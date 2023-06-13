#pragma once
#pragma warning(push)
#pragma warning(disable: 4062)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<DirectXMath.h>
#include<Windows.h>
#pragma warning(pop)

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
		Vector2(float x_, float y_);//x成分、y成分を指定しての生成
		Vector2(size_t x_, size_t y_);//x成分、y成分を指定しての生成
		Vector2(int16_t x_, int16_t y_);//x成分、y成分を指定しての生成
		Vector2(int32_t x_, int32_t y_);//x成分、y成分を指定しての生成
		Vector2(int64_t x_, int64_t y_);//x成分、y成分を指定しての生成

		//メンバ関数
		float length_()const;//ノルムを求める
		Vector2& Normalization();//正規化する
		float Dot(const Vector2& v_)const;//内積を求める
		float Cross(const Vector2& v_)const;//外積を求める

		//単項演算子オーバーロード
		Vector2 operator+()const;
		Vector2 operator-()const;

		//代入演算子オーバーロード
		Vector2& operator+=(const Vector2& v_);
		Vector2& operator-=(const Vector2& v_);
		Vector2& operator*=(float s_);
		Vector2& operator/=(float s_);
		Vector2& operator/=(const Vector2& v_);

		bool operator==(const Vector2& v_);

	};

	//2項演算子
	const Vector2 operator+(const Vector2& v1_, const Vector2& v2_);
	const Vector2 operator-(const Vector2& v1_, const Vector2& v2_);
	const Vector2 operator*(const Vector2& v_, float s_);
	const Vector2 operator*(float s_, const Vector2& v_);
	const Vector2 operator/(const Vector2& v_, float s_);
}

//点
typedef AliceMathF::Vector2 Point2D;