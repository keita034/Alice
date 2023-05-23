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
	class Matrix3x4
	{
	public:
		std::array<std::array<float, 3>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix3x4();

		Matrix3x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x4(const DirectX::XMFLOAT3X4& XMFLOAT3X4);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMFLOAT3X4() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x4& operator=(const Matrix3x4& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x4& operator=(Matrix3x4& _m);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x4& operator+=(const Matrix3x4& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x4& operator-=(const Matrix3x4 mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x4& operator*=(const Matrix3x4& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3x4 operator+(const Matrix3x4& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3x4 operator-(const Matrix3x4& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3x4 operator*(const Matrix3x4& mat) const;

		Matrix3x4 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix3x4 MakeMatrix3x4Identity();

}

