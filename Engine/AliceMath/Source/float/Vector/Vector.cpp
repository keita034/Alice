#include "Vector.h"
#include"AliceMathUtility.h"

namespace AliceMathF
{
	const Vector4 operator+(const Vector3& v1, const Vector4& v2)
	{
		Vector4 tmp(v2);
		tmp.x += v1.x;
		tmp.y += v1.y;
		tmp.z += v1.z;
		tmp.w += 0;
		return tmp;
	}

	// Vector4をVector3に変換
	Vector3 TrnsVec3(const Vector4& vec)
	{
		return { vec.x,vec.y ,vec.z };
	}

	// Vector3をVector4に変換
	Vector4 TrnsVec4(const Vector3& vec, bool matFlag)
	{
		if (matFlag)
		{
			return { vec.x,vec.y,vec.z,1.0f };
		}
		return { vec.x,vec.y,vec.z,0.0f };
	}

	// ベクトルと行列の掛け算
	Vector3 Vec3Mat4Mul(const Vector3& vec, const Matrix4& mat)
	{
		Vector3 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + 1 * mat.m[3][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + 1 * mat.m[3][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + 1 * mat.m[3][2];

		return retVec;;
	}

	Vector2 Vec2Mat3Mul(const Vector2& vec, const Matrix3& mat)
	{
		Vector2 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + 1.0f * mat.m[2][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + 1.0f * mat.m[2][1];


		return retVec;
	}

	Vector3 Vec3Mat4MulWdiv(const Vector3& vec, const Matrix4& mat)
	{
		Vector4 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + 1 * mat.m[3][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + 1 * mat.m[3][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + 1 * mat.m[3][2];

		retVec.w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + 1 * mat.m[3][3];

		retVec /= retVec.w;

		return { retVec.x, retVec.y, retVec.z };
	}

	// ベクトルと行列の掛け算
	Vector4 Vec4Mat4Mul(const Vector4& vec, const Matrix4& mat)
	{
		Vector4 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];

		return retVec;
	}

	Vector3 Vector3Lerp(const Vector3& src1, const Vector3& src2, float t)
	{
		Vector3 dest;

		dest.x = src1.x + (src2.x - src1.x) * t;
		dest.y = src1.y + (src2.y - src1.y) * t;
		dest.z = src1.z + (src2.z - src1.z) * t;

		return dest;
	}

	Vector4 Vec4MulPs(const Vector4& vec, const Vector4& vec2)
	{
		Vector4 tmp;

		tmp = {
			vec.x * vec2.x,
			vec.y * vec2.y,
			vec.z * vec2.z,
			vec.w * vec2.w
		};

		return tmp;
	}

	Vector4 Vec4AddPs(const Vector4& vec, const Vector4& vec2)
	{
		Vector4 tmp;

		tmp = {
			vec.x + vec2.x,
			vec.y + vec2.y,
			vec.z + vec2.z,
			vec.w + vec2.w
		};

		return tmp;
	}
	Vector4 Vec4SetPs1(float value)
	{
		return { value,value ,value ,value };
	}
	Vector4 VectorReplicate(float value)
	{
		return Vec4SetPs1(value);
	}

	/*Vector4 QuaternionSlerpV(const Vector4& vec, const Vector4& vec2)
	{
		const Vector4 oneMinusEpsilon = { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f };
		const Vector4 signMask2 = { 0x80000000, 0x00000000, 0x00000000, 0x00000000 };

		float dot = vec.Dot(vec2);

		Vector4 cosOmega = Vec4SetPs1(dot);

		const Vector4 Zero = {0.0f,0.0f,0.0f,0.0f};
		Vector4 control = VectorLess(cosOmega, Zero);
		Vector4 Sign = XMVectorSelect(g_XMOne, g_XMNegativeOne, Control);

		cosOmega = Vec4MulPs(cosOmega, Sign);

		control = XMVectorLess(cosOmega, oneMinusEpsilon);

		Vector4 sinOmega = _mm_mul_ps(cosOmega, cosOmega);
		SinOmega = _mm_sub_ps(g_XMOne, sinOmega);
		SinOmega = _mm_sqrt_ps(sinOmega);

		Vector4 Omega = XMVectorATan2(sinOmega, cosOmega);

		Vector4 V01 = XM_PERMUTE_PS(T, _MM_SHUFFLE(2, 3, 0, 1));
		V01 = _mm_and_ps(V01, g_XMMaskXY);
		V01 = _mm_xor_ps(V01, SignMask2);
		V01 = _mm_add_ps(g_XMIdentityR0, V01);

		Vector4 S0 = _mm_mul_ps(V01, Omega);
		S0 = XMVectorSin(S0);
		S0 = _mm_div_ps(S0, sinOmega);

		S0 = XMVectorSelect(V01, S0, control);

		Vector4 S1 = XMVectorSplatY(S0);
		S0 = XMVectorSplatX(S0);

		S1 = _mm_mul_ps(S1, Sign);
		Vector4 result = _mm_mul_ps(Q0, S0);
		S1 = _mm_mul_ps(S1, Q1);
		result = _mm_add_ps(result, S1);
		return result;
	}*/
	Vector4 VectorLess(const Vector4& vec, const Vector4& vec2)
	{
		return Vec4CmpltPs(vec, vec2);
	}
	Vector4 Vec4CmpltPs(const Vector4& vec, const Vector4& vec2)
	{
		Vector4 tmp;
		tmp.x = vec.x < vec2.x ? static_cast<float>(0xffffffff) : static_cast<float>(0x0);
		tmp.y = vec.y < vec2.y ? static_cast<float>(0xffffffff) :static_cast<float>( 0x0);
		tmp.z = vec.z < vec2.z ? static_cast<float>(0xffffffff) :static_cast<float>( 0x0);
		tmp.w = vec.w < vec2.w ? static_cast<float>(0xffffffff) :static_cast<float>( 0x0);
		return tmp;
	}
}