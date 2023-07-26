#include "Vector.h"
#include"AliceMathUtility.h"

namespace AliceMathF
{
	const Vector4 operator+(const Vector3& v1_, const Vector4& v2_)
	{
		Vector4 lTmp(v2_);
		lTmp.x += v1_.x;
		lTmp.y += v1_.y;
		lTmp.z += v1_.z;
		lTmp.w += 0;
		return lTmp;
	}

	// Vector4をVector3に変換
	Vector3 TrnsVec3(const Vector4& vec_)
	{
		return { vec_.x,vec_.y ,vec_.z };
	}

	// Vector3をVector4に変換
	Vector4 TrnsVec4(const Vector3& vec_, bool matFlag_)
	{
		if (matFlag_)
		{
			return { vec_.x,vec_.y,vec_.z,1.0f };
		}
		return { vec_.x,vec_.y,vec_.z,0.0f };
	}

	// ベクトルと行列の掛け算
	Vector3 Vec3Mat4Mul(const Vector3& vec_, const Matrix4& mat_)
	{
		Vector3 retVec = {};

		retVec.x = vec_.x * mat_.m[0][0] + vec_.y * mat_.m[1][0] + vec_.z * mat_.m[2][0];

		retVec.y = vec_.x * mat_.m[0][1] + vec_.y * mat_.m[1][1] + vec_.z * mat_.m[2][1];

		retVec.z = vec_.x * mat_.m[0][2] + vec_.y * mat_.m[1][2] + vec_.z * mat_.m[2][2];

		return retVec;;
	}

	Vector2 Vec2Mat3Mul(const Vector2& vec_, const Matrix3& mat_)
	{
		Vector2 retVec = {};

		retVec.x = vec_.x * mat_.m[0][0] + vec_.y * mat_.m[1][0] + 1.0f * mat_.m[2][0];

		retVec.y = vec_.x * mat_.m[0][1] + vec_.y * mat_.m[1][1] + 1.0f * mat_.m[2][1];


		return retVec;
	}

	Vector3 Vec3Mat4MulWdiv(const Vector3& vec_, const Matrix4& mat_)
	{
		Vector4 retVec = {};

		retVec.x = vec_.x * mat_.m[0][0] + vec_.y * mat_.m[1][0] + vec_.z * mat_.m[2][0] + 1 * mat_.m[3][0];

		retVec.y = vec_.x * mat_.m[0][1] + vec_.y * mat_.m[1][1] + vec_.z * mat_.m[2][1] + 1 * mat_.m[3][1];

		retVec.z = vec_.x * mat_.m[0][2] + vec_.y * mat_.m[1][2] + vec_.z * mat_.m[2][2] + 1 * mat_.m[3][2];

		retVec.w = vec_.x * mat_.m[0][3] + vec_.y * mat_.m[1][3] + vec_.z * mat_.m[2][3] + 1 * mat_.m[3][3];

		retVec /= retVec.w;

		return { retVec.x, retVec.y, retVec.z };
	}

	// ベクトルと行列の掛け算
	Vector4 Vec4Mat4Mul(const Vector4& vec_, const Matrix4& mat_)
	{
		Vector4 retVec = {};

		retVec.x = vec_.x * mat_.m[0][0] + vec_.y * mat_.m[1][0] + vec_.z * mat_.m[2][0] + vec_.w * mat_.m[3][0];

		retVec.y = vec_.x * mat_.m[0][1] + vec_.y * mat_.m[1][1] + vec_.z * mat_.m[2][1] + vec_.w * mat_.m[3][1];

		retVec.z = vec_.x * mat_.m[0][2] + vec_.y * mat_.m[1][2] + vec_.z * mat_.m[2][2] + vec_.w * mat_.m[3][2];

		return retVec;
	}

	Vector4 Mat4Vec4Mul(const Matrix4& mat_, const Vector4& vec_ )
	{
		Vector4 retVec = {};

		retVec.x = mat_.m[0][0] * vec_.x + mat_.m[1][0] * vec_.y + mat_.m[2][0] * vec_.z + mat_.m[3][0] * vec_.w;
		retVec.y = mat_.m[0][1] * vec_.x + mat_.m[1][1] * vec_.y + mat_.m[2][1] * vec_.z + mat_.m[3][1] * vec_.w;
		retVec.z = mat_.m[0][2] * vec_.x + mat_.m[1][2] * vec_.y + mat_.m[2][2] * vec_.z + mat_.m[3][2] * vec_.w;
		retVec.w = mat_.m[0][3] * vec_.x + mat_.m[1][3] * vec_.y + mat_.m[2][3] * vec_.z + mat_.m[3][3] * vec_.w;

		return retVec;
	}

	Vector3 Vector3Lerp(const Vector3& src1_, const Vector3& src2_, float t_)
	{
		Vector3 dest;

		dest.x = src1_.x + ((src2_.x - src1_.x) * t_);
		dest.y = src1_.y + ((src2_.y - src1_.y) * t_);
		dest.z = src1_.z + ((src2_.z - src1_.z) * t_);

		return dest;
	}

	Vector4 Vec4MulPs(const Vector4& vec_, const Vector4& vec2_)
	{
		Vector4 lTmp;

		lTmp = {
			vec_.x * vec2_.x,
			vec_.y * vec2_.y,
			vec_.z * vec2_.z,
			vec_.w * vec2_.w
		};

		return lTmp;
	}

	Vector4 Vec4AddPs(const Vector4& vec_, const Vector4& vec2_)
	{
		Vector4 lTmp;

		lTmp = {
			vec_.x + vec2_.x,
			vec_.y + vec2_.y,
			vec_.z + vec2_.z,
			vec_.w + vec2_.w
		};

		return lTmp;
	}
	Vector4 Vec4SetPs1(float value_)
	{
		return { value_,value_ ,value_ ,value_ };
	}
	Vector4 VectorReplicate(float value_)
	{
		return Vec4SetPs1(value_);
	}

	Vector4 VectorLess(const Vector4& vec_, const Vector4& vec2_)
	{
		return Vec4CmpltPs(vec_, vec2_);
	}
	Vector4 Vec4CmpltPs(const Vector4& vec_, const Vector4& vec2_)
	{
		Vector4 lTmp;
		lTmp.x = vec_.x < vec2_.x ? static_cast<float>(0xffffffff) : static_cast<float>(0x0);
		lTmp.y = vec_.y < vec2_.y ? static_cast<float>(0xffffffff) :static_cast<float>( 0x0);
		lTmp.z = vec_.z < vec2_.z ? static_cast<float>(0xffffffff) :static_cast<float>( 0x0);
		lTmp.w = vec_.w < vec2_.w ? static_cast<float>(0xffffffff) :static_cast<float>( 0x0);
		return lTmp;
	}
}