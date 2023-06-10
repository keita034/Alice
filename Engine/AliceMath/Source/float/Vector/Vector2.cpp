#include "Vector2.h"
#include "Vector2.h"
#include "Vector2.h"
#include "Vector2.h"
#include"AliceMathUtility.h"

namespace AliceMathF
{
	//零ベクトル
	Vector2::Vector2() : x(0.0f), y(0.0f)
	{
	}

	Vector2::Vector2(float x, float y) : x(x), y(y)
	{
	}

	Vector2::Vector2(size_t x, size_t y) : x(static_cast<float>(x)), y(static_cast<float>(y))
	{

	}

	Vector2::Vector2(SHORT x, SHORT y) : x(static_cast<float>(x)), y(static_cast<float>(y))
	{

	}

	Vector2::Vector2(int32_t x, int32_t y) : x(static_cast<float>(x)), y(static_cast<float>(y))
	{
	}

	Vector2::Vector2(LONG x, LONG y) : x(static_cast<float>(x)), y(static_cast<float>(y))
	{
	}

	// ベクトルの長さを計算する
	float Vector2::length_() const
	{
		return Sqrt(x * x + y * y);
	}

	// 正規化ベクトル
	Vector2& Vector2::Normalization()
	{
		float len = length_();
		if (len != 0)
		{
			return *this /= len;

		}

		return *this;
	}

	// もう一方のベクトルとの内積
	float Vector2::Dot(const Vector2& v) const
	{
		return x * v.x + y * v.y;
	}
	//外積を求める
	float Vector2::Cross(const Vector2& v) const
	{
		return x * v.y - v.x * y;
	}

	//単項演算子オーバーロード
	Vector2 Vector2::operator+() const
	{
		return *this;
	}

	Vector2 Vector2::operator-() const
	{
		return  Vector2(-x, -y);
	}

	//代入演算子オーバーロード
	Vector2& Vector2::operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vector2& Vector2::operator*=(float s)
	{
		x *= s;
		y *= s;

		return *this;
	}

	Vector2& Vector2::operator/=(float s)
	{
		x /= s;
		y /= s;

		return *this;
	}

	Vector2& Vector2::operator/=(const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	bool Vector2::operator==(const Vector2& v)
	{
		if (Approximately(v.x, x) && Approximately(v.y, y))
		{
			return true;
		}

		return false;
	}

	//Vector2 クラスに属さない関数群
	//二項演算子
	const Vector2 operator+(const Vector2& v1, const Vector2& v2)
	{
		Vector2 temp(v1);
		return temp += v2;
	}

	const Vector2 operator-(const Vector2& v1, const Vector2& v2)
	{
		Vector2 temp(v1);
		return temp -= v2;
	}

	const Vector2 operator*(const Vector2& v, float s)
	{
		Vector2 temp(v);
		return temp *= s;
	}

	const Vector2 operator*(float s, const Vector2& v)
	{
		return v * s;
	}

	const Vector2 operator/(const Vector2& v, float s)
	{
		Vector2 temp(v);
		return temp /= s;
	}
}