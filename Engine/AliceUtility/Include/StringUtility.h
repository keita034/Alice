#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<string>

ALICE_SUPPRESS_WARNINGS_END

namespace AliceUtility
{
	namespace String
	{
		/// <summary>
		/// マルチバイト文字列をワイド文字列に変換
		/// </summary>
		/// <param name="string">マルチバイト文字列</param>
		std::wstring StringToWstring(const std::string& string_);
	}
}