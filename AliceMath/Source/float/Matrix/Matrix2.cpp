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

	Matrix2::Matrix2(float m00, float m01,  float m10, float m11)
	{
		m[0][0] = m00;
		m[0][1] = m01;

		m[1][0] = m10;
		m[1][1] = m11;
	}

	Matrix2 MakeMat2Identity()
	{
		Matrix2 mat;
		return mat;
	}

	Matrix2 MakeInverse(const Matrix2* mat)
	{
		assert(mat);

		//掃き出し法を行う行列
		float sweep[2][4]{};
		//定数倍用
		float constTimes = 0.0f;
		//許容する誤差
		float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix2 retMat;

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				sweep[i][j] = mat->m[i][j];

				//sweepの右側に単位行列をセット
				sweep[i][2 + j] = MakeMat3Identity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (size_t i = 0; i < 2; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float max = std::fabs(sweep[i][i]);
			size_t maxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (size_t j = i + 1; j < 2; j++)
			{
				if (std::fabs(sweep[j][i]) > max)
				{
					max = std::fabs(sweep[j][i]);
					maxIndex = j;
				}
			}

			if (fabs(sweep[maxIndex][i]) <= MAX_ERR)
			{
				//逆行列は求められない
				return MakeMat2Identity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != maxIndex)
			{
				for (size_t j = 0; j < 4; j++)
				{
					float tmp = sweep[maxIndex][j];
					sweep[maxIndex][j] = sweep[i][j];
					sweep[i][j] = tmp;
				}
			}

			//sweep[i][i]に掛けると1になる値を求める
			constTimes = 1 / sweep[i][i];

			//操作(2):p行目をa倍する
			for (size_t j = 0; j < 4; j++)
			{
				//これによりsweep[i][i]が1になる
				sweep[i][j] *= constTimes;
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
				constTimes = -sweep[j][i];

				for (size_t k = 0; k < 4; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりsweep[j][i]が0になる
					sweep[j][k] += sweep[i][k] * constTimes;
				}
			}
		}

		//sweepの右半分がmatの逆行列
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				retMat.m[i][j] = sweep[i][2 + j];
			}
		}

		return retMat;
	}

	Matrix2 Matrix2::Transpose()
	{
		Matrix2 tmp(*this);
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				float f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	Matrix2& Matrix2::operator=(const Matrix2& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	const Matrix2& Matrix2::operator=(Matrix2& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	Matrix2& Matrix2::operator+=(const Matrix2& mat)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix2& Matrix2::operator-=(const Matrix2& mat)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix2& Matrix2::operator*=(const Matrix2& mat)
	{
		Matrix2 temp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				double f = 0.0;
				for (size_t k = 0; k < 2; k++)
				{
					f += (double)temp.m[i][k] * (double)mat.m[k][j];

					m[i][j] = (float)f;
				}


			}
		}
		return *this;
	}

	Matrix2 Matrix2::operator+(const Matrix2& mat) const
	{
		Matrix2 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix2 Matrix2::operator-(const Matrix2& mat) const
	{
		Matrix2 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix2 Matrix2::operator*(const Matrix2& mat) const
	{
		Matrix2 temp(*this);
		temp *= mat;
		return temp;
	}
}