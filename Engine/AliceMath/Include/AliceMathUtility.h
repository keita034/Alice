#pragma once
#include"AliceMathF.h"
#include"AliceMath.h"

namespace AliceMathUtility
{
#pragma region 絶対値の計算

	//絶対値の計算
	int32_t Abs(int32_t abs);

	//絶対値の計算
	int64_t Abs(int64_t abs);

#pragma endregion 絶対値の計算

#pragma region 比較関数

	/// <summary>
	/// 値が大きい方を返す
	/// </summary>
	template<typename T>
	T Max(T a_, T b_)
	{
		return (a_ > b_) ? a_ : b_;
	}

	/// <summary>
	/// 値が大きい奴を返す(vector)
	/// </summary>
	template<typename T>
	T Max(const std::vector<T>& values_)
	{
		size_t lNum = values_.size() - 1;

		if (lNum == 0)
		{
			return static_cast<T>(0);
		}

		float lNum2 = values_[0];

		for (size_t i = 1; i < lNum; i++)
		{
			if (values_[i] > lNum2)
			{
				lNum2 = values_[i];
			}
		}

		return lNum2;
	}

	/// <summary>
	/// 値が小さい方を返す
	/// </summary>
	template<typename T>
	T Min(T a_, T b_)
	{
		return (a_ < b_) ? a_ : b_;
	}

	/// <summary>
	/// 値が小さい奴を返す(vector)
	/// </summary>
	template<typename T>
	T Min(std::vector<T> values_)
	{
		size_t lNum = values_.size() - 1;

		if (lNum == 0)
		{
			return static_cast<T>(0);
		}

		float lNum2 = values_[0];

		for (size_t i = 1; i < lNum; i++)
		{
			if (values_[i] < lNum2)
			{
				lNum2 = values_[i];
			}
		}

		return lNum2;
	}

#pragma endregion 比較関数

	int32_t GetRand(int32_t min,int32_t max);
};