#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<array>

#pragma warning(pop)

namespace AliceMathF
{
	struct Matrix3x2
	{
		std::array<std::array<float, 3>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix3x2();

		Matrix3x2(
			float m00_, float m01_,
			float m10_, float m11_, 
			float m20_, float m21_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x2& operator=(const Matrix3x2& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x2& operator=(Matrix3x2& m_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x2& operator+=(const Matrix3x2& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x2& operator-=(const Matrix3x2 mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x2& operator*=(const Matrix3x2& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x2 operator+(const Matrix3x2& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x2 operator-(const Matrix3x2& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x2 operator*(const Matrix3x2& mat_) const;

		Matrix3x2 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix3x2 MakeMatrix3x2Identity();

}

