#include "AliceAssert.h"
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN


#ifdef _DEBUG
int internal::Abort()
{
	::abort();
	return 1;
}
#endif

ALICE_SUPPRESS_WARNINGS_END
