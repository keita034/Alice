#include "Matrix4x2.h"
namespace AliceMathF
{
	Matrix4x2::Matrix4x2()
	{
		m[0][0] = 1.0f;
		m[0][1] = 0.0f;

		m[1][0] = 0.0f;
		m[1][1] = 1.0f;

		m[2][0] = 0.0f;
		m[2][1] = 0.0f;

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
	}

	Matrix4x2::Matrix4x2(float m00, float m01, float m10, float m11, float m20, float m21, float m30, float m31)
	{
		m[0][0] = m00;
		m[0][1] = m01;

		m[1][0] = m10;
		m[1][1] = m11;

		m[2][0] = m20;
		m[2][1] = m21;

		m[3][0] = m30;
		m[3][1] = m31;
	}

	Matrix4x2& Matrix4x2::operator=(const Matrix4x2& mat)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = mat.m[i][j];
			}
		}

		return *this;
	}

	const Matrix4x2& Matrix4x2::operator=(Matrix4x2& mat)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x2& Matrix4x2::operator+=(const Matrix4x2& mat)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x2& Matrix4x2::operator-=(const Matrix4x2 mat)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x2& Matrix4x2::operator*=(const Matrix4x2& mat)
	{
		Matrix4x2 temp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4x2 Matrix4x2::operator+(const Matrix4x2& mat) const
	{
		Matrix4x2 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix4x2 Matrix4x2::operator-(const Matrix4x2& mat) const
	{
		Matrix4x2 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix4x2 Matrix4x2::operator*(const Matrix4x2& mat) const
	{
		Matrix4x2 temp(*this);
		temp *= mat;
		return temp;
	}

	Matrix4x2 Matrix4x2::Transpose()
	{
		Matrix4x2 tmp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4x2 MakeMatrix4x2Identity()
	{
		Matrix4x2 mat;
		return mat;
	}
}