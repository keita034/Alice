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

	Vector2::Vector2(float x_, float y_) : x(x_), y(y_)
	{
	}

	Vector2::Vector2(size_t x_, size_t y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_))
	{

	}

	Vector2::Vector2(int16_t x_, int16_t y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_))
	{

	}

	Vector2::Vector2(int32_t x_, int32_t y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_))
	{

	}

	Vector2::Vector2(int64_t x_, int64_t y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_))
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
		float lLen = length_();
		if (lLen != 0)
		{
			return *this /= lLen;

		}

		return *this;
	}

	// もう一方のベクトルとの内積
	float Vector2::Dot(const Vector2& v_) const
	{
		return x * v_.x + y * v_.y;
	}
	//外積を求める
	float Vector2::Cross(const Vector2& v_) const
	{
		return x * v_.y - v_.x * y;
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
	Vector2& Vector2::operator+=(const Vector2& v_)
	{
		x += v_.x;
		y += v_.y;

		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& v_)
	{
		x -= v_.x;
		y -= v_.y;

		return *this;
	}

	Vector2& Vector2::operator*=(float s_)
	{
		x *= s_;
		y *= s_;

		return *this;
	}

	Vector2& Vector2::operator/=(float s_)
	{
		x /= s_;
		y /= s_;

		return *this;
	}

	Vector2& Vector2::operator/=(const Vector2& v_)
	{
		x /= v_.x;
		y /= v_.y;
		return *this;
	}

	bool Vector2::operator==(const Vector2& v_)
	{
		if (Approximately(v_.x, x) && Approximately(v_.y, y))
		{
			return true;
		}

		return false;
	}

	//Vector2 クラスに属さない関数群
	//二項演算子
	const Vector2 operator+(const Vector2& v1_, const Vector2& v2_)
	{
		Vector2 lTmp(v1_);
		return lTmp += v2_;
	}

	const Vector2 operator-(const Vector2& v1_, const Vector2& v2_)
	{
		Vector2 lTmp(v1_);
		return lTmp -= v2_;
	}

	const Vector2 operator*(const Vector2& v_, float s_)
	{
		Vector2 lTmp(v_);
		return lTmp *= s_;
	}

	const Vector2 operator*(float s_, const Vector2& v_)
	{
		return v_ * s_;
	}

	const Vector2 operator/(const Vector2& v_, float s_)
	{
		Vector2 lTmp(v_);
		return lTmp /= s_;
	}
}