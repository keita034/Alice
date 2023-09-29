#pragma once
#include"Vector4.h"
#include"Vector3.h"
#include"Matrix4.h"

namespace physx
{
	class PxQuat;
}

namespace JPH
{
	class Quat;
}

namespace AliceMathF
{
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		//クオータニオンを作成

		//成分を指定して クォータニオンを作成
		Quaternion(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 1.0f);

		//任意軸まわりの回転の指定からクォータニオンを作成
		Quaternion(const Vector3& v_, float angle_);

		//回転行列からクォータニオンを作成
		Quaternion(const Matrix4& m_);

		Quaternion(const aiQuaternion& q_);

		Quaternion(const physx::PxQuat& q_);

		Quaternion(const Vector4& v_);

		Quaternion(const DirectX::XMVECTOR& v_);

		Quaternion(const JPH::Quat& q_);


		//ベクトルからベクトルの角度
		Quaternion(const Vector3& u, const Vector3& v_);

		//内積を求める
		float Dot(const Quaternion& q_)const;

		//ノルムを求める
		float Norm()const;

		//正規化する
		Quaternion Normal() const;
		Quaternion& Normal();

		//単位クオータニオン
		Quaternion Identity() const;

		// 共役クオータニオン
		Quaternion Conjugate() const;

		// 逆クオータニオン
		Quaternion Inverse() const;

		//クオータニオン球面線形補間
		Quaternion Slerp(const Quaternion& q_, float t_);

		//クオータニオン補間正規化済み
		Quaternion Nlerp(const Quaternion& q_, float t_);

		//クオータニオン線形補間
		Quaternion Lerp(const Quaternion& q_, float t_);

		//クオータニオンから回転行列を求める
		Matrix4 Rotate()const;

		//回転軸の算出
		Vector3 GetAxis();

		//オイラー角に変換
		Vector3 GetEulerAngles();

		//オイラー角からクオータニオンに変換
		void SeTEuler(const Vector3& rot_);

		//単項演算子オーバーロード
		Quaternion operator + ();
		Quaternion operator - ()const;

		//代入演算子オーバーロード
		Quaternion& operator += (const Quaternion& q_);
		Quaternion& operator -= (const Quaternion& q_);
		Quaternion& operator *= (float s_);
		Quaternion& operator /= (float s_);
		Quaternion& operator *= (const Quaternion& q_);

		operator physx::PxQuat() const;
		operator JPH::Quat()const;
	};

	//2項演算子オーバーロード
	const Quaternion operator + (const Quaternion& q1_, const Quaternion& q2_);
	const Quaternion operator - (const Quaternion& q1_, const Quaternion& q2_);
	const Quaternion operator * (const Quaternion& q1_, const Quaternion& q2_);
	const Quaternion operator * (const Quaternion& q_, float s_);
	const Quaternion operator * (float s_, const Quaternion& q_);
	const Quaternion operator / (const Quaternion& q_, float s_);
	const Vector3 operator*(Quaternion q_, Vector3 vec_);

	void QuaternionSlerp(Quaternion& vOut_, const Quaternion& qStart_, const Quaternion& qEnd_, float t_);

	Quaternion LookRotation(const Vector3& forward, const Vector3& up = {0.0f,-1.0f,0.0f });

}