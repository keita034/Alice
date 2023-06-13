#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)

#include<assimp/quaternion.h>

#pragma warning(pop)

#include "Vector4.h"
#include"Vector3.h"
#include"AliceMathUtility.h"

namespace AliceMathF
{
	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
	}

	Vector4::Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_)
	{
	}


	Vector4::Vector4(aiQuaternion& quaternion_)
	{
		x = quaternion_.x;
		y = quaternion_.y;
		z = quaternion_.z;
		w = quaternion_.w;
	}

	Vector4::Vector4(const DirectX::XMVECTOR& vec_)
	{
		x = vec_.m128_f32[0];
		y = vec_.m128_f32[1];
		z = vec_.m128_f32[2];
		w = vec_.m128_f32[3];
	}

	float Vector4::length() const
	{
		float lLen = lengthSquared();
		return Sqrt(lLen);
	}

	float Vector4::lengthSquared() const
	{
		return Dot(*this);
	}

	Vector4 Vector4::Normalization() const
	{
		Vector4 lTmp(*this);

		float lLen = length();
		if (lLen != 0)
		{
			return lTmp /= lLen;

		}

		return lTmp;
	}

	Vector4& Vector4::Normal()
	{
		float lLen = length();
		if (lLen != 0)
		{
			return *this /= lLen;
		}
		return *this;
	}

	float Vector4::Dot(const Vector4& v_) const
	{
		return x * v_.x + y * v_.y + z * v_.z + w * v_.w;
	}

	Vector4 Vector4::Cross(const Vector4& v_) const
	{
		Vector3 lV1 = { x,y,z };
		Vector3 lV2 = { v_.x,v_.y,v_.z };
		Vector3 lTmp = lV1.Cross(lV2);
		return { lTmp.x,lTmp.y,lTmp.z,0.0f };
	}
	float Vector4::Vector3length_() const
	{
		float lLen = Vector3length_Squared();
		return Sqrt(lLen);
	}

	float Vector4::Vector3length_Squared() const
	{
		return Vector3Dot(*this);
	}

	Vector4 Vector4::Vector3Normalization() const
	{
		Vector4 lTmp(*this);

		float lLen = Vector3length_();
		if (lLen != 0)
		{
			return lTmp /= lLen;

		}

		return lTmp;
	}

	Vector4& Vector4::Vector3Normal()
	{
		float lLen = Vector3length_();
		if (lLen != 0)
		{
			return *this /= lLen;

		}

		return *this;
	}

	float Vector4::Vector3Dot(const Vector4& v_) const
	{
		return x * v_.x + y * v_.y + z * v_.z;
	}

	Vector4 Vector4::Vector3Cross(const Vector4& v_) const
	{
		Vector3 lV1 = { x,y,z };
		Vector3 lV2 = { v_.x,v_.y,v_.z };
		Vector3 lTmp = lV1.Cross(lV2);
		return { lTmp.x,lTmp.y,lTmp.z,w };
	}

	Vector4 Vector4::operator+() const
	{
		return *this;
	}

	Vector4 Vector4::operator-() const
	{
		return  Vector4(-x, -y, -z, -w);
	}

	Vector4& Vector4::operator+=(const Vector4& v_)
	{
		x += v_.x;
		y += v_.y;
		z += v_.z;
		w += v_.w;

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& v_)
	{
		x -= v_.x;
		y -= v_.y;
		z -= v_.z;
		w -= v_.w;

		return *this;
	}

	Vector4& Vector4::operator*=(float s_)
	{
		x *= s_;
		y *= s_;
		z *= s_;
		w *= s_;

		return *this;
	}

	Vector4& Vector4::operator/=(float s_)
	{
		x /= s_;
		y /= s_;
		z /= s_;
		w /= s_;

		return *this;
	}

	const Vector4 operator+(const Vector4& v1_, const Vector4& v2_)
	{
		Vector4 lTmp(v1_);
		return lTmp += v2_;
	}

	const Vector4 operator-(const Vector4& v1_, const Vector4& v2_)
	{
		Vector4 lTmp(v1_);
		return lTmp -= v2_;
	}

	const Vector4 operator*(const Vector4& v_, float s_)
	{
		Vector4 lTmp(v_);
		return lTmp *= s_;
	}

	const Vector4 operator*(float s_, const Vector4& v_)
	{
		return v_ * s_;
	}

	const Vector4 operator/(const Vector4& v_, float s_)
	{
		Vector4 lTmp(v_);
		return lTmp /= s_;
	}

	Vector4 Vector4Lerp(const Vector4& src1_, const Vector4& src2_, float t_)
	{
		Vector4 lTmp;
		Vector4Lerp(src1_, src2_, t_, lTmp);
		return lTmp;
	}

	void Vector4Lerp(const Vector4& src1_, const Vector4& src2_, float t_, Vector4& dest_)
	{
		dest_.x = src1_.x + (src2_.x - src1_.x) * t_;
		dest_.y = src1_.y + (src2_.y - src1_.y) * t_;
		dest_.z = src1_.z + (src2_.z - src1_.z) * t_;
		dest_.w = src1_.w + (src2_.w - src1_.w) * t_;
	}
}