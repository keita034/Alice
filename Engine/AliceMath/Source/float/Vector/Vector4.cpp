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

	Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{
	}

	Vector4::Vector4(aiQuaternion& quaternion)
	{
		x = quaternion.x;
		y = quaternion.y;
		z = quaternion.z;
		w = quaternion.w;
	}

	Vector4::Vector4(const DirectX::XMVECTOR& vec)
	{
		x = vec.m128_f32[0];
		y = vec.m128_f32[1];
		z = vec.m128_f32[2];
		w = vec.m128_f32[3];
	}

	float Vector4::length_() const
	{
		float len = length_Squared();
		return Sqrt(len);
	}

	float Vector4::length_Squared() const
	{
		return Dot(*this);
	}

	Vector4 Vector4::Normalization() const
	{
		Vector4 tmp(*this);

		float len = length_();
		if (len != 0)
		{
			return tmp /= len;

		}

		return tmp;
	}

	Vector4& Vector4::Normal()
	{
		float len = length_();
		if (len != 0)
		{
			return *this /= len;

		}

		return *this;
	}

	float Vector4::Dot(const Vector4& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	Vector4 Vector4::Cross(const Vector4& v) const
	{
		Vector3 v1 = { x,y,z };
		Vector3 v2 = { v.x,v.y,v.z };
		Vector3 temp = v1.Cross(v2);
		return { temp.x,temp.y,temp.z,0.0f };
	}
	float Vector4::Vector3length_() const
	{
		float len = Vector3length_Squared();
		return Sqrt(len);
	}

	float Vector4::Vector3length_Squared() const
	{
		return Vector3Dot(*this);
	}

	Vector4 Vector4::Vector3Normalization() const
	{
		Vector4 tmp(*this);

		float len = Vector3length_();
		if (len != 0)
		{
			return tmp /= len;

		}

		return tmp;
	}

	Vector4& Vector4::Vector3Normal()
	{
		float len = Vector3length_();
		if (len != 0)
		{
			return *this /= len;

		}

		return *this;
	}

	float Vector4::Vector3Dot(const Vector4& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vector4 Vector4::Vector3Cross(const Vector4& v) const
	{
		Vector3 v1 = { x,y,z };
		Vector3 v2 = { v.x,v.y,v.z };
		Vector3 temp = v1.Cross(v2);
		return { temp.x,temp.y,temp.z,w };
	}

	Vector4 Vector4::operator+() const
	{
		return *this;
	}

	Vector4 Vector4::operator-() const
	{
		return  Vector4(-x, -y, -z, -w);
	}

	Vector4& Vector4::operator+=(const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;

		return *this;
	}

	Vector4& Vector4::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;

		return *this;
	}

	Vector4& Vector4::operator/=(float s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;

		return *this;
	}

	const Vector4 operator+(const Vector4& v1, const Vector4& v2)
	{
		Vector4 temp(v1);
		return temp += v2;
	}

	const Vector4 operator-(const Vector4& v1, const Vector4& v2)
	{
		Vector4 temp(v1);
		return temp -= v2;
	}

	const Vector4 operator*(const Vector4& v, float s)
	{
		Vector4 temp(v);
		return temp *= s;
	}

	const Vector4 operator*(float s, const Vector4& v)
	{
		return v * s;
	}

	const Vector4 operator/(const Vector4& v, float s)
	{
		Vector4 temp(v);
		return temp /= s;
	}

	Vector4 Vector4Lerp(const Vector4& src1, const Vector4& src2, float t)
	{
		Vector4 temp;
		Vector4Lerp(src1, src2, t, temp);
		return temp;
	}

	void Vector4Lerp(const Vector4& src1, const Vector4& src2, float t, Vector4& dest)
	{
		dest.x = src1.x + (src2.x - src1.x) * t;
		dest.y = src1.y + (src2.y - src1.y) * t;
		dest.z = src1.z + (src2.z - src1.z) * t;
		dest.w = src1.w + (src2.w - src1.w) * t;
	}
}