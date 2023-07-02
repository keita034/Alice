#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)

#include<assimp/quaternion.h>

#pragma warning(pop)

#include "Quaternion.h"
#include"AliceMathUtility.h"

float AngleNormalize(float x)
{
	return static_cast<float>(static_cast<int32_t>((x > 0.0f ? x : 360.0f + x)) % 360);
}

namespace AliceMathF
{

	Quaternion::Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_)
	{

	}

	Quaternion::Quaternion(const Vector3& v_, float angle_)
	{
		float lSin = sinf(angle_ / 2.0f);

		x = lSin * v_.x;
		y = lSin * v_.y;
		z = lSin * v_.z;
		w = cosf(angle_ / 2.0f);

	}

	Quaternion::Quaternion(const Vector4& v_)
	{
		x = v_.x;
		y = v_.y;
		z = v_.z;
		w = v_.w;
	}

	Quaternion::Quaternion(const DirectX::XMVECTOR& v_)
	{
		x = v_.m128_f32[0];
		y = v_.m128_f32[1];
		z = v_.m128_f32[2];
		w = v_.m128_f32[3];
	}

	Quaternion::Quaternion(const aiQuaternion& q_)
	{
		x = q_.x;
		y = q_.y;
		z = q_.z;
		w = q_.w;
	}

	Quaternion::Quaternion(const Matrix4& m_)
	{
		float lTr = m_.m[0][0] + m_.m[1][1] + m_.m[2][2] + m_.m[3][3];

		if (lTr >= 1.0f)
		{
			float fourD = 2.0f * sqrtf(lTr);
			x = (m_.m[1][2] - m_.m[2][1]) / fourD;
			y = (m_.m[2][0] - m_.m[0][2]) / fourD;
			z = (m_.m[0][1] - m_.m[1][0]) / fourD;
			w = fourD / 4.0f;
		}
		else
		{
			size_t i = 0;

			if (m_.m[0][0] <= m_.m[1][1])
			{
				i = 1;
			}
			if (m_.m[2][2] > m_.m[i][i])
			{
				i = 2;
			}

			size_t j = (i + 1) % 3;
			size_t k = (j + 1) % 3;

			lTr = m_.m[i][i] - m_.m[j][k] - m_.m[k][k] + 1.0f;

			float lFourD = 2.0f * sqrtf(lTr);
			float lQa[4]{};

			lQa[i] = lFourD / 4.0f;
			lQa[j] = (m_.m[j][i] + m_.m[i][j]) / lFourD;
			lQa[k] = (m_.m[k][i] + m_.m[i][k]) / lFourD;
			lQa[3] = (m_.m[j][k] - m_.m[k][j]) / lFourD;

			x = lQa[0];
			y = lQa[1];
			z = lQa[2];
			w = lQa[3];

		}
	}

	Quaternion::Quaternion(const Vector3& u_, const Vector3& v_)
	{
		Vector3 lVecU = u_.Normal();
		Vector3 lVecV = v_.Normal();

		float lDot = lVecU.Dot(lVecV);

		Vector3 lCross = lVecU.Cross(lVecV);

		Vector3 Axis = lCross.Normal();

		float lTheta = Acos(lDot);

		float lSin = Sin(lTheta / 2.0f);

		x = lSin * Axis.x;
		y = lSin * Axis.y;
		z = lSin * Axis.z;
		w = Cos(lTheta / 2.0f);
	}

	float Quaternion::Dot(const Quaternion& q_)const
	{
		return w * q_.w + x * q_.x + y * q_.y + z * q_.z;
	}

	float Quaternion::Norm()const
	{
		return sqrtf(Dot(*this));
	}

	Quaternion Quaternion::Normalize()const
	{
		float lLen = Norm();

		if (lLen != 0)
		{
			Quaternion lTmp = *this;
			lTmp /= lLen;

			return lTmp;
		}

		return *this;
	}

	Quaternion Quaternion::Identity() const
	{
		return { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	Quaternion Quaternion::Conjugate() const
	{
		return { -x,-y,-z,w };
	}

	Quaternion Quaternion::Inverse() const
	{
		return Conjugate() / (Norm() * Norm());
	}

	Quaternion Quaternion::Slerp(const Quaternion& q_, float t_)
	{
		float lCos = Dot(q_);

		Quaternion lT2 = *this;

		if (lCos < 0.0f)
		{
			lCos = -lCos;
			lT2 = -*this;
		}

		if (lCos >= 1.0f - 0.0005f)
		{
			return (1.0f - t_) * q_ + t_ * lT2;
		}

		float lK0 = t_;
		float lK1 = 1.0f - t_;

		float lTheta = acosf(lCos);

		lK0 = (sinf(lTheta * lK0) / sinf(lTheta));
		lK1 = (sinf(lTheta * lK1) / sinf(lTheta));

		return  lK0 * q_ + lK1 * lT2;
	}

	Quaternion Quaternion::Lerp(const Quaternion& q_, float t_)
	{
		float lCos = this->Dot(q_);
		Quaternion lT2 = q_;
		if (lCos < 0.0f)
		{
			lCos = -lCos;
			lT2 = -q_;
		}

		float lK0 = 1.0f - t_;
		float lK1 = t_;

		return *this * lK0 + lT2 * lK1;
	}

	Matrix4 Quaternion::Rotate()const
	{
		float lXX = x * x * 2.0f;
		float lYY = y * y * 2.0f;
		float lZZ = z * z * 2.0f;
		float lXY = x * y * 2.0f;
		float lXZ = x * z * 2.0f;
		float lYZ = y * z * 2.0f;
		float lWX = w * x * 2.0f;
		float lWY = w * y * 2.0f;
		float lWZ = w * z * 2.0f;

		Matrix4 lResult = {
			1.0f - lYY - lZZ,lXY + lWZ,lXZ - lWY,0.0f,
			lXY - lWZ, 1.0f - lXX - lZZ,lYZ + lWX,0.0f,
			lWZ + lWY, lYZ - lWX,1.0f - lXX - lYY,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};

		return lResult;
	}

	Vector3 Quaternion::GetAxis()
	{
		Vector3 lResult;

		float lX = x;
		float lY = y;
		float lZ = z;
		float lLen = Norm();

		lResult.x = lX / lLen;
		lResult.y = lY / lLen;
		lResult.z = lZ / lLen;

		return lResult;
	}

	Vector3 Quaternion::GetEulerAngles()
	{
		Vector3 lResult;

		float lSinX = 2 * y * z - 2 * x * w;
		float lAbsSinX = fabsf(lSinX);

		const float E = 0.001f;

		if (lAbsSinX < E)
		{
			lSinX = E;
		}

		lResult.x = asinf(-lSinX);
		float lCosX = cosf(x);

		float lSinY = (2 * x * z + 2 * y * w) / lCosX;
		float lCosY = (2 * powf(w, 2) + 2 * powf(z, 2) - 1) / lCosX;
		lResult.y = atan2f(lSinY, lCosY);

		float sinZ = (2 * x * y + 2 * z * w) / lCosX;
		float cosZ = (2 * powf(w, 2) + 2 * powf(y, 2) - 1) / lCosX;
		lResult.z = atan2f(sinZ, cosZ);

		lResult = lResult * (AX_PI / 180.0f);

		lResult.x = AngleNormalize(lResult.x);
		lResult.y = AngleNormalize(lResult.y);
		lResult.z = AngleNormalize(lResult.z);

		return lResult;
	}

	void Quaternion::SeTEuler(const Vector3& rot_)
	{
		float lCosR = cosf(rot_.x / 2.0f);
		float lSinR = sinf(rot_.x / 2.0f);
		float lCosP = cosf(rot_.y / 2.0f);
		float lSinP = sinf(rot_.y / 2.0f);
		float lCosY = cosf(rot_.z / 2.0f);
		float lSinY = sinf(rot_.z / 2.0f);


		y = lCosR * lSinP * lCosY + lSinR * lCosP * lSinY;
		z = lCosR * lCosP * lSinY - lSinR * lSinP * lCosY;
		x = lSinR * lCosP * lCosY - lCosR * lSinP * lSinY;
		w = lCosR * lCosP * lCosY + lSinR * lSinP * lSinY;
	}

	Quaternion Quaternion::operator+()
	{
		return *this;
	}

	Quaternion Quaternion::operator-()const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q_)
	{
		x += q_.x;
		y += q_.y;
		z += q_.z;
		w += q_.w;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q_)
	{

		x -= q_.x;
		y -= q_.y;
		z -= q_.z;
		w -= q_.w;

		return *this;
	}

	Quaternion& Quaternion::operator*=(float s_)
	{
		x *= s_;
		y *= s_;
		z *= s_;
		w *= s_;

		return *this;
	}

	Quaternion& Quaternion::operator/=(float s_)
	{
		return *this *= 1.0f / s_;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q_)
	{
		float lRX, lRY, lRZ, lRW;

		lRX = w * q_.x + q_.w * x + y * q_.z + -z * q_.y;
		lRY = w * q_.y + q_.w * y + z * q_.x + -x * q_.z;
		lRZ = w * q_.z + q_.w * z + x * q_.y + -y * q_.x;
		lRW = w * q_.w + -x * q_.x + -y * q_.y + -z * q_.z;

		x = lRX;
		y = lRY;
		z = lRZ;
		w = lRW;

		return *this;
	}

	const Quaternion operator+(const Quaternion& q1_, const Quaternion& q2_)
	{
		Quaternion lTmp = q1_;

		return lTmp += q2_;
	}

	const Quaternion operator-(const Quaternion& q1_, const Quaternion& q2_)
	{
		Quaternion result = q1_;
		return result -= q2_;
	}

	const Quaternion operator*(const Quaternion& q1_, const Quaternion& q2_)
	{
		Quaternion lTmp = q1_;

		return lTmp *= q2_;
	}

	const Quaternion operator*(const Quaternion& q_, float s_)
	{
		Quaternion lTmp = q_;

		return lTmp *= s_;
	}

	const Quaternion operator*(float s_, const Quaternion& q_)
	{
		Quaternion lTmp = q_;

		return lTmp *= s_;
	}

	const Quaternion operator/(const Quaternion& q_, float s_)
	{
		Quaternion result = q_;
		result /= s_;
		return result;
	}

	const Vector3 operator*(Quaternion q_, Vector3 vec)
	{
		float lX = q_.x * 2.0f;
		float lY = q_.y * 2.0f;
		float lZ = q_.z * 2.0f;
		float lXX = q_.x * lX;
		float lYY = q_.y * lY;
		float lZZ = q_.z * lZ;
		float lXY = q_.x * lY;
		float lXZ = q_.x * lZ;
		float lYZ = q_.y * lZ;
		float lWX = q_.w * lX;
		float lWY = q_.w * lY;
		float lWZ = q_.w * lZ;

		Vector3 lTmp;

		lTmp.x = (1.0f - (lYY + lZZ)) * vec.x + (lXY - lWZ) * vec.y + (lXZ + lWY) * vec.z;
		lTmp.y = (lXY + lWZ) * vec.x + (1.0f - (lXX + lZZ)) * vec.y + (lYZ - lWX) * vec.z;
		lTmp.z = (lXZ - lWY) * vec.x + (lYZ + lWX) * vec.y + (1.0f - (lXX + lYY)) * vec.z;

		return lTmp;
	}

	void QuaternionSlerp(Quaternion& vOut_, const Quaternion& qStart_, const Quaternion& qEnd_, float t_)
	{

		Quaternion start = Quaternion(qStart_);
		Quaternion end = Quaternion(qEnd_);

		vOut_ = start.Slerp(end, t_);
	}

	Quaternion LookRotation(const Vector3& forward, const Vector3& up)
	{
		Matrix4 m = CreateLookToMatrix({0.0f,0.0f,0.0f}, forward, up);

		Quaternion result = Quaternion(m);

		return result;
	}

}
