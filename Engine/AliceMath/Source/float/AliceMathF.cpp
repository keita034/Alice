#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cmath>
#include<random>

#pragma warning(pop)

#include "AliceMathF.h"
#include"AliceMathUtility.h"

namespace AliceMathF
{
#pragma region sin,cos,tan関数

#pragma region sin関数

	//sin関数
	float Sin(float sin_)
	{
		return std::sinf(sin_);
	}

	//asin関数
	float Asin(float sin_)
	{
		return std::asinf(sin_);
	}

#pragma endregion sin関数

#pragma region cos関数

	//cos関数
	float Cos(float cos_)
	{
		return std::cosf(cos_);
	}

	//acos関数
	float Acos(float cos_)
	{
		return std::acosf(cos_);
	}

#pragma endregion cos関数

#pragma region tan関数

	//tan関数
	float Tan(float tan_)
	{
		return std::tanf(tan_);
	}

	//atan関数
	float Atan(float tan_)
	{
		return std::atanf(tan_);
	}


	//atan2関数
	float Atan2(float y_, float x_)
	{
		return std::atan2f(y_, x_);
	}

#pragma endregion tan関数

	//sin、cosを両方出す
	void SinCos(float& sin_, float& cos_, float angle_)
	{
		sin_ = Sin(angle_);
		cos_ = Cos(angle_);
	}

#pragma endregion sin,cos,tan関数

#pragma region 角度変換

	//度数法からラジアン変換
	float ChangeRadians(const float& angle_)
	{
		float lRadian = angle_ * DEG_TO_RAD;
		return lRadian;
	}

	// ラジアンから度数法変換
	float ChangeDira(const float& angle_)
	{
		float lDegree = angle_ * RAD_TO_DEG;
		return lDegree;
	}


#pragma endregion 角度変換

#pragma region sqrt関数

	//sqrt関数
	float Sqrt(float sqrt_)
	{
		return std::sqrtf(sqrt_);
	}

#pragma endregion sqrt関数

#pragma region pow関数

	//pow関数
	float Pow(float pow_, float powNum_)
	{
		return std::powf(pow_, powNum_);
	}

#pragma endregion pow関数

#pragma region exp関数

	//exp関数
	float Exp(float exp_)
	{
		return std::expf(exp_);
	}

#pragma endregion exp関数

#pragma region log関数

	//log関数
	float Log(float log_)
	{
		return std::logf(log_);
	}

	//log10関数
	float Log10(float log_)
	{
		return std::log10f(log_);
	}

#pragma endregion log関数

#pragma region 絶対値の計算

	//絶対値の計算
	float Abs(float abs_)
	{
		return 	std::fabsf(abs_);
	}

#pragma endregion 絶対値の計算

#pragma region ceil関数

	//ceil関数
	float Ceil(float ceil_)
	{
		return std::ceilf(ceil_);
	}

#pragma endregion ceil関数

#pragma region floor関数

	//floor関数
	float Floor(float floor_)
	{
		return std::floorf(floor_);
	}

#pragma endregion floor関数

#pragma region round関数

	//round関数
	float Round(float round_)
	{
		return std::roundf(round_);
	}

#pragma endregion round関数

	// 値の符号を返す
	float Sign(float sign_)
	{
		return (sign_ >= 0.0f) ? 1.0f : (-1.0f);
	}

	// 値を範囲内に納める
	float Clamp(float value_, const float low_, const float high_)
	{
		if (high_ < value_)
		{
			value_ = high_;
		}

		if (value_ < low_)
		{
			value_ = low_;
		}

		return value_;
	}

	// 0と1の間に値を納め、その値を返す
	float Clamp01(float value_)
	{
		if (value_ < 0.0f)
		{
			return 0.0f;
		}

		if (value_ > 1.0f)
		{
			return 1.0f;
		}

		return value_;
	}

	// 線形補間(0～1)
	float Lerp(float a_, float b_, float t_)
	{
		return a_ + (b_ - a_) * Clamp01(t_);
	}

	Vector3 Lerp(const Vector3& a_, const Vector3& b_, float t_)
	{
		Vector3 lTmp;
		lTmp.x = a_.x + (b_.x - a_.x) * Clamp01(t_);
		lTmp.y = a_.y + (b_.y - a_.y) * Clamp01(t_);
		lTmp.z = a_.z + (b_.z - a_.z) * Clamp01(t_);
		return lTmp;
	}

	Vector4 Lerp(const Vector4& a_, const Vector4& b_, float t_)
	{
		Vector4 lTmp;
		lTmp.x = a_.x + (b_.x - a_.x) * Clamp01(t_);
		lTmp.y = a_.y + (b_.y - a_.y) * Clamp01(t_);
		lTmp.z = a_.z + (b_.z - a_.z) * Clamp01(t_);
		lTmp.w = a_.w + (b_.w - a_.w) * Clamp01(t_);
		return lTmp;
	}

	Vector3 Slerp(const Vector3& v1_, const Vector3& v2_, float t_)
	{
		Vector3 lTmp1 = v1_.Normal();
		Vector3 lTmp2 = v2_.Normal();
		float lOmega = Acos(lTmp1.Dot(lTmp2));

		float lSinOmega = Sin(lOmega);

		Vector3 lTermOne = v1_ * (Sin(lOmega * (1.0f - t_)) / lSinOmega);
		Vector3 lTermTwo = v2_ * (Sin(lOmega * (t_)) / lSinOmega);

		return lTermOne + lTermTwo;
	}

	// 線形補間(0～)
	float LerpUnclamped(float a_, float b_, float t_)
	{
		return a_ + (b_ - a_) * t_;
	}

	// Lerpの角度版(0～1)
	float LerpAngle(float a_, float b_, float t_)
	{
		float lNum = Repeat(b_ - a_, 360.0f);

		if (lNum > 180.0f)
		{
			lNum -= 360.0f;
		}

		return a_ + lNum * Clamp01(t_);
	}

	// tはlength_より大きくはならず0より小さくならず、その間をループ(float)
	float Repeat(float t_, float length_)
	{
		return Clamp(t_ - Floor(t_ / length_) * length_, 0.0f, length_);
	}

	// 現在地から目的地まで一定速度で移動
	float MoveTowards(float current_, float target_, float maxDelta_)
	{
		if (Abs(target_ - current_) <= maxDelta_)
		{
			return target_;
		}

		return current_ + Sign(target_ - current_) * maxDelta_;
	}

	// 現在の角度から目的の角度まで一定速度で移動
	float MoveTowardsAngle(float current_, float target_, float maxDelta_)
	{
		float lNum = DeltaAngle(current_, target_);

		if (0.0f - maxDelta_ < lNum && lNum < maxDelta_)
		{
			return target_;
		}

		target_ = current_ + lNum;
		return MoveTowards(current_, target_, maxDelta_);
	}

	// 角度の差を返す(度数法)
	float DeltaAngle(float current_, float target_)
	{
		float lNum = Repeat(target_ - current_, 360.0f);

		if (lNum > 180.0f)
		{
			lNum -= 360.0f;
		}

		return lNum;
	}

	// スムージング補間
	float SmoothStep(float from_, float to_, float t_)
	{
		t_ = Clamp01(t_);
		t_ = -2.0f * t_ * t_ * t_ + 3.0f * t_ * t_;
		return to_ * t_ + from_ * (1.0f - t_);
	}

	// ガンマ補正(float)
	float Gamma(float value_, float absmax_, float gamma_)
	{
		bool lFlag = value_ < 0.0f;
		float lNum = Abs(value_);
		if (lNum > absmax_)
		{
			return lFlag ? (0.0f - lNum) : lNum;
		}

		float lNum2 = Pow(lNum / absmax_, gamma_) * absmax_;
		return lFlag ? (0.0f - lNum2) : lNum2;
	}

	// 何を何処に何秒で指定してオブジェクトを移動させる
	float SmoothDamp(float current_, float target_, float& currentVelocity_, float smoothTime_, float maxSpeed_, float deltaTime_)
	{
		smoothTime_ = AliceMathUtility::Max(0.0001f, smoothTime_);
		float lNum = 2.0f / smoothTime_;
		float lNum2 = lNum * deltaTime_;
		float lNum3 = 1.0f / (1.0f + lNum2 + 0.48f * lNum2 * lNum2 + 0.235f * lNum2 * lNum2 * lNum2);
		float lValue = current_ - target_;
		float lNum4 = target_;
		float lNum5 = maxSpeed_ * smoothTime_;
		lValue = Clamp(lValue, 0.0f - lNum5, lNum5);
		target_ = current_ - lValue;
		float lNum6 = (currentVelocity_ + lNum * lValue) * deltaTime_;
		currentVelocity_ = (currentVelocity_ - lNum * lNum6) * lNum3;
		float lNum7 = target_ + (lValue + lNum6) * lNum3;
		if (lNum4 - current_ > 0.0f == lNum7 > lNum4)
		{
			lNum7 = lNum4;
			currentVelocity_ = (lNum7 - lNum4) / deltaTime_;
		}

		return lNum7;
	}

	// tが0とlength_の間で往復する(0～length_)
	float PingPong(float t_, float length_)
	{
		t_ = Repeat(t_, length_ * 2.0f);
		return length_ - Abs(t_ - length_);
	}

	// lerpの補間値を出す
	float InverseLerp(float a_, float b_, float value_)
	{
		if (a_ != b_)
		{
			return Clamp01((value_ - a_) / (b_ - a_));
		}

		return 0.0f;
	}

	// 2本の線の交点を計算
	bool LineIntersection(const Vector2& p1_, const Vector2& p2_, const Vector2& p3_, const Vector2& p4_, Vector2& result_)
	{
		float lNum = p2_.x - p1_.x;
		float lNum2 = p2_.y - p1_.y;
		float lNum3 = p4_.x - p3_.x;
		float lNum4 = p4_.y - p3_.y;
		float lNum5 = lNum * lNum4 - lNum2 * lNum3;
		if (lNum5 == 0.0f)
		{
			return false;
		}

		float lNum6 = p3_.x - p1_.x;
		float lNum7 = p3_.y - p1_.y;
		float lNum8 = (lNum6 * lNum4 - lNum7 * lNum3) / lNum5;
		result_.x = p1_.x + lNum8 * lNum;
		result_.y = p1_.y + lNum8 * lNum2;
		return true;
	}

	bool LineSegmentIntersection(const Vector2& p1_, const Vector2& p2_, const Vector2& p3_, const Vector2& p4_, Vector2& result_)
	{
		float lNum = p2_.x - p1_.x;
		float lNum2 = p2_.y - p1_.y;
		float lNum3 = p4_.x - p3_.x;
		float lNum4 = p4_.y - p3_.y;
		float lNum5 = lNum * lNum4 - lNum2 * lNum3;
		if (lNum5 == 0.0f)
		{
			return false;
		}

		float lNum6 = p3_.x - p1_.x;
		float lNum7 = p3_.y - p1_.y;
		float lNum8 = (lNum6 * lNum4 - lNum7 * lNum3) / lNum5;
		if (lNum8 < 0.0f || lNum8 > 1.0f)
		{
			return false;
		}

		float num9 = (lNum6 * lNum2 - lNum7 * lNum) / lNum5;
		if (num9 < 0.0f || num9 > 1.0f)
		{
			return false;
		}

		result_.x = p1_.x + lNum8 * lNum;
		result_.y = p1_.y + lNum8 * lNum2;
		return true;
	}

	// 二つの値がほぼ等しいか
	bool Approximately(float a, float b)
	{
		float lTmp = 1e-06f * AliceMathUtility::Max(Abs(a), Abs(b));

		float tmp2 = EPSILON * 8.0f;

		if (Abs(b - a) < AliceMathUtility::Max(lTmp, tmp2))
		{
			return true;
		}

		return false;
	}

	// ランダムな値を取得
	float GetRand(float min_, float max_)
	{
		// 乱数生成器
		static std::random_device slRD;
		static std::default_random_engine lsEngine(slRD());

		// 分布生成器
		std::uniform_real_distribution<float> lsDistribution(min_, max_);

		// 乱数を生成
		return lsDistribution(lsEngine);
	}

	Vector3 HermiteGetPoint(const Vector3& p0_, const Vector3& p1_, const Vector3& v0_, const Vector3& v1, float t_)
	{
		Vector3 lC0 = 2.0f * p0_ + -2.0f * p1_ + v0_ + v1;
		Vector3 lC1 = -3.0f * p0_ + 3.0f * p1_ + -2.0f * v0_ - v1;
		Vector3 lC2 = v0_;
		Vector3 lC3 = p0_;

		float lT2 = t_ * t_;
		float lT3 = lT2 * t_;
		return lC0 * lT3 + lC1 * lT2 + lC2 * t_ + lC3;
	}

	Vector3 CatmullRomSpline(std::vector<Vector3>& points_, float t_)
	{

		float lLength = static_cast<float>(points_.size());
		float lProgress = (lLength - 1) * t_;
		float lIndex = std::floor(lProgress);
		float lWeight = lProgress - lIndex;

		if (Approximately(lWeight, 0.0f) && lIndex >= lLength - 1)
		{
			lIndex = lLength - 2;
			lWeight = 1;
		}

		Vector3 lP0 = points_[static_cast<size_t>(lIndex)];
		Vector3 lP1 = points_[static_cast<size_t>(lIndex + 1.0f)];
		Vector3 lP2;
		Vector3 lP3;

		if (lIndex > 0.0f)
		{
			lP2 = 0.5f * (points_[static_cast<size_t>(lIndex + 1.0f)] - points_[static_cast<size_t>(lIndex - 1.0f)]);
		}
		else
		{
			lP2 = points_[static_cast<size_t>(lIndex + 1.0f)] - points_[static_cast<size_t>(lIndex)];
		}

		if (lIndex < lLength - 2.0f)
		{
			lP3 = 0.5f * (points_[static_cast<size_t>(lIndex + 2.0f)] - points_[static_cast<size_t>(lIndex)]);
		}
		else
		{
			lP3 = points_[static_cast<size_t>(lIndex + 1.0f)] - points_[static_cast<size_t>(lIndex)];
		}

		return HermiteGetPoint(lP0, lP1, lP2, lP3, lWeight);
	}

	// 分離軸に投影された軸成分から投影線分長を算出
	float LenSegOnSeparateAxis(const Vector3* sep_, const Vector3* e1_, const Vector3* e2_, const Vector3* e3_)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		// 分離軸Sepは標準化されていること
		float lR1 = AliceMathF::Abs(sep_->Dot(*e1_));
		float lR2 = AliceMathF::Abs(sep_->Dot(*e2_));
		float lR3 = e3_ ? (AliceMathF::Abs(sep_->Dot(*e3_))) : 0;
		return lR1 + lR2 + lR3;
	}

	//対角線を出す
	float GetDiagonal(float width_, float height_)
	{
		float lDiagonal = 0.0f;

		lDiagonal = Sqrt(height_ * height_ + width_ * width_);

		return lDiagonal;
	}

	float GetCenter(float p1_, float p2_)
	{
		return (p1_ + p2_) / 2.0f;
	}
}