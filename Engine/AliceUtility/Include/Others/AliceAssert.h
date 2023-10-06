#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>

#ifdef _DEBUG

namespace internal
{
    int Abort();
}

#define AliceAssert(expr,str,...) \
    (!(expr) && printf("%s(%ld): %s: "#expr"\n", __FILE__, __LINE__,#str, __VA_ARGS__) && internal::Abort())

#else

#define AliceAssert(expr,str,...)

#endif

//値が範囲外なのだ！
#define AliceAssertRange(value, min, max,str) \
    AliceAssert((min <= value) && (value <= max) && "(%d <= [%d] <= %d)",str, min, value, max);

//配列外アクセスなのだ！
#define AliceAssertArray(index, max,str) \
    AliceAssert((index < max) && "(0 <= [%zd] < %zd)",str, index, max);

//ここに来ちゃだめなのだ！
#define AliceAssertMustNotHappen(str) \
    AliceAssert(false && "(must not happen.)",str,);

//NULL アクセスなのだ！
#define AliceAssertNull(ptr,str) \
    AliceAssert(ptr != NULL && "(null access.)",str,);

//未実装アサート
#define ALICE_ASSERT_NOT_IMPLEMENTED AliceAssert(0,"Not implemented");

//スイッチ・ブロック内のデフォルトケースをエラーにする
#define ALICE_DEFAULT_CASE_NOT_IMPLEMENTED \
  default:                                 \
    ALICE_ASSERT_NOT_IMPLEMENTED           \
    break;

/// inVが2のべき乗かどうかをチェックする
template <typename T>
constexpr bool IsPowerOf2(T inV)
{
    return (inV & (inV - 1)) == 0;
}

/// inVを次のinAlignmentバイトまで揃える。
template <typename T>
inline T AlignUp(T inV, uint64_t inAlignment)
{
    AliceAssert(IsPowerOf2(inAlignment), "NotPowerOf2");
    return T((uint64_t(inV) + inAlignment - 1) & ~(inAlignment - 1));
}

ALICE_SUPPRESS_WARNINGS_END
