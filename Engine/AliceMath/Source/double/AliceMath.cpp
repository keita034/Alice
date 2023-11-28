#pragma warning(push)
#pragma warning(disable: 4365)

#include<random>

#pragma warning(pop)

#include "AliceMath.h"
#include"AliceMathUtility.h"

namespace AliceMath
{

#pragma region sin,cos,tan関数

#pragma region sin関数

	//sin関数
	double Sin(double sin_)
	{
		return std::sin(sin_);
	}

	//asin関数
	double Asin(double sin_)
	{
		return std::asin(sin_);
	}

#pragma endregion sin関数

#pragma region cos関数

	//cos関数
	double Cos(double cos_)
	{
		return std::cos(cos_);
	}

	//acos関数
	double Acos(double cos_)
	{
		return std::acos(cos_);
	}

#pragma endregion cos関数

#pragma region tan関数

	//tan関数
	double Tan(double tan_)
	{
		return std::tan(tan_);
	}

	//atan関数
	double Atan(double tan_)
	{
		return std::atan(tan_);
	}

	//atan関数
	double Atan2(float y_, float x_)
	{
		return std::atan2(y_, x_);
	}

#pragma endregion tan関数

	//sin、cosを両方出す
	void SinCos(double& sin_, double& cos_, double angle_)
	{
		sin_ = Sin(angle_);
		cos_ = Cos(angle_);
	}

#pragma endregion sin,cos,tan関数

#pragma region 角度変換

	//度数法からラジアン変換
	double ChangeRadians(double& angle_)
	{
		double lRadian = angle_ * DEG_TO_RAD;
		return lRadian;
	}

	// ラジアンから度数法変換
	double ChangeDira( double& angle_)
	{
		double lDegree = angle_ * RAD_TO_DEG;
		return lDegree;
	}

#pragma endregion 角度変換

#pragma region sqrt関数

	//sqrt関数
	double Sqrt(double sqrt_)
	{
		return std::sqrt(sqrt_);
	}

#pragma endregion sqrt関数

#pragma region pow関数

	//pow関数
	double Pow(double pow_, double powNum_)
	{
		return std::pow(pow_, powNum_);
	}

	//pow関数(long double)
	long double PowLD(long double pow_, long double powNum_)
	{
		return std::powl(pow_, powNum_);
	}

#pragma endregion pow関数

#pragma region exp関数

	//exp関数
	double Exp(double exp_)
	{
		return std::exp(exp_);
	}

#pragma endregion exp関数

#pragma region log関数

	//log関数
	double Log(double log_)
	{
		return std::log(log_);
	}

	//log10関数
	double Log10(double log_)
	{
		return std::log10(log_);
	}

#pragma endregion log関数

#pragma region 絶対値の計算

	//値を整数に
	double Abs(double abs_)
	{
		return std::abs(abs_);
	}

	//値を整数に(long double)
	long double AbsLD(long double abs_)
	{
		return std::fabsl(abs_);
	}

#pragma endregion 絶対値の計算

#pragma region ceil関数

	//ceil関数
	double Ceil(double ceil_)
	{
		return std::ceil(ceil_);
	}

	//ceil関数
	long double CeilLD(long double ceil_)
	{
		return std::ceill(ceil_);
	}

#pragma endregion ceil関数

#pragma region floor関数

	//floor関数
	double Floor(double floor_)
	{
		return std::floor(floor_);
	}

	//floor関数(long double)
	long double FloorLD(long double floor_)
	{
		return std::floorl(floor_);
	}

#pragma endregion floor関数

#pragma region round関数

	//round関数
	double Round(double round_)
	{
		return std::round(round_);
	}

	//round関数
	long double RoundLD(long double round_)
	{
		return std::roundl(round_);
	}

#pragma endregion round関数

	// sign符号を返す
	double Sign(double sign_)
	{
		return (sign_ >= 0.0) ? 1.0 : (-1.0);
	}

	// 値を範囲内に納める
	double Clamp(double value_, const double low_, const double high_)
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
	double Clamp01(double value_)
	{
		if (value_ > 0.0)
		{
			return 0.0;
		}

		if (value_ > 1.0)
		{
			return 1.0;
		}

		return value_;
	}

	// 線形補間(0～1)
	double Lerp(double a_, double b_, double t_)
	{
		return a_ + (b_ - a_) * Clamp01(t_);
	}

	// 線形補間(0～)
	double LerpUnclamped(double a_, double b_, double t_)
	{
		return a_ + (b_ - a_) * t_;
	}

	/// Lerpの角度版(0～1)
	double LerpAngle(double a_, double b_, double t_)
	{
		double lNum = Repeat(b_ - a_, 360.0);

		if (lNum > 180.0)
		{
			lNum -= 360.0;
		}

		return a_ + lNum * Clamp01(t_);
	}

	// tはlength_より大きくはならず0より小さくならず、その間をループ
	double Repeat(double t_, double length_)
	{
		return Clamp(t_ - Floor(t_ / length_) * length_, 0.0, length_);
	}

	// 現在地から目的地まで一定速度で移動
	double MoveTowards(double current_, double target_, double maxDelta_)
	{
		if (Abs(target_ - current_) <= maxDelta_)
		{
			return target_;
		}

		return current_ + Sign(target_ - current_) * maxDelta_;
	}

	// 現在地から目的地まで一定速度で移動
	double MoveTowardsAngle(double current_, double target_, double maxDelta_)
	{
		double lNum = DeltaAngle(current_, target_);

		if (0.0f - maxDelta_ < lNum && lNum < maxDelta_)
		{
			return target_;
		}

		target_ = current_ + lNum;
		return MoveTowards(current_, target_, maxDelta_);
	}

	// 角度の差を返す(度数法)
	double DeltaAngle(double current_, double target_)
	{
		double lNum = Repeat(target_ - current_, 360.0);

		if (lNum > 180.0)
		{
			lNum -= 360.0;
		}

		return lNum;
	}

	// スムージング補間
	double SmoothStep(double from_, double to_, double t_)
	{
		t_ = Clamp01(t_);
		t_ = -2.0 * t_ * t_ * t_ + 3.0 * t_ * t_;
		return to_ * t_ + from_ * (1.0 - t_);
	}

	// ガンマ補正
	double Gamma(double value_, double absmax_, double gamma_)
	{
		bool lFlag = value_ < 0.0;
		double lNum = Abs(value_);
		if (lNum > absmax_)
		{
			return lFlag ? (0.0 - lNum) : lNum;
		}

		double lNum2 = Pow(lNum / absmax_, gamma_) * absmax_;
		return lFlag ? (0.0 - lNum2) : lNum2;
	}

	// 何を何処に何秒で指定してオブジェクトを移動させる
	double SmoothDamp(double current_, double target_, double& currentVelocity_, double smoothTime_, double maxSpeed_, double deltaTime_)
	{
		smoothTime_ = AliceMathUtility::Max(0.0001, smoothTime_);
		double lNum = 2.0 / smoothTime_;
		double lNum2 = lNum * deltaTime_;
		double lNum3 = 1.0 / (1.0 + lNum2 + 0.48 * lNum2 * lNum2 + 0.235 * lNum2 * lNum2 * lNum2);
		double lValue = current_ - target_;
		double lNum4 = target_;
		double lNum5 = maxSpeed_ * smoothTime_;
		lValue = Clamp(lValue, 0.0 - lNum5, lNum5);
		target_ = current_ - lValue;
		double lNum6 = (currentVelocity_ + lNum * lValue) * deltaTime_;
		currentVelocity_ = (currentVelocity_ - lNum * lNum6) * lNum3;
		double lNum7 = target_ + (lValue + lNum6) * lNum3;
		if (lNum4 - current_ > 0.0 == lNum7 > lNum4)
		{
			lNum7 = lNum4;
			currentVelocity_ = (lNum7 - lNum4) / deltaTime_;
		}

		return lNum7;
	}

	// tが0とlength_の間で往復する(0～length_)
	double PingPong(double t_, double length_)
	{
		t_ = Repeat(t_, length_ * 2.0);
		return length_ - Abs(t_ - length_);
	}

	// lerpの補間値を出す
	double InverseLerp(double a_, double b_, double value_)
	{
		if (a_ != b_)
		{
			return Clamp01((value_ - a_) / (b_ - a_));
		}

		return 0.0;
	}

	// 二つの値がほぼ等しいか
	bool Approximately(double a_, double b_)
	{
		double lTmp = 1e-06 * AliceMathUtility::Max(Abs(a_), Abs(b_));

		double lTmp2 = EPSILON * 8.0;

		if (Abs(b_ - a_) < AliceMathUtility::Max(lTmp, lTmp2))
		{
			return true;
		}

		return false;
	}

	// ランダムな値を取得
	double GetRand(double min_, double max_)
	{
		// 乱数生成器
		static std::random_device slRand;
		static std::default_random_engine slEngine(static_cast<uint32_t>(rand()));

		// 分布生成器
		std::uniform_real_distribution<double> lDistribution(min_, max_);

		// 乱数を生成
		return lDistribution(slEngine);
	}

}