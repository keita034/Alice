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

	Matrix3x2::Matrix3x2(float m00, float m01, float m10, float m11, float m20, float m21)
	{
		m[0][0] = m00;
		m[0][1] = m01;

		m[1][0] = m10;
		m[1][1] = m11;

		m[2][0] = m20;
		m[2][1] = m21;
	}

	Matrix3x2& AliceMathF::Matrix3x2::operator=(const Matrix3x2& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}
	const Matrix3x2& Matrix3x2::operator=(Matrix3x2& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x2& Matrix3x2::operator+=(const Matrix3x2& mat)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x2& Matrix3x2::operator-=(const Matrix3x2 mat)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x2& Matrix3x2::operator*=(const Matrix3x2& mat)
	{
		Matrix3x2 temp(*this);

		for (size_t i = 0; i < 3; i++)
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

	Matrix3x2 Matrix3x2::operator+(const Matrix3x2& mat) const
	{
		Matrix3x2 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix3x2 Matrix3x2::operator-(const Matrix3x2& mat) const
	{
		Matrix3x2 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix3x2 Matrix3x2::operator*(const Matrix3x2& mat) const
	{
		Matrix3x2 temp(*this);
		temp *= mat;
		return temp;
	}

	Matrix3x2 Matrix3x2::Transpose()
	{
		Matrix3x2 tmp(*this);

		for (size_t i = 0; i < 3; i++)
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

	Matrix3x2 MakeMatrix3x2Identity()
	{
		Matrix3x2 mat;
		return mat;
	}
}
