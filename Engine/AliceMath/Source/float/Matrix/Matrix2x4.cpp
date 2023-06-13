#include "Matrix2x4.h"

namespace AliceMathF
{
	Matrix2x4::Matrix2x4()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;
		m[0][2] = 0.0f;
		m[0][3] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;
		m[1][2] = 0.0f;
		m[1][3] = 0.0f;

	}

	Matrix2x4::Matrix2x4(float m00_, float m01_, float m02_, float m03_, float m10_, float m11_, float m12_, float m13_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;
		m[0][3] = m03_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;
		m[1][3] = m13_;
	}

	Matrix2x4& Matrix2x4::operator=(const Matrix2x4& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	const Matrix2x4& Matrix2x4::operator=(Matrix2x4& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	Matrix2x4& Matrix2x4::operator+=(const Matrix2x4& mat_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix2x4& Matrix2x4::operator-=(const Matrix2x4 mat_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix2x4& Matrix2x4::operator*=(const Matrix2x4& mat_)
	{
		Matrix2x4 lTmp(*this);

		for (size_t i = 0; i < 2; i++)
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

	Matrix2x4 Matrix2x4::operator+(const Matrix2x4& mat_) const
	{
		Matrix2x4 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix2x4 Matrix2x4::operator-(const Matrix2x4& mat_) const
	{
		Matrix2x4 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix2x4 Matrix2x4::operator*(const Matrix2x4& mat_) const
	{
		Matrix2x4 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}

	Matrix2x4 Matrix2x4::Transpose()
	{
		Matrix2x4 lTmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				float lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	Matrix2x4 MakeMatrix2x4Identity()
	{
		Matrix2x4 lTmp;
		return lTmp;
	}
}
