#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<array>

#pragma warning(pop)

#include"Vector3.h"

template <typename TReal>
class aiMatrix4x4t;
typedef aiMatrix4x4t<ai_real> aiMatrix4x4;

namespace physx
{
	class PxMat44;
}

namespace AliceMathF
{
	class Quaternion;

	class Matrix4
	{
	public:
		std::array<std::array<float, 4>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix4();

		Matrix4(float m00_, float m01_, float m02_, float m03_,
			float m10_, float m11_, float m12_, float m13_,
			float m20_, float m21_, float m22_, float m23_,
			float m30_, float m31_, float m32_, float m33_);

		Matrix4(const Vector3& scale_, Quaternion& rotat_, const Vector3& trans_);

		Matrix4(const Vector3* scale_, Quaternion* rotat_, const Vector3* trans_);

		Matrix4(const Vector3* scale_, Vector3* rotat_, const Vector3* trans_);

		Matrix4(const aiMatrix4x4& mat_);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4(const DirectX::XMMATRIX& matrix4_);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="scale"> : 拡大率</param>
		void MakeScaling(const Vector3& scale_);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="x,y,z"> : 拡大率</param>
		void MakeScaling(float x_, float y_, float z_);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="trans"> : 平行移動</param>
		void MakeTranslation(const Vector3& trans_);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="x,y,z"> : 平行移動</param>
		void MakeTranslation(float x_, float y_, float z_);

		/// <summary>
		/// Y軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotationY(float angle_);

		/// <summary>
		/// Z軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotationZ(float angle_);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotationX(float angle_);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="Rotat"> : 回転角度(単位：ラジアン)</param>
		void MakeRotation(const Vector3& rotation_);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="x,y,z"> : 回転角度(単位：ラジアン)</param>
		void MakeRotation(float x_, float y_, float z_);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMMATRIX() const;

		operator physx::PxMat44() const;


		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4& operator=(const Matrix4& m_);
		const Matrix4& operator=(Matrix4& m_);
		/// <summary>
		/// 加算
		/// </summary>
		Matrix4& operator+=(const Matrix4& mat_);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4& operator-=(const Matrix4& mat_);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4& operator*=(const Matrix4& mat_);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4 operator+(const Matrix4& mat_) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4 operator-(const Matrix4& mat_) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4 operator*(const Matrix4& mat_) const;

		Matrix4 Transpose();
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4 MakeIdentity();

	/// <summary>
	/// 逆行列を求める
	/// </summary>
	/// <param name="mat">逆行列にしたい行列</param>
	/// <returns>逆行列or単位行列</returns>
	Matrix4 MakeInverse(const Matrix4* mat_);

	/// <summary>
	/// 平行投影変換行列を生成(左手系,原点左上)
	/// </summary>
	/// <param name="left"> : 左側</param>
	/// <param name="right"> : 右側</param>
	/// <param name="bottom"> : 下側</param>
	/// <param name="top"> : 上側</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakeOrthogonalLOffCenter(float left_, float right_, float bottom_, float top_, float near_, float far_, Matrix4& matrix_);

	/// <summary>
	/// 平行投影変換行列を生成(左手系)
	/// </summary>
	/// <param name="width"> : 横幅</param>
	/// <param name="height"> : 縦幅</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakeOrthogonalL(float width_, float height_, float near_, float far_, Matrix4& matrix_);

	/// <summary>
	/// 平行投影変換行列を生成(右手系)
	/// </summary>
	/// <param name="left"> : 左側</param>
	/// <param name="right"> : 右側</param>
	/// <param name="bottom"> : 下側</param>
	/// <param name="top"> : 上側</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakeOrthogonalR(float left_, float right_, float bottom_, float top_, float near_, float far_, Matrix4& matrix_);

	/// <summary>
	/// 透視投影変換行列を生成(左手系)
	/// </summary>
	/// <param name="fovAngleY"> : 上下画角</param>
	/// <param name="aspect"> : アスペクト比(画面横幅/画面縦幅)</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakePerspectiveL(float fovAngleY_, float aspect_, float near_, float far_, Matrix4& matrix_);

	/// <summary>
	/// 透視投影変換行列を生成(右手系)
	/// </summary>
	/// <param name="fovAngleY"> : 上下画角</param>
	/// <param name="aspect"> : アスペクト比(画面横幅/画面縦幅)</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakePerspectiveR(float fovAngleY_, float aspect_, float near_, float far_, Matrix4& matrix_);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="x">掛けられるベクトルのX座標</param>
	/// <param name="y">掛けられるベクトルのY座標</param>
	/// <param name="z">掛けられるベクトルのZ座標</param>
	/// <param name="mat">掛ける行列</param>
	void CoordinateTransformation3D(float& x_, float& y_, float& z_, Matrix4& mat_);

	/// <summary>
	/// ビュー変換行列を生成
	/// </summary>
	/// <param name="eye"></param>
	/// <param name="target"></param>
	/// <param name="up"></param>
	void MakeLookL(const Vector3& eye_, const Vector3& target_, const Vector3& up_, Matrix4& mat_);

	/// <summary>
	/// ビューポート行列を生成
	/// </summary>
	/// <param name="width_">横幅</param>
	/// <param name="height_">縦幅</param>
	Matrix4 MakeViewport(float width_, float height_);


	Matrix4 CreateLookToMatrix(const Vector3& pos_, const Vector3& direction_, const Vector3& up_);
}

