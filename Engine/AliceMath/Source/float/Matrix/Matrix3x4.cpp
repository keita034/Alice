#include "Matrix3x4.h"

namespace AliceMathF
{
	Matrix3x4::Matrix3x4()
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
	}

	Matrix3x4::Matrix3x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;

	}

	Matrix3x4::Matrix3x4(const DirectX::XMFLOAT3X4& XMFLOAT3X4)
	{
		m[0][0] = XMFLOAT3X4.m[0][0];
		m[0][1] = XMFLOAT3X4.m[0][1];
		m[0][2] = XMFLOAT3X4.m[0][2];
		m[0][3] = XMFLOAT3X4.m[0][3];

		m[1][0] = XMFLOAT3X4.m[1][0];
		m[1][1] = XMFLOAT3X4.m[1][1];
		m[1][2] = XMFLOAT3X4.m[1][2];
		m[1][3] = XMFLOAT3X4.m[1][3];

		m[2][0] = XMFLOAT3X4.m[2][0];
		m[2][1] = XMFLOAT3X4.m[2][1];
		m[2][2] = XMFLOAT3X4.m[2][2];
		m[2][3] = XMFLOAT3X4.m[2][3];
	}

	Matrix3x4::operator DirectX::XMFLOAT3X4() const
	{
		DirectX::XMFLOAT3X4 mat{
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3]};

		return mat;
	}

	Matrix3x4& Matrix3x4::operator=(const Matrix3x4& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	const Matrix3x4& Matrix3x4::operator=(Matrix3x4& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x4& Matrix3x4::operator+=(const Matrix3x4& mat)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x4& Matrix3x4::operator-=(const Matrix3x4 mat)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix3x4& Matrix3x4::operator*=(const Matrix3x4& mat)
	{
		Matrix3x4 temp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				double f = 0.0;
				for (size_t k = 0; k < 4; k++)
				{
					f += (double)temp.m[i][k] * (double)mat.m[k][j];

					m[i][j] = (float)f;
				}


			}
		}
		return *this;
	}

	Matrix3x4 Matrix3x4::operator+(const Matrix3x4& mat) const
	{
		Matrix3x4 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix3x4 Matrix3x4::operator-(const Matrix3x4& mat) const
	{
		Matrix3x4 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix3x4 Matrix3x4::operator*(const Matrix3x4& mat) const
	{
		Matrix3x4 temp(*this);
		temp *= mat;
		return temp;
	}

	Matrix3x4 Matrix3x4::Transpose()
	{
		Matrix3x4 tmp(*this);

		for (size_t i = 0; i < 3; i++)
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

	Matrix3x4 MakeMatrix3x4Identity()
	{
		Matrix3x4 mat;
		return mat;
	}
}
