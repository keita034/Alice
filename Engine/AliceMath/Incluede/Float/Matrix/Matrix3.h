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

template <typename TReal>
class aiMatrix3x3t;
typedef float ai_real;
typedef aiMatrix3x3t<ai_real> aiMatrix3x3;

namespace AliceMathF
{

	struct  Matrix3
	{
		std::array<std::array<float, 3>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix3();

		Matrix3(
			float m00_, float m01_, float m02_,
			float m10_, float m11_, float m12_,
			float m20_, float m21_, float m22_
		);

		Matrix3(const Vector2& scale_, float angle_, const Vector2& trans_);

		Matrix3(const aiMatrix3x3& mat_);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3(const DirectX::XMFLOAT3X3& matrix3_);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="scale"> : 拡大率</param>
		void MakeScaling(const Vector2& scale_);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="x,y"> : 拡大率</param>
		void MakeScaling(float x_, float y_);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="trans"> : 平行移動</param>
		void MakeTranslation(const Vector2& trans_);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="x,y"> : 平行移動</param>
		void MakeTranslation(float x_, float y_);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotation(float angle_);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMFLOAT3X3() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3& operator=(const Matrix3& m_);
		const Matrix3& operator=(Matrix3& m_);
		/// <summary>
		/// 加算
		/// </summary>
		Matrix3& operator+=(const Matrix3& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3& operator-=(const Matrix3& mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3& operator*=(const Matrix3& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3 operator+(const Matrix3& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3 operator-(const Matrix3& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3 operator*(const Matrix3& mat_) const;

		Matrix3 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix3 MakeMat3Identity();

	/// <summary>
	/// 逆行列を求める
	/// </summary>
	/// <param name="mat">逆行列にしたい行列</param>
	/// <returns>逆行列or単位行列</returns>
	Matrix3 MakeInverse(const Matrix3* mat_);
}

