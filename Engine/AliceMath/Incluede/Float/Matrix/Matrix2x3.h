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
	struct Matrix2x3
	{
		std::array<std::array<float, 2>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix2x3();

		Matrix2x3(
			float m00_, float m01_, float m02_,
			float m10_, float m11_, float m12_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x3& operator=(const Matrix2x3& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x3& operator=(Matrix2x3& m_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2x3& operator+=(const Matrix2x3& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2x3& operator-=(const Matrix2x3 mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2x3& operator*=(const Matrix2x3& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2x3 operator+(const Matrix2x3& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2x3 operator-(const Matrix2x3& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2x3 operator*(const Matrix2x3& mat_) const;


		Matrix2x3 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix2x3 MakeMatrix2x3Identity();

}