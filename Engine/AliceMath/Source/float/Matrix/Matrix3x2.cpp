#include "Matrix3x2.h"

namespace AliceMathF
{
	Matrix3x2::Matrix3x2()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;
	}

	Matrix3x2::Matrix3x2(float m00_, float m01_, float m10_, float m11_, float m20_, float m21_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;

		m[1][0] = m10_;
		m[1][1] = m11_;

		m[2][0] = m20_;
		m[2][1] = m21_;
	}

	Matrix3x2& AliceMathF::Matrix3x2::operator=(const Matrix3x2& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}
	const Matrix3x2& Matrix3x2::operator=(Matrix3x2& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x2& Matrix3x2::operator+=(const Matrix3x2& mat_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x2& Matrix3x2::operator-=(const Matrix3x2 mat_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x2& Matrix3x2::operator*=(const Matrix3x2& mat_)
	{
		Matrix3x2 lTmp(*this);

		for (size_t i = 0; i < 3; i++)
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

	Matrix3x2 Matrix3x2::operator+(const Matrix3x2& mat_) const
	{
		Matrix3x2 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix3x2 Matrix3x2::operator-(const Matrix3x2& mat_) const
	{
		Matrix3x2 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix3x2 Matrix3x2::operator*(const Matrix3x2& mat_) const
	{
		Matrix3x2 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}

	Matrix3x2 Matrix3x2::Transpose()
	{
		Matrix3x2 lTmp(*this);

		for (size_t i = 0; i < 3; i++)
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

	Matrix3x2 MakeMatrix3x2Identity()
	{
		Matrix3x2 lTmp;
		return lTmp;
	}
}
