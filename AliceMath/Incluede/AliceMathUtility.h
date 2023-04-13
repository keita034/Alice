#pragma once
#include"AliceMathF.h"
#include"AliceMath.h"

namespace AliceMathUtility
{
#pragma region 絶対値の計算

	//絶対値の計算
	int Abs(int abs);

	//絶対値の計算
	long AbsL(long abs);

	//絶対値の計算
	long long AbsLL(long long abs);

#pragma endregion 絶対値の計算

#pragma region 比較関数

	/// <summary>
	/// 値が大きい方を返す
	/// </summary>
	template<typename T>
	T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	/// <summary>
	/// 値が大きい奴を返す(vector)
	/// </summary>
	template<typename T>
	T Max(std::vector<T> values)
	{
		int num = values.size() - 1;

		if (num == 0)
		{
			return static_cast<T>(0);
		}

		float num2 = values[0];

		for (int i = 1; i < num; i++)
		{
			if (values[i] > num2)
			{
				num2 = values[i];
			}
		}

		return num2;
	}

	/// <summary>
	/// 値が小さい方を返す
	/// </summary>
	template<typename T>
	T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	/// <summary>
	/// 値が小さい奴を返す(vector)
	/// </summary>
	template<typename T>
	T Min(std::vector<T> values)
	{
		int num = values.size() - 1;

		if (num == 0)
		{
			return static_cast<T>(0);
		}

		float num2 = values[0];

		for (int i = 1; i < num; i++)
		{
			if (values[i] < num2)
			{
				num2 = values[i];
			}
		}

		return num2;
	}

#pragma endregion 比較関数
};