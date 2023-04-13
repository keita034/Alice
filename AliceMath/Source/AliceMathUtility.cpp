#include "AliceMathUtility.h"

namespace AliceMathUtility
{
#pragma region 絶対値の計算

	//値を整数に
	int Abs(int abs)
	{
		return 	std::abs(abs);
	}

	//値を整数に
	long AbsL(long abs)
	{
		return 	std::labs(abs);
	}

	//値を整数に
	long long AbsLL(long long abs)
	{
		return 	std::llabs(abs);
	}

#pragma endregion 絶対値の計算
}