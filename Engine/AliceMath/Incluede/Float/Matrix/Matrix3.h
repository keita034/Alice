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

	class Matrix3
	{
	public:
		std::array<std::array<float, 3>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix3();

		Matrix3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22
		);

		Matrix3(const Vector2& scale, float angle, const Vector2& trans);

		Matrix3(const aiMatrix3x3& mat);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3(const DirectX::XMFLOAT3X3& Matrix3);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="scale"> : 拡大率</param>
		void MakeScaling(const Vector2& scale);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="x,y"> : 拡大率</param>
		void MakeScaling(float x, float y);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="trans"> : 平行移動</param>
		void MakeTranslation(const Vector2& trans);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="x,y"> : 平行移動</param>
		void MakeTranslation(float x, float y);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotation(float angle);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMFLOAT3X3() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3& operator=(const Matrix3& _m);
		const Matrix3& operator=(Matrix3& _m);
		/// <summary>
		/// 加算
		/// </summary>
		Matrix3& operator+=(const Matrix3& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3& operator-=(const Matrix3& mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3& operator*=(const Matrix3& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix3 operator+(const Matrix3& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix3 operator-(const Matrix3& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix3 operator*(const Matrix3& mat) const;

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
	Matrix3 MakeInverse(const Matrix3* mat);
}

