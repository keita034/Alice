#include "FilterShader.h"

physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	static_cast<void>(constantBlockSize);
	static_cast<void>(constantBlock);

	// トリガーを通す
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}
	//  上記でフィルタリングされなかったすべてのコンタクトを生成する
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	// (A,B)のペアに対するコンタクト・コールバックをトリガーする。
	// AのフィルターマスクはBのIDを含み、逆も同様。
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	}

	return physx::PxFilterFlag::eDEFAULT;

	
}
