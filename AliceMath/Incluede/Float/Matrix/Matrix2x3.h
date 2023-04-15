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
	class Matrix2x3
	{
	public:
		std::array<std::array<float, 2>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix2x3();

		Matrix2x3(
			float m00, float m01, float m02,
			float m10, float m11, float m12);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x3& operator=(const Matrix2x3& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x3& operator=(Matrix2x3& _m);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2x3& operator+=(const Matrix2x3& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2x3& operator-=(const Matrix2x3 mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2x3& operator*=(const Matrix2x3& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2x3 operator+(const Matrix2x3& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2x3 operator-(const Matrix2x3& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2x3 operator*(const Matrix2x3& mat) const;


		Matrix2x3 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix2x3 MakeMatrix2x3Identity();

}