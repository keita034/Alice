#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<array>

#pragma warning(pop)

#include"Vector2.h"

namespace AliceMathF
{

	class Matrix2
	{
	public:
		std::array<std::array<float, 2>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix2();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2(
			float m00, float m01,
			float m10, float m11
		);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2& operator=(const Matrix2& _m);
		const Matrix2& operator=(Matrix2& _m);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2& operator+=(const Matrix2& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2& operator-=(const Matrix2& mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2& operator*=(const Matrix2& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix2 operator+(const Matrix2& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix2 operator-(const Matrix2& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix2 operator*(const Matrix2& mat) const;

		Matrix2 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix2 MakeMat2Identity();

	/// <summary>
	/// 逆行列を求める
	/// </summary>
	/// <param name="mat">逆行列にしたい行列</param>
	/// <returns>逆行列or単位行列</returns>
	Matrix2 MakeInverse(const Matrix2* mat);
}
