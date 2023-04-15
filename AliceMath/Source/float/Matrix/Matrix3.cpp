#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cmath>
#include<assimp/matrix3x3.h>

#pragma warning(pop)

#include "Matrix3.h"
#include"AliceMathF.h"

namespace AliceMathF
{
	Matrix3::Matrix3()
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
	}

	Matrix3::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
	}


	Matrix3::Matrix3(const Vector2& scale, float angle, const Vector2& trans)
	{
		AliceMathF::Matrix3 matScale, matRot, matTrans;

		//スケール、回転、平行移動行列の計算
		matScale.MakeScaling(scale);
		matRot.MakeRotation(angle);
		matTrans.MakeTranslation(trans);

		//ワールド行列の合成
		//変形をリセット
		*this = AliceMathF::MakeMat3Identity();
		//ワールド行列にスケーリングを反映
		*this *= matScale;
		//ワールド行列に回転を反映
		*this *= matRot;
		//ワールド行列に平行移動を反映
		*this *= matTrans;
	}

	Matrix3::Matrix3(const aiMatrix3x3& mat)
	{
		m[0][0] = mat.a1;
		m[0][1] = mat.a2;
		m[0][2] = mat.a3;

		m[1][0] = mat.b1;
		m[1][1] = mat.b2;
		m[1][2] = mat.b3;

		m[2][0] = mat.c1;
		m[2][1] = mat.c2;
		m[2][2] = mat.c3;
	}

	Matrix3 MakeMat3Identity()
	{
		Matrix3 mat;
		return mat;
	}

	Matrix3 MakeInverse(const Matrix3* mat)
	{
		assert(mat);

		//掃き出し法を行う行列
		float sweep[3][6]{};
		//定数倍用
		float constTimes = 0.0f;
		//許容する誤差
		float MAX_ERR = 1e-10f;
		//戻り値用
		Matrix3 retMat;

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				//weepの左側に逆行列を求める行列をセット
				sweep[i][j] = mat->m[i][j];

				//sweepの右側に単位行列をセット
				sweep[i][3 + j] = MakeMat3Identity().m[i][j];
			}
		}

		//全ての列の対角成分に対する繰り返し
		for (size_t i = 0; i < 3; i++)
		{
			//最大の絶対値を注目対角成分の絶対値と仮定
			float max = std::fabs(sweep[i][i]);
			size_t maxIndex = i;

			//i列目が最大の絶対値となる行を探す
			for (size_t j = i + 1; j < 3; j++)
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
				return MakeMat3Identity();
			}

			//操作(1):i行目とmaxIndex行目を入れ替える
			if (i != maxIndex)
			{
				for (size_t j = 0; j < 6; j++)
				{
					float tmp = sweep[maxIndex][j];
					sweep[maxIndex][j] = sweep[i][j];
					sweep[i][j] = tmp;
				}
			}

			//sweep[i][i]に掛けると1になる値を求める
			constTimes = 1 / sweep[i][i];

			//操作(2):p行目をa倍する
			for (size_t j = 0; j < 6; j++)
			{
				//これによりsweep[i][i]が1になる
				sweep[i][j] *= constTimes;
			}

			//操作(3)によりi行目以外の行のi列目を0にする
			for (size_t j = 0; j < 6; j++)
			{
				if (j == i)
				{
					//i行目はそのまま
					continue;
				}

				//i行目に掛ける値を求める
				constTimes = -sweep[j][i];

				for (size_t k = 0; k < 6; k++)
				{
					//j行目にi行目をa倍した行を足す
					//これによりsweep[j][i]が0になる
					sweep[j][k] += sweep[i][k] * constTimes;
				}
			}
		}

		//sweepの右半分がmatの逆行列
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				retMat.m[i][j] = sweep[i][3 + j];
			}
		}

		return retMat;
	}

	Matrix3::Matrix3(const DirectX::XMFLOAT3X3& Matrix3)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = Matrix3.m[i][j];
			}
		}
	}

	void Matrix3::MakeScaling(const Vector2& scale)
	{
		//スケーリング倍率を行列に設定する
		m[0][0] = scale.x;
		m[1][1] = scale.y;
	}

	void Matrix3::MakeScaling(float x, float y)
	{
		MakeScaling(Vector2(x, y));
	}

	void Matrix3::MakeTranslation(const Vector2& trans)
	{
		m[3][0] = trans.x;
		m[3][1] = trans.y;
	}

	void Matrix3::MakeTranslation(float x, float y)
	{
		MakeTranslation(Vector2(x, y));
	}

	void Matrix3::MakeRotation(float angle)
	{
		m[0][0] = Cos(angle);
		m[0][1] = -Sin(angle);

		m[1][0] = Sin(angle);
		m[1][1] = Cos(angle);
	}

	Matrix3::operator DirectX::XMFLOAT3X3() const
	{
		DirectX::XMFLOAT3X3 mat {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2]};

		return mat;
	}

	Matrix3 Matrix3::Transpose()
	{
		Matrix3 tmp(*this);
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				float f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	Matrix3& Matrix3::operator=(const Matrix3& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	const Matrix3& Matrix3::operator=(Matrix3& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& mat)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] += mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix3& Matrix3::operator-=(const Matrix3& mat)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] -= mat.m[i][j];
			}
		}

		return *this;
	}

	Matrix3& Matrix3::operator*=(const Matrix3& mat)
	{
		Matrix3 temp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				double f = 0.0;
				for (size_t k = 0; k < 3; k++)
				{
					f += (double)temp.m[i][k] * (double)mat.m[k][j];

					m[i][j] = (float)f;
				}


			}
		}
		return *this;
	}

	Matrix3 Matrix3::operator+(const Matrix3& mat) const
	{
		Matrix3 temp(*this);
		temp += mat;
		return temp;
	}

	Matrix3 Matrix3::operator-(const Matrix3& mat) const
	{
		Matrix3 temp(*this);
		temp -= mat;
		return temp;
	}

	Matrix3 Matrix3::operator*(const Matrix3& mat) const
	{
		Matrix3 temp(*this);
		temp *= mat;
		return temp;
	}
}