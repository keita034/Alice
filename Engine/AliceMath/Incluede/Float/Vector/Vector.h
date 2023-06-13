#pragma once
#include"Vector4.h"
#include"Vector3.h"
#include"Vector2.h"
#include"VectorT.h"

#include"Matrix3.h"
#include"Matrix4.h"

namespace AliceMathF
{
	const Vector4 operator+(const Vector3& v1_, const Vector4& v2_);

	/// <summary>
	/// Vector4をVector3に変換
	/// </summary>
	/// <param name="vec">変換するベクトル</param>
	/// <returns>Vector3</returns>
	Vector3 TrnsVec3(const Vector4& vec_);

	/// <summary>
	/// Vector3をVector4に変換
	/// </summary>
	/// <param name="vec">変換するベクトル</param>
	/// <param name="matFlag">wを1にするかどうか</param>
	/// <returns>Vector4</returns>
	Vector4 TrnsVec4(const Vector3& vec_, bool matFlag_ = false);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="mat">行列</param>
	/// <returns>計算された値</returns>
	Vector3 Vec3Mat4Mul(const Vector3& vec_, const Matrix4& mat_);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="mat">行列</param>
	/// <returns>計算された値</returns>
	Vector2 Vec2Mat3Mul(const Vector2& vec_, const Matrix3& mat_);

	/// <summary>
	/// ベクトルと行列の掛け算(W除算)
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="mat">行列</param>
	/// <returns>計算された値</returns>
	Vector3 Vec3Mat4MulWdiv(const Vector3& vec_, const Matrix4& mat_);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <param name="mat">行列</param>
	/// <returns>計算された値</returns>
	Vector4 Vec4Mat4Mul(const Vector4& vec_, const Matrix4& mat_);

	Vector3 Vector3Lerp(const Vector3& src1_, const Vector3& src2_, float t_);

	Vector4 Vec4MulPs(const Vector4& vec_, const Vector4& vec2_);

	Vector4 Vec4AddPs(const Vector4& vec_, const Vector4& vec2_);

	Vector4 Vec4SetPs1(float value_);

	Vector4 VectorReplicate(float value_);

	Vector4 VectorLess(const Vector4& vec_, const Vector4& vec2_);

	Vector4 Vec4CmpltPs(const Vector4& vec_, const Vector4& vec2_);
}