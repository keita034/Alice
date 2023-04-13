#pragma once
#include"ErrorException.h"

namespace AliceMathF
{
	class Matrix4x2
	{
	public:
		std::array<std::array<float, 4>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix4x2();

		Matrix4x2(float m00, float m01, 
			float m10, float m11,
			float m20, float m21,
			float m30, float m31);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x2& operator=(const Matrix4x2& mat);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4x2& operator=(Matrix4x2& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x2& operator+=(const Matrix4x2& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x2& operator-=(const Matrix4x2 mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x2& operator*=(const Matrix4x2& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x2 operator+(const Matrix4x2& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x2 operator-(const Matrix4x2& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x2 operator*(const Matrix4x2& mat) const;


		Matrix4x2 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4x2 MakeMatrix4x2Identity();

}

