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
	class Matrix3x2
	{
	public:
		std::array<std::array<float, 3>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix3x2();

		Matrix3x2(
			float m00, float m01,
			float m10, float m11, 
			float m20, float m21);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x2& operator=(const Matrix3x2& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x2& operator=(Matrix3x2& _m);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x2& operator+=(const Matrix3x2& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x2& operator-=(const Matrix3x2 mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x2& operator*=(const Matrix3x2& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x2 operator+(const Matrix3x2& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x2 operator-(const Matrix3x2& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x2 operator*(const Matrix3x2& mat) const;

		Matrix3x2 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix3x2 MakeMatrix3x2Identity();

}

