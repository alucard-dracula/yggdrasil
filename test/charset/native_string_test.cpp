//native_string_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <yggr/charset/native_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	{
	    //char str[2] = {'a'};// if not using gcc options -fshort-wchar in linux gcc sizoef(wchar_t) == 4
		char str[sizeof(wchar_t)] = {'a'};
		yggr_test_assert(&(yggr::charset::native_string<wchar_t>(str)[0]) == reinterpret_cast<wchar_t*>(str));
		yggr_test_assert(sizeof(yggr::charset::native_string<wchar_t>(str)) == sizeof(str));
	}

	{
		wchar_t str[2] = {L'a'};
		yggr_test_assert(&(yggr::charset::native_string<char>(str)[0]) == reinterpret_cast<char*>(str));
		yggr_test_assert(sizeof(yggr::charset::native_string<char>(str)) == sizeof(str));
	}

	{
		const char* str = "abc";
		yggr_test_assert((yggr::charset::native_string(str) == str));
	}

	{
		const char* str = "abc";
		yggr_test_assert((yggr::charset::native_string<wchar_t>(str) == reinterpret_cast<const wchar_t*>(str)));
	}

	{
		const wchar_t* str = L"abc";
		yggr_test_assert((yggr::charset::native_string<char>(str) == reinterpret_cast<const char*>(str)));
	}

	{
		std::string str = "abc";
		yggr_test_assert(0 == memcmp(str.c_str(), yggr::charset::native_string<std::wstring>(str).c_str(), str.size()));
		yggr_test_assert(0 == memcmp(str.c_str(), yggr::charset::native_string<std::wstring>(str, str.get_allocator()).c_str(), str.size()));
	}

	{
		std::string str = "abc";
		yggr_test_assert(str.c_str() == yggr::charset::native_string(str).c_str());
		yggr_test_assert(str.c_str() == yggr::charset::native_string(str, str.get_allocator()).c_str());
		yggr_test_assert(str.c_str() == yggr::charset::native_string<std::string>(str).c_str());
		yggr_test_assert(str.c_str() == yggr::charset::native_string<std::string>(str, str.get_allocator()).c_str());
	}

	{
		std::wstring str = L"abc";
		yggr_test_assert(0 == memcmp(str.c_str(), yggr::charset::native_string<std::string>(str).c_str(), str.size() * sizeof(wchar_t)));
		yggr_test_assert(0 == memcmp(str.c_str(), yggr::charset::native_string<std::string>(str, str.get_allocator()).c_str(), str.size() * sizeof(wchar_t)));
	}

	wait_any_key(argc, argv);
	return 0;
}
