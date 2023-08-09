#include "Matrix4x3.h"
namespace AliceMathF
{
	Matrix4x3::Matrix4x3()
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

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
	}

	Matrix4x3::Matrix4x3(float m00_, float m01_, float m02_, float m10_, float m11_, float m12_, float m20_, float m21_, float m22_, float m30_, float m31_, float m32_)
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

		m[3][0] = m30_;
		m[3][1] = m31_;
		m[3][2] = m32_;
	}

	Matrix4x3::Matrix4x3(const DirectX::XMFLOAT4X3& matrix4X3_)
	{
		m[0][0] = matrix4X3_.m[0][0];
		m[0][1] = matrix4X3_.m[0][1];
		m[0][2] = matrix4X3_.m[0][2];

		m[1][0] = matrix4X3_.m[1][0];
		m[1][1] = matrix4X3_.m[1][1];
		m[1][2] = matrix4X3_.m[1][2];

		m[2][0] = matrix4X3_.m[2][0];
		m[2][1] = matrix4X3_.m[2][1];
		m[2][2] = matrix4X3_.m[2][2];

		m[3][0] = matrix4X3_.m[3][0];
		m[3][1] = matrix4X3_.m[3][1];
		m[3][2] = matrix4X3_.m[3][2];
	}



	Matrix4x3::operator DirectX::XMFLOAT4X3() const
	{
		DirectX::XMFLOAT4X3 lMat{
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2] };

		return lMat;
	}

	Matrix4x3& Matrix4x3::operator=(const Matrix4x3& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	const Matrix4x3& Matrix4x3::operator=(Matrix4x3& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x3& Matrix4x3::operator+=(const Matrix4x3& mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] += mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x3& Matrix4x3::operator-=(const Matrix4x3 mat_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] -= mat_.m[i][j];
			}
		}

		return *this;
	}

	Matrix4x3& Matrix4x3::operator*=(const Matrix4x3& mat_)
	{
		Matrix4x3 lTmp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4x3 Matrix4x3::operator+(const Matrix4x3& mat_) const
	{
		Matrix4x3 lTmp(*this);
		lTmp += mat_;
		return lTmp;
	}

	Matrix4x3 Matrix4x3::operator-(const Matrix4x3& mat_) const
	{
		Matrix4x3 lTmp(*this);
		lTmp -= mat_;
		return lTmp;
	}

	Matrix4x3 Matrix4x3::operator*(const Matrix4x3& mat_) const
	{
		Matrix4x3 lTmp(*this);
		lTmp *= mat_;
		return lTmp;
	}

	Matrix4x3 Matrix4x3::Transpose()
	{
		Matrix4x3 lTmp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4x3::Matrix4x3(const Matrix4x3&) = default;

	Matrix4x3 MakeMatrix4x3Identity()
	{
		Matrix4x3 lTmp;
		return lTmp;
	}
}