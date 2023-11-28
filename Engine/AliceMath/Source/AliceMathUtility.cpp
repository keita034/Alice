#include "AliceMathUtility.h"

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<cmath>
#include<random>

#pragma warning(pop)

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

	int32_t GetRand(int32_t min_,int32_t max_)
	{
		// 乱数生成器
		static std::random_device slRD;
		static std::default_random_engine lsEngine(slRD());

		// 分布生成器
		std::uniform_int_distribution<int32_t> lsDistribution(min_,max_);

		// 乱数を生成
		return lsDistribution(lsEngine);
	}

#pragma endregion 絶対値の計算
}