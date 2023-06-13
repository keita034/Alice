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
	struct Matrix4x3
	{
		std::array<std::array<float, 4>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix4x3();

		Matrix4x3(float m00_, float m01_, float m02_,
			float m10_, float m11_, float m12_,
			float m20_, float m21_, float m22_,
			float m30_, float m31_, float m32_);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x3(const DirectX::XMFLOAT4X3& matrix4X3_);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMFLOAT4X3() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x3& operator=(const Matrix4x3& m_);
		const Matrix4x3& operator=(Matrix4x3& m_);
		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x3& operator+=(const Matrix4x3& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x3& operator-=(const Matrix4x3 mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x3& operator*=(const Matrix4x3& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x3 operator+(const Matrix4x3& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x3 operator-(const Matrix4x3& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x3 operator*(const Matrix4x3& mat_) const;

		Matrix4x3 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4x3 MakeMatrix4x3Identity();

}