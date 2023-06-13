#include "AliceMathUtility.h"

namespace AliceMathUtility
{
#pragma region 絶対値の計算

	//値を整数に
	int32_t Abs(int32_t abs)
	{
		return 	std::abs(abs);
	}

	//値を整数に
	int64_t Abs(int64_t abs)
	{
		return 	std::llabs(abs);
	}

#pragma endregion 絶対値の計算
}