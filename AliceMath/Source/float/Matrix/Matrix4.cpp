#pragma once
#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)

#include<assimp/matrix4x4.h>

#pragma warning(pop)

#include "Matrix4.h"
#include "Matrix4.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	Matrix4::Matrix4()
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

		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
		m[3][3] = 1.0f;
	}

	Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
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

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}


	Matrix4::Matrix4(const Vector3& scale, Quaternion& rotat, const Vector3& trans)
	{
		AliceMathF::Matrix4 matScale, matRot, matTrans;

		//スケール、回転、平行移動行列の計算
		matScale.MakeScaling(scale);
		matRot = rotat.Rotate();
		matTrans.MakeTranslation(trans);

		//ワールド行列の合成
		//変形をリセット
		*this = AliceMathF::MakeIdentity();
		//ワールド行列にスケーリングを反映
		*this *= matScale;
		//ワールド行列に回転を反映
		*this *= matRot;
		//ワールド行列に平行移動を反映
		*this *= matTrans;
	}
	
	Matrix4::Matrix4(const aiMatrix4x4& mat)
	{
		m[0][0] = mat.a1;
		m[0][1] = mat.a2;
		m[0][2] = mat.a3;
		m[0][3] = mat.a4;

		m[1][0] = mat.b1;
		m[1][1] = mat.b2;
		m[1][2] = mat.b3;
		m[1][3] = mat.b4;

		m[2][0] = mat.c1;
		m[2][1] = mat.c2;
		m[2][2] = mat.c3;
		m[2][3] = mat.c4;

		m[3][0] = mat.d1;
		m[3][1] = mat.d2;
		m[3][2] = mat.d3;
		m[3][3] = mat.d4;
	}

	Matrix4 MakeIdentity()
	{
		Matrix4 mat;
		return mat;
	}

	Matrix4 MakeInverse(const Matrix4* mat)
	{
		assert(mat);

		//掃き出し法を行う行列
		float sweep[4][8]{};
		//定数倍用
		float constTimes = 0.0f;
		//許容する誤差
		float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix4 retMat;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				sweep[i][j] = mat->m[i][j];

				//sweepの右側に単位行列をセット
				sweep[i][4 + j] = MakeIdentity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (size_t i = 0; i < 4; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float max = std::fabs(sweep[i][i]);
			size_t maxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (size_t j = i + 1; j < 4; j++)
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
				return MakeIdentity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != maxIndex)
			{
				for (size_t j = 0; j < 8; j++)
				{
					float tmp = sweep[maxIndex][j];
					sweep[maxIndex][j] = sweep[i][j];
					sweep[i][j] = tmp;
				}
			}

			//sweep[i][i]に掛けると1になる値を求める
			constTimes = 1 / sweep[i][i];

			//操作(2):p行目をa倍する
			for (size_t j = 0; j < 8; j++)
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

				for (size_t k = 0; k < 8; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりsweep[j][i]が0になる
					sweep[j][k] += sweep[i][k] * constTimes;
				}
			}
		}

		//sweepの右半分がmatの逆行列
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				retMat.m[i][j] = sweep[i][4 + j];
			}
		}

		return retMat;
	}

	Matrix4::Matrix4(const DirectX::XMMATRIX& Matrix4)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = Matrix4.r[i].m128_f32[j];
			}
		}
	}

	void Matrix4::MakeScaling(const Vector3& scale)
	{
		//スケーリング倍率を行列に設定する
		m[0][0] = scale.x;
		m[1][1] = scale.y;
		m[2][2] = scale.z;
	}

	void Matrix4::MakeScaling(float x, float y, float z)
	{
		MakeScaling(Vector3(x, y, z));
	}

	void Matrix4::MakeTranslation(const Vector3& trans)
	{
		m[3][0] = trans.x;
		m[3][1] = trans.y;
		m[3][2] = trans.z;
	}

	void Matrix4::MakeTranslation(float x, float y, float z)
	{
		MakeTranslation(Vector3(x, y, z));
	}

	void Matrix4::MakeRotationY(float angle)
	{
		m[0][0] = std::cos(angle);
		m[0][2] = -std::sin(angle);

		m[2][0] = std::sin(angle);
		m[2][2] = std::cos(angle);

	}

	void Matrix4::MakeRotationZ(float angle)
	{
		m[0][0] = std::cos(angle);
		m[0][1] = std::sin(angle);

		m[1][0] = -std::sin(angle);
		m[1][1] = std::cos(angle);
	}

	void Matrix4::MakeRotationX(float angle)
	{
		m[1][1] = std::cos(angle);
		m[1][2] = std::sin(angle);

		m[2][1] = -std::sin(angle);
		m[2][2] = std::cos(angle);
	}

	void Matrix4::MakeRotation(const Vector3& Rotat)
	{
		Matrix4 RotX;
		Matrix4 RotY;
		Matrix4 RotZ;

		RotZ.MakeRotationZ(Rotat.z);
		RotX.MakeRotationX(Rotat.x);
		RotY.MakeRotationY(Rotat.y);

		*this = RotZ * RotX * RotY;
	}

	void Matrix4::MakeRotation(float x, float y, float z)
	{
		MakeRotation(Vector3(x, y, z));
	}

	Matrix4::operator DirectX::XMMATRIX() const
	{
		DirectX::XMMATRIX mat{
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3] };

		return mat;
	}

	Matrix4 Matrix4::Transpose()
	{
		Matrix4 tmp(*this);
		for (size_t i = 0; i < 4; i++)
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

	Matrix4 Matrix4::MatrixMultiply(const Matrix4& mat)
	{
		Matrix4 tmp;

		Vector4 vX;
		Vector4 vY;
		Vector4 vZ;
		Vector4 vW;

		Vector4 matR0;
		Vector4 matR1;
		Vector4 matR2;
		Vector4 matR3;

		for (size_t i = 0; i < 4; i++)
		{
			vX = { m[i][0],m[i][0],m[i][0],m[i][0] };
			vY = { m[i][1],m[i][1],m[i][1],m[i][1] };
			vZ = { m[i][2],m[i][2],m[i][2],m[i][2] };
			vW = { m[i][3],m[i][3],m[i][3],m[i][3] };

			matR0 = { mat.m[0][0],mat.m[0][1],mat.m[0][2],mat.m[0][3] };
			matR1 = { mat.m[1][0],mat.m[1][1],mat.m[1][2],mat.m[1][3] };
			matR2 = { mat.m[2][0],mat.m[2][1],mat.m[2][2],mat.m[2][3] };
			matR3 = { mat.m[3][0],mat.m[3][1],mat.m[3][2],mat.m[3][3] };

			vX = Vec4MulPs(vX, matR0);
			vY = Vec4MulPs(vY, matR1);
			vZ = Vec4MulPs(vZ, matR2);
			vW = Vec4MulPs(vW, matR3);

			vX = Vec4AddPs(vX, vZ);
			vY = Vec4AddPs(vY, vW);
			vX = Vec4AddPs(vX, vY);

			tmp.m[i][0] = vX.x;
			tmp.m[i][1] = vX.y;
			tmp.m[i][2] = vX.z;
			tmp.m[i][3] = vX.w;
		}

		return tmp;
	}

	Matrix4& Matrix4::operator=(const Matrix4& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	const Matrix4& Matrix4::operator=(Matrix4& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& mat)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix4& Matrix4::operator-=(const Matrix4& mat)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& mat)
	{
		Matrix4 temp(*this);

		for (size_t i = 0; i < 4; i++)
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

	Matrix4 Matrix4::operator+(const Matrix4& mat) const
	{
		Matrix4 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix4 Matrix4::operator-(const Matrix4& mat) const
	{
		Matrix4 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix4 Matrix4::operator*(const Matrix4& mat) const
	{
		Matrix4 temp(*this);
		temp *= mat;
		return temp;
	}

	void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix)
	{

		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float range = 1.0f / (far_ - near_);

		matrix.m[0][0] = width * 2;

		matrix.m[1][1] = height * 2;

		matrix.m[2][2] = range;

		matrix.m[3][0] = -(left + right) * width;
		matrix.m[3][1] = -(top + bottom) * height;
		matrix.m[3][2] = range * -near_;
		matrix.m[3][3] = 1.0f;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;

		matrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, near_, far_);
	}

	void MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix)
	{
		float width = 1.0f / (right - left);
		float height = 1.0f / (top - bottom);
		float range = 1.0f / (far_ - near_);

		matrix.m[0][0] = width * 2;

		matrix.m[1][1] = height * 2;

		matrix.m[2][2] = range;

		matrix.m[3][0] = -(left + right) * width;
		matrix.m[3][1] = -(top + bottom) * height;
		matrix.m[3][2] = range * -near_;
		matrix.m[3][3] = 1.0f;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] = 0.0f;

	}

	void MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix)
	{

		float sinFov = 0.0f;
		float cosFov = 0.0f;
		SinCos(sinFov, cosFov, 0.5f * fovAngleY);

		float range = far_ / (far_ - near_);
		float height = cosFov / sinFov;

		matrix.m[0][0] = height / aspect;

		matrix.m[1][1] = cosFov / sinFov;

		matrix.m[2][2] = range;
		matrix.m[2][3] = 1.0f;

		matrix.m[3][2] = -range * near_;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] =
			matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][3] = 0.0f;
	}

	void MakePerspectiveR(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix)
	{
		float sinFov = 0.0f;
		float cosFov = 0.0f;
		SinCos(sinFov, cosFov, 0.5f * fovAngleY);

		float range = far_ / (far_ - near_);
		float height = cosFov / sinFov;

		matrix.m[0][0] = height / aspect;

		matrix.m[1][1] = cosFov / sinFov;

		matrix.m[2][2] = range;
		matrix.m[2][3] = 1.0f;

		matrix.m[3][2] = -range * near_;

		matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
			matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] =
			matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][3] = 0.0f;


	}

	void CoordinateTransformation3D(float& x, float& y, float& z, Matrix4& mat)
	{
		Vector3 vec(x, y, z);

		x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

		y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

		z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];
	}

	void MakeLookL(const Vector3& eye, const Vector3& target, const Vector3& up, Matrix4& mat)
	{
		Vector3 zVec = target - eye;
		zVec.Normal();

		Vector3 xVec = up.Cross(zVec);
		xVec.Normal();

		Vector3 yVec = zVec.Cross(xVec);
		yVec.Normal();

		mat.m[0][0] = xVec.x;
		mat.m[0][1] = xVec.y;
		mat.m[0][2] = xVec.z;
		mat.m[1][0] = yVec.x;
		mat.m[1][1] = yVec.y;
		mat.m[1][2] = yVec.z;
		mat.m[2][0] = zVec.x;
		mat.m[2][1] = zVec.y;
		mat.m[2][2] = zVec.z;
		mat.m[3][0] = eye.x;
		mat.m[3][1] = eye.y;
		mat.m[3][2] = eye.z;
	}

	Matrix4 AliceMathF::MakeViewport(float width, float height)
	{
		float winH = height / 2.0f;
		float winW = width / 2.0f;

		return { winW, 0, 0, 0, 0, -winH, 0, 0, 0, 0, 1, 0, winW, winH, 0, 1 };
	}
}