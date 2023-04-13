#pragma once
#include"ErrorException.h"

namespace AliceMathF
{
	class Matrix2x4
	{
	public:
		std::array<std::array<float, 2>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix2x4();

		Matrix2x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x4& operator=(const Matrix2x4& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x4& operator=(Matrix2x4& _m);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2x4& operator+=(const Matrix2x4& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2x4& operator-=(const Matrix2x4 mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2x4& operator*=(const Matrix2x4& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2x4 operator+(const Matrix2x4& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2x4 operator-(const Matrix2x4& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2x4 operator*(const Matrix2x4& mat) const;


		Matrix2x4 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix2x4 MakeMatrix2x4Identity();

}
