#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cmath>
#include<assimp/matrix3x3.h>


#pragma warning(pop)

#include "Matrix3.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	Matrix3::Matrix3()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
		m[1][2] = 0.0f;

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
		m[2][2] = 1.0f;
	}

	Matrix3::Matrix3(float m00_, float m01_, float m02_, float m10_, float m11_, float m12_, float m20_, float m21_, float m22_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;

		m[2][0] = m20_;
		m[2][1] = m21_;
		m[2][2] = m22_;
	}


	Matrix3::Matrix3(const Vector2& scale_, float angle_, const Vector2& trans_)
	{
		AliceMathF::Matrix3 lMatScale, lMatRot, lMatTrans;

		//スケール、回転、平行移動行列の計算
		lMatScale.MakeScaling(scale_);
		lMatRot.MakeRotation(angle_);
		lMatTrans.MakeTranslation(trans_);

		//ワールド行列の合成
		//変形をリセット
		*this = AliceMathF::MakeMat3Identity();
		//ワールド行列にスケーリングを反映
		*this *= lMatScale;
		//ワールド行列に回転を反映
		*this *= lMatRot;
		//ワールド行列に平行移動を反映
		*this *= lMatTrans;
	}

	Matrix3::Matrix3(const aiMatrix3x3& mat_)
	{
		m[0][0] = mat_.a1;
		m[0][1] = mat_.a2;
		m[0][2] = mat_.a3;

		m[1][0] = mat_.b1;
		m[1][1] = mat_.b2;
		m[1][2] = mat_.b3;

		m[2][0] = mat_.c1;
		m[2][1] = mat_.c2;
		m[2][2] = mat_.c3;
	}

	Matrix3 MakeMat3Identity()
	{
		Matrix3 lMat;
		return lMat;
	}

	Matrix3 MakeInverse(const Matrix3* mat_)
	{
		assert(mat_);

		//掃き出し法を行う行列
		float lSweep[3][6]{};
		//定数倍用
		 float lConstTimes = 0.0f;
		//許容する誤差
		const float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix3 lRetMat;

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				lSweep[i][j] = mat_->m[i][j];

				//lSweepの右側に単位行列をセット
				lSweep[i][3 + j] = MakeMat3Identity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (size_t i = 0; i < 3; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float lMax = std::fabs(lSweep[i][i]);
			size_t lMaxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (size_t j = i + 1; j < 3; j++)
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
				return MakeMat3Identity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != lMaxIndex)
			{
				for (size_t j = 0; j < 6; j++)
				{
					float lTmp = lSweep[lMaxIndex][j];
					lSweep[lMaxIndex][j] = lSweep[i][j];
					lSweep[i][j] = lTmp;
				}
			}

			//lSweep[i][i]に掛けると1になる値を求める
			lConstTimes = 1 / lSweep[i][i];

			//操作(2):p行目をa倍する
			for (size_t j = 0; j < 6; j++)
			{
				//これによりlSweep[i][i]が1になる
				lSweep[i][j] *= lConstTimes;
			}

			//操作(3)によりi行目以外の行のi列目を0にする
			for (size_t j = 0; j < 6; j++)
			{
				if (j == i)
				{
					//i行目はそのまま
					continue;
				}

				//i行目に掛ける値を求める
				lConstTimes = -lSweep[j][i];

				for (size_t k = 0; k < 6; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりlSweep[j][i]が0になる
					lSweep[j][k] += lSweep[i][k] * lConstTimes;
				}
			}
		}

		//lSweepの右半分がmatの逆行列
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				lRetMat.m[i][j] = lSweep[i][3 + j];
			}
		}

		return lRetMat;
	}

	Matrix3::Matrix3(const DirectX::XMFLOAT3X3& matrix3_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = matrix3_.m[i][j];
			}
		}
	}

	void Matrix3::MakeScaling(const Vector2& scale_)
	{
		//スケーリング倍率を行列に設定する
		m[0][0] = scale_.x;
		m[1][1] = scale_.y;
	}

	void Matrix3::MakeScaling(float x_, float y_)
	{
		MakeScaling(Vector2(x_, y_));
	}

	void Matrix3::MakeTranslation(const Vector2& trans_)
	{
		m[3][0] = trans_.x;
		m[3][1] = trans_.y;
	}

	void Matrix3::MakeTranslation(float x_, float y_)
	{
		MakeTranslation(Vector2(x_, y_));
	}

	void Matrix3::MakeRotation(float angle_)
	{
		m[0][0] = Cos(angle_);
		m[0][1] = -Sin(angle_);

		m[1][0] = Sin(angle_);
		m[1][1] = Cos(angle_);
	}

	Matrix3::operator DirectX::XMFLOAT3X3() const
	{
		DirectX::XMFLOAT3X3 lMat {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2]};

		return lMat;
	}

	Matrix3 Matrix3::Transpose()
	{
		Matrix3 lTmp(*this);
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				float lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	Matrix3& Matrix3::operator=(const Matrix3& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	const Matrix3& Matrix3::operator=(Matrix3& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& mat_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix3& Matrix3::operator-=(const Matrix3& mat_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix3& Matrix3::operator*=(const Matrix3& mat_)
	{
		Matrix3 lTmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				double lF = 0.0;
				for (size_t k = 0; k < 3; k++)
				{
					lF += static_cast<double>(lTmp.m[i][k]) * static_cast<double>(mat_.m[k][j]);

					m[i][j] = static_cast<float>(lF);
				}


			}
		}
		return *this;
	}

	Matrix3 Matrix3::operator+(const Matrix3& mat_) const
	{
		Matrix3 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix3 Matrix3::operator-(const Matrix3& mat_) const
	{
		Matrix3 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix3 Matrix3::operator*(const Matrix3& mat_) const
	{
		Matrix3 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}
}