#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)

#include<assimp/matrix4x4.h>

#pragma warning(pop)

#include "Matrix4.h"
#include "Matrix4.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	Matrix4::Matrix4()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = 1.0f;
		m[2][3] = 0.0f;

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	Matrix4::Matrix4(float m00_, float m01_, float m02_, float m03_, float m10_, float m11_, float m12_, float m13_, float m20_, float m21_, float m22_, float m23_, float m30_, float m31_, float m32_, float m33_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;
		m[0][3] = m03_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;
		m[1][3] = m13_;

		m[2][0] = m20_;
		m[2][1] = m21_;
		m[2][2] = m22_;
		m[2][3] = m23_;

		m[3][0] = m30_;
		m[3][1] = m31_;
		m[3][2] = m32_;
		m[3][3] = m33_;
	}


	Matrix4::Matrix4(const Vector3& scale_, Quaternion& rotat_, const Vector3& trans_)
	{
		AliceMathF::Matrix4 lMatScale, lMatRot, lMatTrans;

		//スケール、回転、平行移動行列の計算
		lMatScale.MakeScaling(scale_);
		lMatRot = rotat_.Rotate();
		lMatTrans.MakeTranslation(trans_);

		//ワールド行列の合成
		//変形をリセット
		*this = AliceMathF::MakeIdentity();
		//ワールド行列にスケーリングを反映
		*this *= lMatScale;
		//ワールド行列に回転を反映
		*this *= lMatRot;
		//ワールド行列に平行移動を反映
		*this *= lMatTrans;
	}
	
	Matrix4::Matrix4(const aiMatrix4x4& mat_)
	{
		m[0][0] = mat_.a1;
		m[0][1] = mat_.a2;
		m[0][2] = mat_.a3;
		m[0][3] = mat_.a4;

		m[1][0] = mat_.b1;
		m[1][1] = mat_.b2;
		m[1][2] = mat_.b3;
		m[1][3] = mat_.b4;

		m[2][0] = mat_.c1;
		m[2][1] = mat_.c2;
		m[2][2] = mat_.c3;
		m[2][3] = mat_.c4;

		m[3][0] = mat_.d1;
		m[3][1] = mat_.d2;
		m[3][2] = mat_.d3;
		m[3][3] = mat_.d4;
	}

	Matrix4 MakeIdentity()
	{
		Matrix4 mat;
		return mat;
	}

	Matrix4 MakeInverse(const Matrix4* mat_)
	{
		assert(mat_);

		//掃き出し法を行う行列
		float lSweep[4][8]{};
		//定数倍用
		float lConstTimes = 0.0f;
		//許容する誤差
		const float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix4 lRetMat;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				lSweep[i][j] = mat_->m[i][j];

				//lSweepの右側に単位行列をセット
				lSweep[i][4 + j] = MakeIdentity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (size_t i = 0; i < 4; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float lMax = std::fabs(lSweep[i][i]);
			size_t lMaxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (size_t j = i + 1; j < 4; j++)
			{
				if (std::fabs(lSweep[j][i]) > lMax)
				{
					lMax = std::fabs(lSweep[j][i]);
					lMaxIndex = j;
				}
			}

			if (fabs(lSweep[lMaxIndex][i]) <= MAX_ERR)
			{
				//逆行列は求められない
				return MakeIdentity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != lMaxIndex)
			{
				for (size_t j = 0; j < 8; j++)
				{
					float lTmp = lSweep[lMaxIndex][j];
					lSweep[lMaxIndex][j] = lSweep[i][j];
					lSweep[i][j] = lTmp;
				}
			}

			//lSweep[i][i]に掛けると1になる値を求める
			lConstTimes = 1 / lSweep[i][i];

			//操作(2):p行目をa倍する
			for (size_t j = 0; j < 8; j++)
			{
				//これによりlSweep[i][i]が1になる
				lSweep[i][j] *= lConstTimes;
			}

			//操作(3)によりi行目以外の行のi列目を0にする
			for (size_t j = 0; j < 4; j++)
			{
				if (j == i)
				{
					//i行目はそのまま
					continue;
				}

				//i行目に掛ける値を求める
				lConstTimes = -lSweep[j][i];

				for (size_t k = 0; k < 8; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりlSweep[j][i]が0になる
					lSweep[j][k] += lSweep[i][k] * lConstTimes;
				}
			}
		}

		//lSweepの右半分がmatの逆行列
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				lRetMat.m[i][j] = lSweep[i][4 + j];
			}
		}

		return lRetMat;
	}

	Matrix4::Matrix4(const DirectX::XMMATRIX& matrix_4_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = matrix_4_.r[i].m128_f32[j];
			}
		}
	}

	void Matrix4::MakeScaling(const Vector3& scale_)
	{
		//スケーリング倍率を行列に設定する
		m[0][0] = scale_.x;
		m[1][1] = scale_.y;
		m[2][2] = scale_.z;
	}

	void Matrix4::MakeScaling(float x_, float y_, float z_)
	{
		MakeScaling(Vector3(x_, y_, z_));
	}

	void Matrix4::MakeTranslation(const Vector3& trans_)
	{
		m[3][0] = trans_.x;
		m[3][1] = trans_.y;
		m[3][2] = trans_.z;
	}

	void Matrix4::MakeTranslation(float x_, float y_, float z_)
	{
		MakeTranslation(Vector3(x_, y_, z_));
	}

	void Matrix4::MakeRotationY(float angle_)
	{
		m[0][0] = std::cos(angle_);
		m[0][2] = -std::sin(angle_);

		m[2][0] = std::sin(angle_);
		m[2][2] = std::cos(angle_);

	}

	void Matrix4::MakeRotationZ(float angle_)
	{
		m[0][0] = std::cos(angle_);
		m[0][1] = std::sin(angle_);

		m[1][0] = -std::sin(angle_);
		m[1][1] = std::cos(angle_);
	}

	void Matrix4::MakeRotationX(float angle_)
	{
		m[1][1] = std::cos(angle_);
		m[1][2] = std::sin(angle_);

		m[2][1] = -std::sin(angle_);
		m[2][2] = std::cos(angle_);
	}

	void Matrix4::MakeRotation(const Vector3& rotation_)
	{
		Matrix4 lRotX;
		Matrix4 lRotY;
		Matrix4 lRotZ;

		lRotZ.MakeRotationZ(rotation_.z);
		lRotX.MakeRotationX(rotation_.x);
		lRotY.MakeRotationY(rotation_.y);

		*this = lRotZ * lRotX * lRotY;
	}

	void Matrix4::MakeRotation(float x_, float y_, float z_)
	{
		MakeRotation(Vector3(x_, y_, z_));
	}

	Matrix4::operator DirectX::XMMATRIX() const
	{
		DirectX::XMMATRIX lMat{
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3] };

		return lMat;
	}

	Matrix4 Matrix4::Transpose()
	{
		Matrix4 lTmp(*this);
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				float f = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = f;
			}
		}

		return lTmp;
	}

	Matrix4& Matrix4::operator=(const Matrix4& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	const Matrix4& Matrix4::operator=(Matrix4& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4& Matrix4::operator-=(const Matrix4& mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& mat_)
	{
		Matrix4 lTmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				double lF = 0.0;
				for (size_t k = 0; k < 4; k++)
				{
					lF += static_cast<double>(lTmp.m[i][k]) * static_cast<double>(mat_.m[k][j]);

					m[i][j] = static_cast<float>(lF);
				}

				
			}
		}
		return *this;
	}

	Matrix4 Matrix4::operator+(const Matrix4& mat_) const
	{
		Matrix4 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix4 Matrix4::operator-(const Matrix4& mat_) const
	{
		Matrix4 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix4 Matrix4::operator*(const Matrix4& mat_) const
	{
		Matrix4 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}

	void MakeOrthogonalLOffCenter(float left_, float right_, float bottom_, float top_, float near_, float far_, Matrix4& matrix_)
	{

		float lWidth = 1.0f / (right_ - left_);
		float lHeight = 1.0f / (top_ - bottom_);
		float lRange = 1.0f / (far_ - near_);

		matrix_.m[0][0] = lWidth * 2;

		matrix_.m[1][1] = lHeight * 2;

		matrix_.m[2][2] = lRange;

		matrix_.m[3][0] = -(left_ + right_) * lWidth;
		matrix_.m[3][1] = -(top_ + bottom_) * lHeight;
		matrix_.m[3][2] = lRange * -near_;
		matrix_.m[3][3] = 1.0f;

		matrix_.m[0][1] = matrix_.m[0][2] = matrix_.m[0][3] =
			matrix_.m[1][0] = matrix_.m[1][2] = matrix_.m[1][3] =
			matrix_.m[2][0] = matrix_.m[2][1] = matrix_.m[2][3] = 0.0f;
	}

	void MakeOrthogonalL(float width_, float height_, float near_, float far_, Matrix4& matrix_)
	{
		float lFRange = 1.0f / (far_ - near_);

		matrix_.m[0][0] = 2.0f / width_;

		matrix_.m[1][1] = 2.0f / height_;

		matrix_.m[2][2] = lFRange;

		matrix_.m[3][2] = -lFRange * near_;
		matrix_.m[3][3] = 1.0f;

		matrix_.m[0][1] = matrix_.m[0][2] = matrix_.m[0][3] =
			matrix_.m[1][0] = matrix_.m[1][2] = matrix_.m[1][3] =
			matrix_.m[2][0] = matrix_.m[2][1] = matrix_.m[2][3] =
			matrix_.m[3][0] = matrix_.m[3][1] = 0.0f;;

	}

	void MakeOrthogonalR(float left_, float right_, float bottom_, float top_, float near_, float far_, Matrix4& matrix_)
	{
		float lWidth = 1.0f / (right_ - left_);
		float lHeight = 1.0f / (top_ - bottom_);
		float lRange = 1.0f / (far_ - near_);

		matrix_.m[0][0] = lWidth * 2;

		matrix_.m[1][1] = lHeight * 2;

		matrix_.m[2][2] = lRange;

		matrix_.m[3][0] = -(left_ + right_) * lWidth;
		matrix_.m[3][1] = -(top_ + bottom_) * lHeight;
		matrix_.m[3][2] = lRange * -near_;
		matrix_.m[3][3] = 1.0f;

		matrix_.m[0][1] = matrix_.m[0][2] = matrix_.m[0][3] =
			matrix_.m[1][0] = matrix_.m[1][2] = matrix_.m[1][3] =
			matrix_.m[2][0] = matrix_.m[2][1] = matrix_.m[2][3] = 0.0f;

	}

	void MakePerspectiveL(float fovAngleY_, float aspect_, float near_, float far_, Matrix4& matrix_)
	{

		float lSinFov = 0.0f;
		float lCosFov = 0.0f;
		SinCos(lSinFov, lCosFov, 0.5f * fovAngleY_);

		float lRange = far_ / (far_ - near_);
		float lHeight = lCosFov / lSinFov;

		matrix_.m[0][0] = lHeight / aspect_;

		matrix_.m[1][1] = lCosFov / lSinFov;

		matrix_.m[2][2] = lRange;
		matrix_.m[2][3] = 1.0f;

		matrix_.m[3][2] = -lRange * near_;

		matrix_.m[0][1] = matrix_.m[0][2] = matrix_.m[0][3] =
			matrix_.m[1][0] = matrix_.m[1][2] = matrix_.m[1][3] =
			matrix_.m[2][0] = matrix_.m[2][1] =
			matrix_.m[3][0] = matrix_.m[3][1] = matrix_.m[3][3] = 0.0f;
	}

	void MakePerspectiveR(float fovAngleY_, float aspect_, float near_, float far_, Matrix4& matrix_)
	{
		float lSinFov = 0.0f;
		float lCosFov = 0.0f;
		SinCos(lSinFov, lCosFov, 0.5f * fovAngleY_);

		float lRange = far_ / (far_ - near_);
		float lHeight = lCosFov / lSinFov;

		matrix_.m[0][0] = lHeight / aspect_;

		matrix_.m[1][1] = lCosFov / lSinFov;

		matrix_.m[2][2] = lRange;
		matrix_.m[2][3] = 1.0f;

		matrix_.m[3][2] = -lRange * near_;

		matrix_.m[0][1] = matrix_.m[0][2] = matrix_.m[0][3] =
			matrix_.m[1][0] = matrix_.m[1][2] = matrix_.m[1][3] =
			matrix_.m[2][0] = matrix_.m[2][1] =
			matrix_.m[3][0] = matrix_.m[3][1] = matrix_.m[3][3] = 0.0f;


	}

	void CoordinateTransformation3D(float& x_, float& y_, float& z_, Matrix4& mat_)
	{
		Vector3 lVec(x_, y_, z_);

		x_ = lVec.x * mat_.m[0][0] + lVec.y * mat_.m[1][0] + lVec.z * mat_.m[2][0];

		y_ = lVec.x * mat_.m[0][1] + lVec.y * mat_.m[1][1] + lVec.z * mat_.m[2][1];

		z_ = lVec.x * mat_.m[0][2] + lVec.y * mat_.m[1][2] + lVec.z * mat_.m[2][2];
	}

	void MakeLookL(const Vector3& eye_, const Vector3& target_, const Vector3& up_, Matrix4& mat_)
	{
		Vector3 lZVec = target_ - eye_;
		lZVec.Normal();

		Vector3 lXVec = up_.Cross(lZVec);
		lXVec.Normal();

		Vector3 lYVec = lZVec.Cross(lXVec);
		lYVec.Normal();

		mat_.m[0][0] = lXVec.x;
		mat_.m[0][1] = lXVec.y;
		mat_.m[0][2] = lXVec.z;
		mat_.m[1][0] = lYVec.x;
		mat_.m[1][1] = lYVec.y;
		mat_.m[1][2] = lYVec.z;
		mat_.m[2][0] = lZVec.x;
		mat_.m[2][1] = lZVec.y;
		mat_.m[2][2] = lZVec.z;
		mat_.m[3][0] = eye_.x;
		mat_.m[3][1] = eye_.y;
		mat_.m[3][2] = eye_.z;
	}

	Matrix4 AliceMathF::MakeViewport(float width_, float height_)
	{
		float lWinH = height_ / 2.0f;
		float lWinW = width_ / 2.0f;

		return { lWinW, 0, 0, 0, 0, -lWinH, 0, 0, 0, 0, 1, 0, lWinW, lWinH, 0, 1 };
	}
	Matrix4 CreateLookToMatrix(const Vector3& pos_, const Vector3& direction_, const Vector3& up_)
	{
		Vector3 z;
		z = direction_.Normal();
		Vector3 x = up_.Cross(z);
		x = x.Normal();
		Vector3 y = z.Cross(x);

		Vector3 p(
			x.Dot(-pos_),
			y.Dot(-pos_),
			z.Dot(-pos_)
		);

		return Matrix4(
			x.x, y.x, z.x, 0,
			x.y, y.y, z.y, 0,
			x.z, y.z, z.z, 0,
			p.x, p.y, p.z, 1
		);
	}
}