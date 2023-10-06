#include "StringUtility.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<Windows.h>
#include<filesystem>

ALICE_SUPPRESS_WARNINGS_END

std::wstring AliceUtility::String::StringToWstring(const std::string& string_)
{
	auto const dest_size = ::MultiByteToWideChar(CP_ACP,0U,string_.data(),-1,nullptr,0U);
	std::vector<wchar_t> dest(static_cast< const uint64_t >( dest_size ),L'\0');

	if ( ::MultiByteToWideChar(CP_ACP,0U,string_.data(),-1,dest.data(),static_cast< int32_t >( dest.size() )) == 0 )
	{
		throw std::system_error{ static_cast< int32_t >( ::GetLastError() ), std::system_category() };
	}

	dest.resize(std::char_traits<wchar_t>::length(dest.data()));
	dest.shrink_to_fit();

	return std::wstring(dest.begin(),dest.end());
}
