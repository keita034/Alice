#pragma once
#include"ErrorException.h"
namespace AliceMathF
{
	class Matrix4x3
	{
	public:
		std::array<std::array<float, 4>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix4x3();

		Matrix4x3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22,
			float m30, float m31, float m32);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x3(const DirectX::XMFLOAT4X3& Matrix4X3);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMFLOAT4X3() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x3& operator=(const Matrix4x3& _m);
		const Matrix4x3& operator=(Matrix4x3& _m);
		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x3& operator+=(const Matrix4x3& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x3& operator-=(const Matrix4x3 mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x3& operator*=(const Matrix4x3& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4x3 operator+(const Matrix4x3& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4x3 operator-(const Matrix4x3& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4x3 operator*(const Matrix4x3& mat) const;

		Matrix4x3 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4x3 MakeMatrix4x3Identity();

}