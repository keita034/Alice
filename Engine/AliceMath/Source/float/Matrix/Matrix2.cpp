#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cmath>

#pragma warning(pop)

#include "Matrix2.h"
#include "AliceMathF.h"

namespace AliceMathF
{
	Matrix2::Matrix2()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
	}

	Matrix2::Matrix2(float m00_, float m01_,  float m10_, float m11_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;

		m[1][0] = m10_;
		m[1][1] = m11_;
	}

	Matrix2 MakeMat2Identity()
	{
		Matrix2 lTmp;
		return lTmp;
	}

	Matrix2 MakeInverse(const Matrix2* mat_)
	{
		assert(mat_);

		//掃き出し法を行う行列
		float lSweep[2][4]{};
		//定数倍用
		float lConstTimes = 0.0f;
		//許容する誤差
		const float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix2 lRetMat;

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				lSweep[i][j] = mat_->m[i][j];

				//lSweepの右側に単位行列をセット
				lSweep[i][2 + j] = MakeMat3Identity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (size_t i = 0; i < 2; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float lMax = std::fabs(lSweep[i][i]);
			size_t lMaxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (size_t j = i + 1; j < 2; j++)
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
				return MakeMat2Identity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != lMaxIndex)
			{
				for (size_t j = 0; j < 4; j++)
				{
					float lTmp = lSweep[lMaxIndex][j];
					lSweep[lMaxIndex][j] = lSweep[i][j];
					lSweep[i][j] = lTmp;
				}
			}

			//lSweep[i][i]に掛けると1になる値を求める
			lConstTimes = 1 / lSweep[i][i];

			//操作(2):p行目をa倍する
			for (size_t j = 0; j < 4; j++)
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

				for (size_t k = 0; k < 4; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりlSweep[j][i]が0になる
					lSweep[j][k] += lSweep[i][k] * lConstTimes;
				}
			}
		}

		//lSweepの右半分がmatの逆行列
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				lRetMat.m[i][j] = lSweep[i][2 + j];
			}
		}

		return lRetMat;
	}

	Matrix2 Matrix2::Transpose()
	{
		Matrix2 lTmp(*this);
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				float lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	Matrix2::Matrix2(const Matrix2&) = default;

	Matrix2& Matrix2::operator=(const Matrix2& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	const Matrix2& Matrix2::operator=(Matrix2& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	Matrix2& Matrix2::operator+=(const Matrix2& mat_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix2& Matrix2::operator-=(const Matrix2& mat_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix2& Matrix2::operator*=(const Matrix2& mat_)
	{
		Matrix2 lTmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				double lF = 0.0;
				for (size_t k = 0; k < 2; k++)
				{
					lF += static_cast<double>(lTmp.m[i][k]) * static_cast<double>(mat_.m[k][j]);

					m[i][j] = static_cast<float>(lF);
				}


			}
		}
		return *this;
	}

	Matrix2 Matrix2::operator+(const Matrix2& mat_) const
	{
		Matrix2 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix2 Matrix2::operator-(const Matrix2& mat_) const
	{
		Matrix2 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix2 Matrix2::operator*(const Matrix2& mat_) const
	{
		Matrix2 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}
}