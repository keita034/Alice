#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<string>

ALICE_SUPPRESS_WARNINGS_END

namespace AliceUtility
{
	namespace String
	{
		std::wstring StringToWstring(const std::string& string_);
	}
}