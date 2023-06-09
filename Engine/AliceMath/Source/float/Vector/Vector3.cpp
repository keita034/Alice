#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)

#include<assimp/vector3.h>

#pragma warning(pop)

#include "Vector3.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f)
	{}

	Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_)
	{}

	Vector3::Vector3(int64_t x_, int64_t y_, int64_t z_) : x(static_cast<float>(x_)), y(static_cast<float>(y_)), z(static_cast<float>(z_))
	{}

	Vector3::Vector3(uint32_t x_, uint32_t y_, uint32_t z_) : x(static_cast<float>(x_)), y(static_cast<float>(y_)), z(static_cast<float>(z_))
	{}

	Vector3::Vector3(int32_t x_, int32_t y_, int32_t z_) : x(static_cast<float>(x_)), y(static_cast<float>(y_)), z(static_cast<float>(z_))
	{}

	Vector3::Vector3(aiVector3D& aiVector_)
	{
		x = aiVector_.x;
		y = aiVector_.y;
		z = aiVector_.z;
	}

	Vector3::Vector3(const Vector2& vec_, float z_):z(z_)
	{
		x = vec_.x;
		y = vec_.y;
	}

	float Vector3::Length() const
	{
		return Sqrt(x * x + y * y + z * z);
	}

	Vector3& Vector3::Normal()
	{
		float lLen = Length();
		if (lLen != 0)
		{
			return *this /= lLen;
		}

		return *this;
	}

	Vector3 Vector3::Normal() const
	{
		Vector3 lTmp(*this);

		float lLen = Length();
		if (lLen != 0)
		{
			return lTmp /= lLen;
		}

		return lTmp;
	}

	float Vector3::Dot(const Vector3& v_) const
	{
		return x * v_.x + y * v_.y + z * v_.z;
	}

	Vector3 Vector3::Cross(const Vector3& v_) const
	{
		Vector3 w;
		w.x = y * v_.z - z * v_.y;
		w.y = z * v_.x - x * v_.z;
		w.z = x * v_.y - y * v_.x;

		return w;
	}

	Vector3 Vector3::operator+() const
	{
		return *this;
	}

	Vector3 Vector3::operator-() const
	{
		return  Vector3(-x, -y, -z);
	}

	Vector3& Vector3::operator+=(const Vector3& v_)
	{
		x += v_.x;
		y += v_.y;
		z += v_.z;

		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v_)
	{
		x -= v_.x;
		y -= v_.y;
		z -= v_.z;

		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& v_)
	{
		x *= v_.x;
		y *= v_.y;
		z *= v_.z;

		return *this;
	}

	Vector3& Vector3::operator-=(float s_)
	{
		x -= s_;
		y -= s_;
		z -= s_;

		return *this;
	}

	Vector3& Vector3::operator*=(float s_)
	{
		x *= s_;
		y *= s_;
		z *= s_;

		return *this;
	}

	Vector3& Vector3::operator/=(float s_)
	{
		x /= s_;
		y /= s_;
		z /= s_;

		return *this;
	}

	bool Vector3::operator==(const Vector3& v_) const
	{
		if (Approximately(this->x, v_.x) && Approximately(this->y, v_.y) && Approximately(this->z, v_.z))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Vector3::operator!=(const Vector3& v_) const
	{
		if (Approximately(this->x, v_.x) && Approximately(this->y, v_.y) && Approximately(this->z, v_.z))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	const Vector3 operator+(const Vector3& v1_, const Vector3& v2_)
	{
		Vector3 lTmp(v1_);
		return lTmp += v2_;
	}

	const Vector3 operator-(const Vector3& v1_, const Vector3& v2_)
	{
		Vector3 lTmp(v1_);
		return lTmp -= v2_;
	}

	const Vector3 operator-(const Vector3& v_, float s_)
	{
		Vector3 lTmp(v_);
		return lTmp -= s_;
	}

	const Vector3 operator*(const Vector3& v_, float s_)
	{
		Vector3 lTmp(v_);
		return lTmp *= s_;
	}

	const Vector3 operator*(const Vector3& v_, const Vector3& v2_)
	{
		Vector3 lTmp(v_);
		return lTmp *= v2_;
	}

	const Vector3 operator*(float s_, const Vector3& v_)
	{
		return v_ * s_;
	}

	const Vector3 operator/(const Vector3& v_, float s_)
	{
		Vector3 lTmp(v_);
		return lTmp /= s_;
	}

	Vector3 Negate(const Vector3& vec_)
	{
		Vector3 lRetVec;
		lRetVec.x = -vec_.x;
		lRetVec.y = -vec_.y;
		lRetVec.z = -vec_.z;

		return lRetVec;
	}

}