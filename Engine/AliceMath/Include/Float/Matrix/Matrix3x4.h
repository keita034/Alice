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
	struct Matrix3x4
	{
		std::array<std::array<float, 3>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix3x4();

		Matrix3x4(float m00_, float m01_, float m02_, float m03_,
			float m10_, float m11_, float m12_, float m13_,
			float m20_, float m21_, float m22_, float m23_);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x4(const DirectX::XMFLOAT3X4& xmfloat3x4_);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMFLOAT3X4() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x4& operator=(const Matrix3x4& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x4& operator=(Matrix3x4& m_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x4& operator+=(const Matrix3x4& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x4& operator-=(const Matrix3x4 mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x4& operator*=(const Matrix3x4& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x4 operator+(const Matrix3x4& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x4 operator-(const Matrix3x4& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x4 operator*(const Matrix3x4& mat_) const;

		Matrix3x4 Transpose();

		Matrix3x4(const Matrix3x4&);
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix3x4 MakeMatrix3x4Identity();

}

