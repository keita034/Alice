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

	Matrix2x4::Matrix2x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;
	}

	Matrix2x4& Matrix2x4::operator=(const Matrix2x4& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	const Matrix2x4& Matrix2x4::operator=(Matrix2x4& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	Matrix2x4& Matrix2x4::operator+=(const Matrix2x4& mat)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix2x4& Matrix2x4::operator-=(const Matrix2x4 mat)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix2x4& Matrix2x4::operator*=(const Matrix2x4& mat)
	{
		Matrix2x4 temp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				double f = 0.0;
				for (size_t k = 0; k < 4; k++)
				{
					f += static_cast<double>(temp.m[i][k]) * static_cast<double>(mat.m[k][j]);

					m[i][j] = static_cast<float>(f);
				}


			}
		}
		return *this;
	}

	Matrix2x4 Matrix2x4::operator+(const Matrix2x4& mat) const
	{
		Matrix2x4 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix2x4 Matrix2x4::operator-(const Matrix2x4& mat) const
	{
		Matrix2x4 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix2x4 Matrix2x4::operator*(const Matrix2x4& mat) const
	{
		Matrix2x4 temp(*this);
		temp *= mat;
		return temp;
	}

	Matrix2x4 Matrix2x4::Transpose()
	{
		Matrix2x4 tmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				float f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	Matrix2x4 MakeMatrix2x4Identity()
	{
		Matrix2x4 mat;
		return mat;
	}
}
