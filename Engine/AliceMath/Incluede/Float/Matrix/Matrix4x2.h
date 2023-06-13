#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<array>
#include<DirectXMath.h>

#pragma warning(pop)

namespace AliceMathF
{
	struct Matrix4x2
	{

		std::array<std::array<float, 4>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix4x2();

		Matrix4x2(float m00_, float m01_, 
			float m10_, float m11_,
			float m20_, float m21_,
			float m30_, float m31_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x2& operator=(const Matrix4x2& mat_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4x2& operator=(Matrix4x2& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x2& operator+=(const Matrix4x2& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x2& operator-=(const Matrix4x2 mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x2& operator*=(const Matrix4x2& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x2 operator+(const Matrix4x2& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x2 operator-(const Matrix4x2& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x2 operator*(const Matrix4x2& mat_) const;

		Matrix4x2 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4x2 MakeMatrix4x2Identity();

}

