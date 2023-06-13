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

	Matrix4x2::Matrix4x2(float m00_, float m01_, float m10_, float m11_, float m20_, float m21_, float m30_, float m31_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;

		m[1][0] = m10_;
		m[1][1] = m11_;

		m[2][0] = m20_;
		m[2][1] = m21_;

		m[3][0] = m30_;
		m[3][1] = m31_;
	}

	Matrix4x2& Matrix4x2::operator=(const Matrix4x2& mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = mat_.m[i][j];
			}
		}

		return *this;
	}

	const Matrix4x2& Matrix4x2::operator=(Matrix4x2& mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x2& Matrix4x2::operator+=(const Matrix4x2& mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x2& Matrix4x2::operator-=(const Matrix4x2 mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x2& Matrix4x2::operator*=(const Matrix4x2& mat_)
	{
		Matrix4x2 lTmp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4x2 Matrix4x2::operator+(const Matrix4x2& mat_) const
	{
		Matrix4x2 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix4x2 Matrix4x2::operator-(const Matrix4x2& mat_) const
	{
		Matrix4x2 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix4x2 Matrix4x2::operator*(const Matrix4x2& mat_) const
	{
		Matrix4x2 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}

	Matrix4x2 Matrix4x2::Transpose()
	{
		Matrix4x2 lTmp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4x2 MakeMatrix4x2Identity()
	{
		Matrix4x2 lTmp;
		return lTmp;
	}
}